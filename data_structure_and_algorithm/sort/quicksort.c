#include <stdio.h>
#define cutoff (3)
typedef int elementtype;

void
print_array(elementtype a[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void 
swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

elementtype
median3(elementtype a[], int left, int right)
{
    int center = (left + right) / 2;

    if (a[left] > a[center])
        swap(a + left, a + center);
    if (a[left] > a[right])
        swap(a + left, a + right);
    if (a[center] > a[right])
        swap(a + center, a + right);

    swap(a + center, a + right-1);
    return(a[right - 1]);
}

void
qsort(elementtype a[], int left, int right)
{
    int i, j;
    elementtype pivot;
    
    if (left + cutoff <= right) {
        pivot = median3(a, left, right);
        i = left; 
        j = right - 1;
        for (;;) {
            while(a[++i] < pivot) {}
            while(a[--j] > pivot) {}
            if (i < j)
                swap(&a[i], &a[j]);
            else
                break;
        }
        swap(&a[i], &a[right - 1]);

        qsort(a, left, i - 1);
        qsort(a, i + 1, right);
    } else
        insertionsort(a + left, right - left + 1);
}

void 
quicksort(elementtype a[], int n)
{
    qsort(a, 0, n - 1);
}

int 
main(void)
{
    elementtype a[] = {10, 5, 78, 111, 34, 55, 100, 300, 201, 2, 70};
    printf("Before insertion_sort: ");
    print_array(a, 11);
    insertion_sort(a, 11);
    printf("After insertion_sort: ");
    print_array(a, 11);
    return(0);
}
