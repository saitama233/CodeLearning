#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sorting.c"

int main(void)
{
    int n;
    clock_t t_start, t_end;
    double duration;
    int *barr, *sarr, *iarr, *sharr, *marr, *harr, *qarr;
    printf("请输入数组大小：");
    scanf("%d",&n);
    barr = malloc(sizeof(int) * n);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        barr[i] = rand()%10000;
    }
    sarr = malloc(sizeof(int) * n);
    memcpy(sarr, barr, sizeof(int) * n);
    iarr = malloc(sizeof(int) * n);
    memcpy(iarr, barr, sizeof(int) * n);
    sharr = malloc(sizeof(int) * n);
    memcpy(sharr, barr, sizeof(int) * n);
    marr = malloc(sizeof(int) * n);
    memcpy(marr, barr, sizeof(int) * n);
    harr = malloc(sizeof(int) * n);
    memcpy(harr, barr, sizeof(int) * n);
    qarr = malloc(sizeof(int) * n);
    memcpy(qarr, barr, sizeof(int) * n);
    //printf("排序前：\n"),print_arr(barr, n);

    t_start = clock();
    bubblesort(barr, n);
    t_end = clock();
    printf("bubblesort:\n");//print_arr(barr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    t_start = clock();
    selectsort(sarr, n);
    t_end = clock();
    printf("selectsort:\n");//print_arr(sarr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    t_start = clock();
    insertsort(iarr, n);
    t_end = clock();
    printf("insertsort:\n");//print_arr(iarr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    t_start = clock();
    shellsort(sharr, n);
    t_end = clock();
    printf("shellsort:\n");//print_arr(sharr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    t_start = clock();
    mergesort(marr, n);
    t_end = clock();
    printf("mergesort:\n");//print_arr(marr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    t_start = clock();
    heapsort(harr, n);
    t_end = clock();
    printf("heapsort:\n");//print_arr(harr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    t_start = clock();
    my_qsort(qarr, n);
    t_end = clock();
    printf("quicksort:\n");//print_arr(qarr, n);
    duration = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("duration: %lf s\n", duration);

    return(0);
}
