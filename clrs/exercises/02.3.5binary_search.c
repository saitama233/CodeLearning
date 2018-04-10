#include <stdio.h>
int bsearch(int a[], int n, int value);
int binary_search(int a[], int beg, int end, int value);

int main(void)
{
    int a[] = {5, 20, 21, 45, 56, 100};
    int i = bsearch(a, 6, 33);
    int j = bsearch(a, 6, 56);
    printf("i = %d, j = %d\n", i, j);
    return(0);
}

int bsearch(int a[], int n, int value)
{
    binary_search(a, 0, n, value);
}

int binary_search(int a[], int beg, int end, int value)
{
    int i;
    int mid = (beg + end) / 2;
    if (value == a[mid])
        return mid;
    if (beg + 1 >= end)
        return -1;

    if (value > a[mid])
        binary_search(a, mid, end, value);
    else if (value < a[mid])
        binary_search(a, beg, mid, value);
}
