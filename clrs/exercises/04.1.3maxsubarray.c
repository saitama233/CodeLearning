// divide and conquer
// 用时5h，想了两个小时没什么结果，大概看了下cross那一步，
// 发现之前一直进入了个误区，不该让两个子步骤完成所有的计算，
// 跨越中点的最大序列，需要完成两个子步骤后单独计算
//
// linear
// 用时7h
// 想了6h也没做出来,在网上看的答案

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

typedef struct {
    int left;
    int right;
    int max;
} subarr;

// divide and conquer
subarr dmaxsubarray(int *arr, int len);
subarr maxsubarr(int *arr, int beg, int end);
subarr maxcross(int *arr, int beg, int mid, int end);
subarr* max(subarr *left, subarr *right, subarr *middle);
// linear
subarr lmaxsubarray(int *arr, int n);
// brute force
subarr bmaxsubarray(int *arr, int n);
void print_arr(int *arr, int n);

int main(void)
{
    int len;
    int a[] = {-1, 1};
    int *arr;
    int sign;
    int count;
    subarr dsub, lsub, bsub;


    // 比较count次,以查看算法是否有错误
    printf("please input the count:\n");
    scanf("%d", &count);
    for (int i = 0; i < count; i++) {
        // 数组大小,以便动态分配,一次一次输入太麻烦了,改成了随机生成数组大小
        // printf("please input the array size:\n");
        // scanf("%d", &size);
        
        // 不加1可能会得0,而我暂时没处理为0的情况
        len = rand() % 100 + 1;
        // printf("array size is %d\n", len);

        if (!(arr = malloc(sizeof(int) * len))) {
            perror("malloc error");
            exit(errno);
        }

        // 随机生成数组元素
        srand(time(0));
        for (int i = 0; i < len; i++) {
            sign = a[rand() % 2];
            arr[i] = rand() % 100 * sign;
        }

        // printf("The origin array is:\n");
        // print_arr(arr, len);
        
        dsub = dmaxsubarray(arr, len);
        lsub = lmaxsubarray(arr, len);
        bsub = bmaxsubarray(arr, len);
        if (dsub.max == lsub.max && lsub.max == bsub.max) {
            ;
        } else {
            printf("This is something wrong!\n");
            printf("The %d time\n", i);
            printf("The origin array is:\n");
            print_arr(arr, len);
            printf("maxarr is from %d to %d:%d\n",
                dsub.left, dsub.right, dsub.max);
            printf("maxarr is from %d to %d:%d\n",
                lsub.left, lsub.right, lsub.max);
            printf("maxarr is from %d to %d:%d\n",
                bsub.left, bsub.right, bsub.max);
            exit(0);
        }
    }
    printf("The algorithm is nearly correct!\n");

    return 0;
}

// brute force
subarr bmaxsubarray(int *arr, int n)
{
    int i, j, tmp;
    subarr sub;

    sub.max = arr[0];
    for (i = 0; i < n; i++) {
        tmp = 0;
        for (j = i; j < n; j++) {
            tmp += arr[j];
            if (tmp > sub.max) {
                sub.max = tmp;
                sub.left = i;
                sub.right = j;
            }
        }
    }
    return sub;
}

// divide and conquer
subarr dmaxsubarray(int *arr, int len)
{
    return maxsubarr(arr, 0, len);
}

subarr maxsubarr(int *arr, int beg, int end)
{
    subarr left;
    subarr right;
    subarr middle;
    int mid;

    if (beg + 1 < end) {
        mid = (beg + end) / 2;
        left = maxsubarr(arr, beg, mid);
        right = maxsubarr(arr, mid, end);
        middle = maxcross(arr, beg, mid, end);
        return *max(&left, &right, &middle);
    } else {
        middle.left = beg;
        middle.right = end-1;
        middle.max = arr[beg];
        return middle;
    }
}

