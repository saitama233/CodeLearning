#include "opend.h"
#include <syslog.h>
#include <fcntl.h>

void handle_request(char *buf, int nread, int clifd, uid_t uid)
{
    int newfd;
    openlog("myopend", LOG_PERROR|LOG_PID , LOG_USER);

    if (buf[nread-1] != 0) {
        syslog(LOG_ERR, "request not null terminated: %*.*s", nread, nread, buf);
        snprintf(errmsg, MAXLINE-1, "request from uid %d not null terminated: %*.*s\n", uid, nread, nread, buf);
        send_err(clifd, -1, errmsg);
        return;
    }
    log_msg("request: %s, from uid %d", buf, uid);
    if (buf_args(buf, cli_args) < 0) {
        syslog(LOG_ERR, "buf_args error");
        send_err(clifd, -1, errmsg);
        return;
    }
    syslog(LOG_ERR, "pathname = %s", pathname);
    if ((newfd = open(pathname, oflag)) < 0) {
        syslog(LOG_ERR, "can't open %s: %s", pathname, strerror(errno));
        snprintf(errmsg, MAXLINE-1, "can't open %s: %s\n",
                pathname, strerror(errno));
        send_err(clifd, -1, errmsg);
        log_msg(errmsg);
        return;
    }
    syslog(LOG_ERR, "send_fd(fd = %d, newfd = %d)", clifd, newfd);
    if (send_fd(clifd, newfd) < 0)
        err_sys("send_fd error");
    log_msg("sent fd %d over fd %d for %s", newfd, clifd, pathname);
    close(newfd);
}
