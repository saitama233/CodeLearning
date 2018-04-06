void print_arr(int *a, int n)
{
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");
}

void swap(int *a, int *b)
{
    //a和b指向同一内存的时候会出错,a与b全会变成0
    if (a == b)
        return;
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

//冒泡排序
void bubblesort(int *a, int n)
{
    for (int i = 0; i < n-1; i++) {
        for (int j = n-1; j > i; j--)
            if (a[j] < a[j-1])
                swap(a+j, a+j-1);
    }
}

//选择排序
void selectsort(int *a, int n)
{
    int i, j, min;
    for (i = 0; i < n-1; i++) {
        min = i;
        for (j = n-1; j > i; j--) {
            if (a[j] < a[min])
                min = j;
        }
        swap(a+i, a+min);
    }
}

//插入排序
void insertsort(int *a, int n)
{
    int i, j, tmp;
    for (i = 1; i < n; i++) {
        tmp = a[i];
        for (j = i; j > 0 && a[j-1] > tmp; j--)
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

//希尔排序:不好理解，分析没看，没法写出来
void shellsort(int *a, int n)
{
    int sep, i, j;
    int tmp;
    for (sep = n/2; sep >= 1; sep /= 2) {
        for (i = sep; i < n; i++) {
            tmp = a[i];
            for (j = i; j >= sep && tmp < a[j-sep]; j -= sep) {
                    a[j] = a[j-sep];
            }
            a[j] = tmp;
        }
    }
}

//归并排序
void msort(int *a, int *tmpa, int b, int e);
void merge(int *a, int *tmpa, int b, int mid, int e);

void mergesort(int *a, int n)
{
    int *tmpa = malloc(sizeof(int) * n);
    if (tmpa) {
        msort(a, tmpa, 0, n);
        free(tmpa);
    } else
        printf("malloc tmpa error\n");
}

void msort(int *a, int *tmpa, int b, int e)
{
    if (b+1 < e) {
        int mid = (b+e)/2;
        msort(a, tmpa, b, mid);
        msort(a, tmpa, mid, e);
        merge(a, tmpa, b, mid, e);
    }
}

void merge(int *a, int *tmpa, int b, int mid, int e)
{
    int i = b, j = mid;
    int ti = 0;
    while (i < mid && j < e) {
        if (a[i] < a[j])
            tmpa[ti++] = a[i++];
        else
            tmpa[ti++] = a[j++];
    }
    while (i < mid)
        tmpa[ti++] = a[i++];
    while (j < e)
        tmpa[ti++] = a[j++];
    for (i = b, ti = 0; i < e;)
        a[i++] = tmpa[ti++];
}

//堆排序
void sink(int *a, int k, int n);
void buildheap(int *a, int n);
void heapsort(int *a, int n)
{
    buildheap(a, n);
    
    for (int i = n; i > 0; i--) {
        swap(a, a+i-1);
        sink(a, 1, i-1);
    }
}

void buildheap(int *a, int n)
{
    for (int i = n/2; i > 0; i--) {
        sink(a, i, n);
    }
}

void sink(int *a, int k, int n)
{
    int biggerchild;
    while (2*k <= n) {
        if ((2*k + 1 > n) || (a[2*k-1] > a[2*k]))
            biggerchild = 2*k;
        else
            biggerchild = 2*k+1;
        if (a[k-1] < a[biggerchild-1])
            swap(a+k-1, a+biggerchild-1);
        k = biggerchild;
    }
}

//快速排序
//这个版本的快排还没有归并排序的第二个版本快
void quicksort(int *a, int b, int e);
void my_qsort(int *a, int n)
{
    quicksort(a, 0, n);
}

void quicksort(int *a, int b, int e)
{
    int i = b,j = e-1;
    int tmp = a[i];
    while (i < j) {
        while (i < j && a[j] >= tmp)
            j--;
        if (i < j) {
            a[i++] = a[j];
        }
        
        while (i < j && a[i] <= tmp)
            i++;
        if (i < j) {
            a[j--] = a[i];
        }
    }
    a[i] = tmp;
    if (b+2 < e) {
        quicksort(a, b, i);
        quicksort(a, i+1, e);
    }
}