subarr maxcross(int *arr, int beg, int mid, int end)
{
    subarr cross;
    int i, j;
    int tmp, maxleft, maxright;

    maxleft = arr[mid-1];
    tmp = 0;
    for (i = mid-1,cross.left = i; i >= 0; i--) {
        tmp += arr[i];
        if (tmp > maxleft) {
            maxleft = tmp;
            cross.left = i;
        }
    }

    maxright = arr[mid];
    tmp = 0;
    for (j = mid, cross.right = j; j < end; j++) {
        tmp += arr[j];
        if (tmp > maxright) {
            maxright = tmp;
            cross.right = j;
        }
    }

    cross.max = maxright + maxleft;

    return cross;
}

subarr* max(subarr *left, subarr *right, subarr *middle)
{
    subarr *max = left;
    if (max->max < right->max)
        max = right;
    if (max->max < middle->max)
        max = middle;
    return max;
}

// 仿写网上的答案
// linear
subarr lmaxsubarray(int *arr, int n)
{
    subarr tmp, sub;

    // 若全为负则返回最大值
    sub.max = arr[0];
    int sign = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0) {
            sign = 1;
            break;
        }
        else 
            if (arr[i] >= sub.max) {
                sub.max = arr[i];
                sub.left = sub.right = i;
            }
    }
    if (sign == -1) {
        return sub;
    }

    // 逐个扫描数组元素。
　　// a.若tmp.max的值为正,则和sub.max的值比较,如果大于sub.max的值,更新sub。
    // b.若tmp.max的值为负，则从tmp.left到tmp.right这一段的数字只能减少子数组的和，要丢弃，tmp.max值清零并重下一个元素重新开始计算，更新tmp.left和tmp.right的值。
    sub.max = arr[0];
    sub.left = sub.right = 0;
    tmp.max = 0;
    tmp.left = tmp.right = 0;
    while (tmp.right < n) {
        tmp.max += arr[tmp.right];
        if (tmp.max > 0) {
            if (tmp.max >= sub.max) {
                sub = tmp;
            }
            tmp.right++;
        } else {
            tmp.left = ++tmp.right;
            tmp.max = 0;
        }
    }
    return sub;
}

#if 0
// 第二次算,思路还是错了,虽然是从左面开始遍历,但没有考虑tmp.max的正负
// linear
subarr lmaxsubarray(int *arr, int n)
{
    subarr tmp, sub;

    sub.max = arr[0];
    sub.left = sub.right = 0;
    tmp = sub;
    while (tmp.right < n) {
        while (tmp.right < n) {
            tmp.right++;
            tmp.max += arr[tmp.right];
            if (tmp.max > sub.max) {
                sub.max = tmp.max;
                sub.left = tmp.left;
                sub.right = tmp.right;
                break;
            }
            if (arr[tmp.right] > sub.max) {
                break;
            }
            if (arr[tmp.right] > tmp.max) {
                break;
            }
        }
        for (; tmp.left < tmp.right;) {
            tmp.max -= arr[tmp.left++];
            if (tmp.max > sub.max) {
                sub.max = tmp.max;
                sub.left = tmp.left;
                sub.right = tmp.right;
            }
        }
    }

    return(sub);
}
#endif

#if 0
// 第一次算,思路错了,试图从两端往中间缩小
// linear
subarr lmaxsubarray(int *arr, int n)
{
    subarr sub;
    int i, j, tmp;

    for (i = 0, tmp = 0; i < n; i++) {
        tmp += arr[i];
    }
    
    sub.left = 0;
    sub.right = n-1;
    sub.max = tmp;
    for (i = 0, j = n-1;i <= j;) {
        while (i <= j) {
            tmp -= arr[i++];
            if (tmp > sub.max) {
                sub.left = i;
                sub.max = tmp;
                break;
            }
        }
        while (i <= j) {
            tmp -= arr[j--];
            if (tmp > sub.max) {
                sub.right = j;
                sub.max = tmp;
                break;
            }
        }
    }

    return sub;
}
#endif

void print_arr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    putchar('\n');
}
