#define _GNU_SOURCE
#include "apue.h"
#include "15.2.3tell_wait.c"
#include <fcntl.h>
#include <sys/mman.h>

#define NLOOPS 1000
#define SIZE   sizeof(long)

static int update(long *ptr)
{
    return((*ptr)++);
}

int main(void)
{
    int     fd, i, counter;
    pid_t   pid;
    void    *area;
#if 1
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, 
                    MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED)
        err_sys("mmap error");
#else
    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
            == MAP_FAILED)
        err_sys("mmap error");
    close(fd);
# endif

    tell_wait();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);
            
            tell_child(pid);
            wait_child();
        }
    } else {
        for (i = 1; i < NLOOPS + 1; i += 2) {
            wait_parent();
            
            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            tell_parent(getppid());
        }
    }
    exit(0);
}
