#include <stdio.h>
typedef int elementtype;

void print(elementtype *a, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void swap(elementtype *a, elementtype *b)
{
    elementtype tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/*自己写的效率没那么高
void insertion_sort(elementtype a[], int n)
{
    elementtype i, j;
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (a[i] < a[j])
                swap(a+i, a+j);
        }
    }
}
*/

void
insertion_sort(elementtype a[], int n)
{
    int i, j;
    elementtype tmp;
    
    for (i = 1; i < n; i++) {
        tmp = a[i];
        for (j = i; j > 0 && a[j-1] > tmp; j--)
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

int main()
{
    elementtype a[] = {34, 8, 64, 51, 32, 21};
    printf("Before insertion_sort: ");
    print(a, 6);
    insertion_sort(a, 6);
    printf("After insertion_sort: ");
    print(a, 6);
    return(0);
}
