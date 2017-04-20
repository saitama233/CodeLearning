#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int my_system(const char *cmdstring);

int main(int argc, char *argv[])
{
    system("date");
    my_system("date");
    my_system(argv[1]);
}

int my_system(const char *cmdstring)
{
    pid_t   pid;
    int     status;

    if (cmdstring == NULL)
        return(1);

    if ((pid = fork()) < 0) {
        status = -1;
    } else if(pid == 0) {
        execl("/bin/bash", "bash", "-c", cmdstring, (char *)0);
        _exit(127);
    } else {
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return(status);
}
