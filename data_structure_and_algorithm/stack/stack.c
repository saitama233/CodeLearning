#define EmptyTOS (-1)
#define MinStackSize (5)

struct StackRecord
{
    int Capacity;
    int TopOfStack;
    ElementType *Array;
};

Stack CreateStack(int MaxElements)
{
    Stack S;
    if (MaxElements < MinStackSize)
        printf("Stack size is too small");

    S = (StackRecord *)malloc(sizeof(struct StackRecord));
    if (S == NULL)
        printf("Out of space!!!"),return(-1);

    S->Array = (ElementType *)malloc(sizeof(ElementType) * MaxElements); 
    if (S->Array == NULL)
        printf("Out of space!!!");
    S->Capacity = MaxElements;
    return S;
}

void DisposeStack(Stack S)
{
    if (S != NULLo)
    {
        free(S->Array);
        free(S);
    }
}

int IsEmpty(Stack S)
{
    return S->TopOfStack == EmptyTOS
}

void MakeEmpty(Stack S)
{
    S->TopOfStack = EmptyTOS;
}

void Push(ElementType X, Stack S)
{
    if (IsFull(S))
        printf("Full stack\n");
    else
        S->Array[++S->TopofStack] = X;
}

ElementType Top(Stack S)
{
    if (!IsEmpty(S))
        return S->Array[S->TopofStack];
    else
    {
        printf("Empty stack\n");
        return 0;
    }
}

void Pop(Stack S)
{
    if (IsEmpty(S))
        printf("Empty stack\n");
    else
        --S->TopOfStack;
}

ElementType TopAndPop(Stack S)
{
    if (!IsEmpty(S))
        return S->Array[S->TopofStack--];
    printf("Empty Stack");
    return 0;
}
