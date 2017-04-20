struct Node
{
    ElementType Element;
    Position    Next;
};

int IsEmpty(List L)
{
    return L->next == NULL;
}

int IsLast(Position P, List L)
{
    return P->Next == NULL;
}

Position Find(ElementType X,List L)
{
    Position P;

    P = L->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;

    return P;
}

Position FindPrevious(ElementType X, List L)
{
    Position P;

    P = L;
    while (P->Next != NULL && P->Next->Element != X)
        P = P->Next;

    return P;
}

void Delete(ElementType X, List L)
{
    Position P, Tmp;

    P = FindPrevious(X, L);

    if (!IsLast(P, L))
    {
        Tmp = P->Next;
        P->Next = Tmp->Next;
        free(Tmp);
    }
}

void Insert(ElementType X, List L, Position P)
{
    Position Tmp;

    Tmp = malloc(sizeof(struct Node));
    if (Tmp == NULL)
        printf("Out of space!!!");

    Tmp->Element = X;
    Tmp->Next = P->Next;
    P->Next = Tmp;
}

void DeleteList(List L)
{
    Position P, Tmp;

    P = L->Next;
    L->Next = NULL;
    while (P != NULL)
    {
        Tmp = P->Next;
        free(P);
        P = Tmp;
    }
}
