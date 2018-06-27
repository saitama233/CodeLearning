// 包含了习题5.1-2和5.1-3
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int crandom(int a, int b);
static unsigned int seed;

int main(void)
{
    int begin, end;

    printf("Please input a range:\nbegin:\n");
    scanf("%d", &begin);
    printf("end:\n");
    scanf("%d", &end);
    srand(time(0));

    printf("The random result is %d\n", crandom(begin, end));
    return 0;
}

int biase_random()
{
    return rand() % 2;
}

// 调用两次BASED-RANDOM，调用两次后，都产生1的概率是p^2，
// 都产生0的概率是(1-p)^2，而产生一个0，一个1的概率是2p(1-p)
int random01()
{
    int x, y;
    while (1) {
        x = biase_random();
        y = biase_random();
        if (x != y)
            return x;
    }
}

// Randome01()只能产生0或者1，恰好二进制只有0或者1，
// 故可以利用Random01()产生Random(a,b)的每一位，
// 计算b的位数，如果生成的数不在[a,b]范围内，就重新生成。
int crandom(int a, int b)
{
    int cur = 0;
    int tmp = 0;

    if (a > b) {
        printf("range error!");
        exit(-1);
    }

    while (1) {
        do {
            tmp = cur;
            cur <<= 1;
            cur += random01();
        } while (cur < b);
        if (tmp > a)
            return tmp;
    }
}
