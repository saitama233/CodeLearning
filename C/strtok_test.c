#include <stdio.h>
#include <string.h>

int main(void) {
    char s[] = "ab-cd : ef;gh :i-jkl;mnop;qrs-tu: vwx-y;z";
    // char *delim = "-:";
    char *delim = "-: ";
    char *p;

    printf("\"%s\"\n", strtok(s, delim));
    while ((p = strtok(NULL, delim)))
        printf("\"%s\"\n", p);
    
    //字符串连接
    char *a = "open" " ";
    printf("%s\n", a);
    return(0);
}
