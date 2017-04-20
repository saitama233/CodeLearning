#include <apue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

int main(void)
{
    DIR* dir;
    int dirfd1;
    if (open("tempfile", O_RDWR) < 0)
        err_sys("open error");

    dir = opendir(".");
    if (NULL == dir)
    {
        perror("open dir error");
        return -1;
    }
    dirfd1 = dirfd(dir);
    if (-1 == dirfd1)
    {
        perror("dirfd error");
        return -1;
    }
    if (unlinkat(dirfd1, "tempfile", 0) < 0)
        err_sys("unlink error");
    printf("file unlined\n");
    sleep(15);
    printf("done\n");
    exit(0);
}
