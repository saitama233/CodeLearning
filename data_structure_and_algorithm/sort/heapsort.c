#include <stdio.h>
#define leftchild(i) (2 * (i) + 1)
typedef int elementtype;

void print_array(elementtype a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", *(a+i));
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

void
percdown(elementtype a[], int i, int n)
{
    int child;
    elementtype tmp;

    for (tmp = a[i]; leftchild(i) < n; i = child) {
        child = leftchild(i);
        if (child != n - 1 && a[child+1] > a[child])
            child++;
        if (tmp < a[child])
            a[i] = a[child];
        else
            break;
    }
    a[i] = tmp;
}

void
heapsort(elementtype a[], int n)
{
    int i;
    for (i = n / 2; i >= 0; i--)
        percdown(a, i, n);
    for (i = n - 1; i > 0; i--) {
        swap(&a[0], &a[i]);
        percdown(a, 0, i);
    }
}

int main()
{
    int n;
    elementtype a[] = {1,9,2,10,3,11,4,12,6,13,6,14,7,15,8,16};
    n = sizeof(a)/sizeof(elementtype);
    printf("Before heapsort:\n");
    print_array(a, n);
    heapsort(a, n);
    printf("After heapsort:\n");
    print_array(a, n);
    return(0);
}
