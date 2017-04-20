struct heapstruct {
    int capacity;
    int size;
    elementtype *elements;
};

priority_queue
initialize(int maxelements)
{
    priorityqueue h;

    if (maxelements < minpqsize)
        printf("priority queue size is too small\n");

    h = (struct heapstruct *)malloc(sizeof(struct heapstruct));
    if (h == NULL)
        printf("out of space\n");

    h->elements = (elementtype *)malloc((maxelements + 1) 
                                         * sizeof(elementtype));
    if (h->elements == NULL)
        printf("out of space\n");

    h->capacity = maxelements;
    h->size = 0;
    h->elements[0] = mindata;

    return h;
}

void 
insert(priority_queue h,elementtype x)
{
    int i;

    if (isfull(h)) {
        printf("priority queue is full\n");
        return;
    }

    for (i = ++h->size; h->elements[i/2] > x; i/=2)
        x->elements[i] = h->elements[i/2];
    h->elements[i] = x;
}

elementtype deletemin(priority_queue h)
{
    int i, child;
    elementtype minelement, lastelement;

    if (isempty(h)) {
        printf("Priority queue is empty\n");
        return h->elements[0];
    }
    minelement = h->elements[1];
    lastelement = h->elements[h->size--];

    for (i = 1; i * 2 <= h->size; i = child) {
        child = i * 2;
        if (child != h->size && h->elements[child + 1] < h->elements[child])
            child++;
        if (lastelement > h->elements[child])
            h->elements[i] = h->elements[child];
        else
            break;
    }
    h->elements[i] = lastelement;
    return minelement;
}
