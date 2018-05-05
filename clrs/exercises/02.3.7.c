// 网上的思路
// 以O(nlgn)的时间查找一个数组里sum正好等于x的两个数
#include <stdio.h>
#include "../sorting.c"

int find_sum(int *a, int n, int x);

int main(void)
{
    int a[9] = {20, 5, 13, 3, 7, 23, 40, 33, 50};
    int x;
    int n = sizeof(a)/sizeof(int);
    //printf("n = %d\n", n);

    print_arr(a, n);
    printf("请输入一个数字：");
    scanf("%d", &x);
    mergesort(a, n);
    find_sum(a, n, x);
    return 0;
}

int find_sum(int *a, int n, int x)
{
    int i = 0, j = n - 1;
    while (i <= j) {
        if (a[i] + a[j] == x) {
            printf("a[i] = %d, a[j] = %d, x = %d\n", a[i], a[j], x);
            return 1;
        } else if (a[i] + a[j] > x)
            j--;
        else if (a[i] + a[j] < x)
            i++;
    }
}
