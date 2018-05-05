#include <stdio.h>
#include <stdlib.h>

void inverse_pairs(int *a, int n, int *num);
int inversions(int *a, int beg, int end, int *num, int *tmparr);
void merge(int *a, int beg, int mid, int end, int *num, int *tmparr);

int main(void)
{
    int a[] = {3, 2, 8, 6, 1, 5, 7, 10, 5};
    int num = 0;
    int n = sizeof(a) / sizeof(int);

    inverse_pairs(a, n, &num);
    printf("num = %d\n", num);

    return(0);
}

void inverse_pairs(int *a, int n, int *num)
{
    int *tmparr = NULL;
    if ((tmparr = malloc(n * sizeof(int))) < 0)
        perror("malloc");
    inversions(a, 0, n, num, tmparr);
    free(tmparr);
    tmparr = NULL;
}

int inversions(int *a, int beg, int end, int *num, int *tmparr)
{
    int mid = (beg + end) / 2;
    if (beg + 1 < end) {
        inversions(a, beg, mid, num, tmparr);
        inversions(a, mid, end, num, tmparr);
        merge(a, beg, mid, end, num, tmparr);
    }
}

void merge(int *a, int beg, int mid, int end, int *num, int *tmparr)
{
    int i = beg, j = mid;
    int index = 0;
    int tmp = 0;
    
    while (i < mid && j < end) {
        if (a[i] <= a[j])
            tmparr[index++] = a[i++];
        else {
            *num += j - i - tmp;
            tmp++;
            tmparr[index++] = a[j++];
        }
    }
    while (i < mid) {
        tmparr[index++] = a[i++];
    }
    while (j < end) {
        tmparr[index++] = a[j++];
    }
    for (index = 0, i = beg; i < end;) {
        a[i++] = tmparr[index++];
    }
}
