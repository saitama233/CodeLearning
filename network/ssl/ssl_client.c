// Usage: ssl_client ip port

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAXBUF 1024
void showcerts(SSL *ssl);

int main(int argc, char **argv)
{
    char pwd[100] = {0};
    char *cafile;
    int i, sockfd, len, fd, size;
    char filename[50];
    struct sockaddr_in serv;
    char buffer[MAXBUF + 1];
    SSL_CTX *ctx;
    SSL *ssl;
    int mode;

    if (argc != 3) {
        printf("Usage: \n\t%s IP Port\n\tSuch as:\t%s 127.0.0.1 80\n",
                argv[0], argv[0]);
        exit(0);
    }

    // SSL 库初始化
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    getcwd(pwd, 100);
    if (strlen(pwd) == 1)
        pwd[0] = '\0';
    cafile = strcat(pwd, "/ca/certs/ca.cer");
    printf("capath = %s\n", cafile);
    if (SSL_CTX_load_verify_locations(ctx, cafile, NULL) == 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    } else {
        printf("SSL_CTX_load_verify_locations success\n");
    }

#if CMODE == CLIENT_SSL_VERIFY_NONE
#warning client SSL_VERIFY_NONE
    // 此时服务器可以不发送证书，或发送错误的证书，因为客户端即使验证失败也可以握手成功，可以调用在connect后调用ssl_get_verify_result来查看是否验证成功
    mode = SSL_VERIFY_NONE;
#elif CMODE == CLIENT_SSL_VERIFY_PERR
#warning client SSL_VERIFY_PEER
    // 此时服务器证书验证失败会导致握手失败
    mode = SSL_VERIFY_PEER;
#endif
    
    // 根据mode设置客户端验证方式
    SSL_CTX_set_verify(ctx, mode, NULL);

#if SMODE == SERVER_SSL_VERIFY_PEER || SMODE == (SERVER_SSL_VERIFY_PEER|SERVER_SSL_VERIFY_FAIL_IF_NO_PEER_CERT)
#warning server SSL_VERIFY_PEER
    // 只有服务器设置了SSL_VERIFY_PEER才需要发送客户端证书

    // getcwd()会将当前工作目录的绝对路径复制到参数buffer所指的内存空间中
    getcwd(pwd, 100);
    // 当为根路径的时候（/），需要置为空，否则之后的strcat会出错
    if (strlen(pwd) == 1)
        pwd[0] = '\0';

    // 载入客户端的数字证书，此证书用来发送给服务器，包含客户端的公钥
    // 这一步需要输入客户端私钥密码
    printf("call SSL_CTX_use_certificate_file:\n");
    if (SSL_CTX_use_certificate_file(ctx, 
                strcat(pwd, "/ca/certs/client.cer"),
                SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    getcwd(pwd, 100);
    if (strlen(pwd) == 1)
        pwd[0] = '\0';
    
    // 加载客户端私钥密码,这样就不需要每次运行都输入密码了
    SSL_CTX_set_default_passwd_cb_userdata(ctx, "zxcasd");

    // 加载客户端私钥
    printf("call SSL_CTX_use_PrivateKey_file:\n");
    if (SSL_CTX_use_PrivateKey_file(ctx, 
                strcat(pwd, "/ca/private/client-key.pem"),
                SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
#endif

    // 创建一个socket用于tcp通信
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(errno);
    }
    printf("socket created\n");

    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serv.sin_addr.s_addr) == 0) {
        perror("inet_pton");
        exit(errno);
    }
    printf("address created\n");

    if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) != 0) {
        perror("connect");
        exit(errno);
    }
    printf("server connected\n");

    // 基于ctx产生一个新的ssl
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    // 建SSL连接
    if (SSL_connect(ssl) == -1)
        ERR_print_errors_fp(stderr);
    else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        // 可选，就是显示服务器的证书，不调用也不影响之后的连接
        showcerts(ssl);
    }
    if (SSL_get_verify_result(ssl) == X509_V_OK) {
        printf("The server sent a certificate "
                "which verified OK\n");
    } else
        printf("The server sent a invalid certificate\n");
    // 接收用户输入的文件名，并打开文件
    printf("Please input the filename which you want to load:\n");
    scanf("%s",filename);
    if ((fd = open(filename, O_RDONLY, 0666)) < 0) {
        perror("open");
        exit(1);
    }

    // 将用户输入的文件名，去掉路径信息后，发给服务器
    for (i = strlen(filename); i >= 0 && filename[i] != '/'; i--);

    if (SSL_write(ssl, filename+i+1, strlen(filename+i+1)) < 0) {
        printf("'%s' message send failure: %s", 
                filename+i+1, strerror(errno));
    }

    bzero(buffer, MAXBUF + 1);
    while (size = read(fd, buffer, 1024)) {
        if (size < 0) {
            perror("read");
            exit(errno);
        } else {
            if (SSL_write(ssl, buffer, size) < 0)
                printf("'%s' message send failure: %s",
                        buffer, strerror(errno));
        }
        bzero(buffer, MAXBUF + 1);
    }
    printf("send complete!\n");

    // 关闭连接
    close(fd);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    return(0);
}

void showcerts(SSL *ssl)
{
    X509 *cert;
    char *tmp;

    cert = SSL_get_peer_certificate(ssl);//此时已经建立好了ssl连接
    if (cert != NULL) {
        printf("Digital certificate information:\n");
        tmp = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Certificate: %s\n", tmp);
        free(tmp);
        tmp = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuser: %s\n", tmp);
        free(tmp);
        X509_free(cert);
    } else
        printf("No certificate information!\n");
}
