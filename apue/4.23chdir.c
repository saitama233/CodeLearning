#include <apue.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("usage: chdir <directory name>");
    if (chdir(argv[1]) < 0)
        err_sys("change to directory %s failed\n", argv[1]);
    printf("chdir to %s succeeded.\n", argv[1]);
    exit(0);
}
