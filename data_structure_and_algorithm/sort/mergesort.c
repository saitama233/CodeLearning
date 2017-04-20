#include <stdio.h>
#include <stdlib.h>
typedef int elementtype;

void print_array(elementtype a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

void
merge(elementtype a[], elementtype *tmparray, 
        int lpos, int rpos, int rightend)
{
    int i = lpos;
    int tmppos = lpos;
    int leftend = rpos;
    while (lpos < leftend && rpos < rightend) {
        if (a[lpos] <= a[rpos])
            tmparray[tmppos++] = a[lpos++];
        else
            tmparray[tmppos++] = a[rpos++];
    }
    while (lpos < leftend)
        tmparray[tmppos++] = a[lpos++];
    while (rpos < rightend)
        tmparray[tmppos++] = a[rpos++];
    for (i; i < rightend; i++)
        a[i] = tmparray[i];
}

void
msort(elementtype a[], elementtype *tmparray, int left, int right)
{
    int center;
    if (left + 1 < right) {
        center = (left + right) / 2;
        msort(a, tmparray, left, center);
        msort(a, tmparray, center, right);
        merge(a, tmparray, left, center, right);
//        printf("left:%d, center:%d, right:%d\n", left, center, right);
//        print_array(a, 8);
    }
}

void
mergesort(elementtype a[], int n)
{
    elementtype *tmparray;

    tmparray = (elementtype *)malloc(sizeof(elementtype) * n);
    if (tmparray != NULL) {
        msort(a, tmparray, 0, n);
        free(tmparray);
    } else {
        printf("no space for tmp array!!!\n");
    }
}

int
main(void)
{
    int n;
    elementtype a[] = {24,13,26,1,2,27,38,15,100,5,50};
    n = sizeof(a) / sizeof(elementtype);
    printf("Before sort:\n");
    print_array(a, n);
    mergesort(a, n);
    printf("After sort:\n");
    print_array(a, n);
}
