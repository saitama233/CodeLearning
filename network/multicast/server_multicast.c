// 多播测试程序

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int                 sockfd;
    int                 err = -1;
    char                group[16] = "224.0.0.88";//局部链路多播地址
    struct sockaddr_in  addr;
    struct ip_mreq      mreq;
    int                 times = 0;
    int                 addr_len = 0;
    char                buf[256] = {0};
    int                 n = 0;

    // 多播和广播只能用UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err(1, "socket error");     
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9527);

    if ((err = bind(sockfd, (struct sockaddr *)&addr, 
                    sizeof(addr))) == -1) {
        err(1, "bind error");
    }

    // 多播地址,类似于QQ群号
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interfae.s_addr = htonl(INADDR_ANY);
    // 加入多播组,类似于加入QQ群
    if ((err = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                    &mreq, sizeof(mreq))) == -1) {
        err(1, "setsockopt IP_ADD_MEMBERSHIP error");
    }

    // 循环接收多播组的消息
    for (times = 0; times < 5; times++) {
        addr_len = sizeof(addr);
        memset(buf, 0, sizeof(buf));
        if ((n = recvfrom(sockfd, buf, sizeof(buf), 0, 
                    (struct sockaddr *)&addr, &addr_len)) == -1) {
            err(1, "recvfrom error");
        }

        printf("Recv %dst message from sender: %s\n", times, buf);
        sleep(2);
    }
    
    setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, 
            sizeof(mreq));

    close(sockfd);

    return(0);
}
