#include "open.h"
#include <syslog.h>
#include <fcntl.h>

#define BUFFSIZE 8192

int main(int argc, char *argv[])
{
    int     n, fd;
    char    buf[BUFFSIZE];
    char    line[MAXLINE];

    openlog("myopen", LOG_PERROR|LOG_PID , LOG_USER);
    while (fgets(line, MAXLINE, stdin) != NULL) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = 0;
        if ((fd = csopen(line, O_RDONLY)) < 0)
            continue;
        syslog(LOG_ERR, "received fd is %d", fd);
        while ((n = read(fd, buf, BUFFSIZE)) > 0)
            if (write(STDOUT_FILENO, buf, n) != n)
                err_sys("write error");
        if (n < 0)
            err_sys("read error");
        close(fd);
    }
}
