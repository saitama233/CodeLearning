// gcc -DMODE=2 -o ssl_server ssl_server.c -lssl -lcrypto
// MODE定义为2表示双向认证，定义为1表示单向认证
// Usage: ssl_server port lisnum

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAXBUF 1024

int main(int argc, char ** argv)
{
    char    pwd[100], file[50];
    char    *cafile;
    char    cli_ip[20];
    char    buf[MAXBUF];
    int     sockfd, clifd, fd;
    int     port, lisnum, len;
    SSL_CTX *ctx;
    SSL     *ssl;
    struct sockaddr_in  serv, cli_addr;
    int mode;

    if (argv[1])
        port = atoi(argv[1]);
    else {
        port = 7838;
        argv[2] = argv[3] = NULL;
    }

    if (argv[2])
        lisnum = atoi(argv[2]);
    else {
        lisnum = 5;
        argv[3] = NULL;
    }

    // SSL库初始化
    SSL_library_init();
    // 载入所有SSL算法
    OpenSSL_add_all_algorithms();
    // 载入所有SSL错误消息
    SSL_load_error_strings();
    // 以SSLv2和v3标准兼容方式产生一个SSL_CTX
    if ((ctx = SSL_CTX_new(SSLv23_server_method())) == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

#if MODE == 2
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
#endif

#if MODE == 1
#warning SSL_VERIFY_NONE
    mode = SSL_VERIFY_NONE;
#elif MODE == 2
#warning SSL_VERIFY_PEER
    mode = SSL_VERIFY_PEER;
#endif
    
    //这个函数要是不调用的话默认就是单向认证
    SSL_CTX_set_verify(ctx, mode, NULL);
    
    // getcwd()会将当前工作目录的绝对路径复制到参数buffer所指的内存空间中
    getcwd(pwd, 100);
    // 当为根路径的时候（/），需要置为空，否则之后的strcat会出错
    if (strlen(pwd) == 1)
        pwd[0] = '\0';

    // 载入服务器的数字证书，此证书用来发送给客户端，包含服务器的公钥
    // 这一步需要输入服务器公钥密码
    printf("call SSL_CTX_use_certificate_file:\n");
    if (SSL_CTX_use_certificate_file(ctx, 
                strcat(pwd, "/ca/certs/server.cer"),
                SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    getcwd(pwd, 100);
    if (strlen(pwd) == 1)
        pwd[0] = '\0';
    // 加载服务器私钥
    // 感觉是自己用来解密或加密用的
    printf("call SSL_CTX_use_PrivateKey_file:\n");
    if (SSL_CTX_use_PrivateKey_file(ctx, 
                strcat(pwd, "/ca/private/server-key.pem"),
                SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    // 可以不调用，也能正常通信
    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    } else
        printf("socket created\n");

    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serv, sizeof(struct sockaddr)) == -1) {
        perror("bind error");
        exit(1);
    } else
        printf("binded\n");

    if (listen(sockfd, lisnum) == -1) {
        perror("listen");
        exit(1);
    } else
        printf("begin listening\n");

    len = sizeof(struct sockaddr);
    while (1) {
        if ((clifd = accept(sockfd, 
                        (struct sockaddr *)&cli_addr, &len)) == -1) {
            perror("accept error");
            exit(errno);
        } else {
            inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, 
                cli_ip, sizeof(cli_ip));
            printf("server: got connection from %s\n", cli_ip);
        }

        // 基于ctx创建一个新的ssl
        ssl = SSL_new(ctx);
        // 将ssl绑定到客户端连接过来的socket上
        SSL_set_fd(ssl, clifd);
        // 建立SSL连接
        if (SSL_accept(ssl) == -1) {
            perror("ssl_accept error");
            ERR_print_errors_fp(stdout);
            break;
        }
        printf("SSL_accept ok!\n");

        // 接收客户端传送的文件名，并在特定目录创建空文件
        bzero(buf, MAXBUF + 1);
        bzero(file, 50);
        if (SSL_read(ssl, buf, MAXBUF) < 0) {
            perror("ssl_read error");
            exit(1);
        }
        printf("buf is %s\n", buf);
        if ((fd = open(strcat(file, buf), O_CREAT|O_RDWR, 0660)) < 0) {
            perror("open");
        }
        printf("fd is %d\n", fd);

        // 接收客户端的数据并写入文件
        bzero(buf, MAXBUF + 1);
        while (len = SSL_read(ssl, buf, MAXBUF)) {
            printf("buf is %s\n", buf);
            if (len < 0) {
                perror("SSL_read error");
                exit(1);
            }
            if (write(fd, buf, len) < 0) {
                perror("write error");
                exit(1);
            }
            bzero(buf, MAXBUF + 1);
        }

        close(fd);
        close(clifd);
        SSL_free(ssl);
    }

    close(clifd);
    close(sockfd);
    SSL_CTX_free(ctx);
    SSL_free(ssl);
    return(0);
}
