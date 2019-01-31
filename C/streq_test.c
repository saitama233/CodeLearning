#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(void) {
	char source[] = "hello, world! welcome to china!";
	char delim[] = " ,!";
	char *token;
	char *s = strdup(source); /* strseq会改变原字符串，复制一个， */
    char *tmp = s;

    /* 每次使用s都指向找到的delim字符串中分隔符的下一个字符位置，如第一次指向,后的空格，第二次指向world的w */
	/* for (token = strsep(&s, delim); token != NULL; token = strsep(&s, delim)) { */
    while ((token = strsep(&s, delim))) {
		printf("token is %s\n", token);
		printf("s is %s\n", s);
	}
    free(tmp);
	return 0;
}
