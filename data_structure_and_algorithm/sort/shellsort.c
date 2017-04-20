#include <stdio.h>

typedef int elementtype;

void print_array(elementtype a[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void
shellsort(elementtype a[], int n)
{
    int i, j, increment;
    elementtype tmp;

    for (increment = n / 2; increment > 0; increment /= 2) {
        for (i = increment; i < n; i++) {
            tmp = a[i];
            for (j = i; j >= increment; j -= increment) {
                if (tmp < a[j - increment])
                    a[j] = a[j-increment];
                else
                    break;
            }
            a[j] = tmp;
        }
        print_array(a, n);
    }
}

int main()
{
    elementtype a[] = {81,94,11,96,12,35,17,95,28,58,41,75,15};
    printf("Before shellsort: \n");
    print_array(a, 13);
    shellsort(a, 13);
    printf("After shellsort: \n");
    print_array(a, 13);

    return(0);
}
