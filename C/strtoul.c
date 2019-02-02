#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* ULONG_MAX */

int main()
{
    char *end;
    int base = 10;
    int ret;
    char *value;
    
    /* 完全为数字时, end会指向最后的'\0', *end = '\0', ret = 12345 */
    value = "12345";
    ret = strtoul(value, &end, base);
    printf("*end is %c\n", *end); 
    printf("ret is %d\n", ret);

    /* *end = a, ret = 0 */
    value = "a12345b";
    ret = strtoul(value, &end, base);
    printf("*end is %c\n", *end);
    printf("ret is %d\n", ret);
    
    /* *end = b, ret = 12345 */
    value = "12345b";
    ret = strtoul(value, &end, base);
    printf("*end is %c\n", *end);
    printf("ret is %d\n", ret);

	value = "123456";
	ret = strtoul(value, &end, base);
	/* 用来判断是否完全是数字 */
    if (*end || ret == ULONG_MAX)
	    printf("The value %s is not a invalid number(containing character)\n", value);

    return 0;
}
