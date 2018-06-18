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
    int sockfd;
    struct sockaddr_in  dest_addr;

    char buf[] = "HELLO MULTICAST";

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err(1, "socket error");
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr("224.0.0.88");
    dest_addr.sin_port = htons(9527);

    while (1) {
        int n = sendto(sockfd, buf, strlen(buf), 0, 
                (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (n < 0)
            err(1, "sendto error");
        sleep(1);
    }

    return 0;
}
