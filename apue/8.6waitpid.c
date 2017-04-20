#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t   pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {   //first child
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        }
        else if (pid > 0)  //second parent  == first child
            exit(0);

        sleep(2);       //second child
        printf("\nsecond child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid)//first parent,waiting for first child
        err_sys("waitpid error");
    else
        printf("wait first child %d ok\n", pid);

    exit(0);
}
