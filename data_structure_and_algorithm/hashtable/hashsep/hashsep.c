#include "hashtable.h"

struct listnode
{
    elementtype element;
    position    next;
};

typedef position list;

struct hashtbl
{
    int     tablesize;
    list    *thelists;
};

hashtable initializee_table(int tablesize)
{
    hashtable h;
    int       i;

    if (tablesize < mintablesize) {
        printf("table size is too small\n");
        return NULL;
    }

    h = (struct hashtbl)malloc(sizeof(struct hashtbl));
    if (h == NULL)
        printf("out of space!\n");

    h->tablesize = nextprime(tablesize);
    h->thelists = (list *)(sizeof(list) * h->tablesize);
    if (h->thelists == NULL)
        printf("out of space!\n");

    for (i = 0; i < h->tablesize; i++) {
        h->thelists[i] = (struct listnode *)malloc(sizeof(struct listnode));
        if (h->thelists[i] == NULL)
            printf("out of space!\n");
        else
            h-thelists[i]->next = NULL;
    }
    return h;
}

position find(hashtable h,elementtype key)
{
    position p;
    list     l;

    l = h->thelists[hash(key, h->tablesize)];
    p = l->next;
    while (p != NULL && p->element != key)
        p = p->next;
    return p;
}

void insert(hashtable h, elementtype key)
{
    position    pos, newcell;
    list        l;

    pos = find(h, key);
    if (pos == NULL) {
        newcell = (struct listnode *)malloc(sizeof(struct listnode));
        if (newcell == NULL)
            printf("out of space!\n");
        else
        {
            l = h->thelists[hash(key, h->tablesize)];
            newcell->next = l->next;
            l->next = newcell;
            newcell->element = key;
        }

    }
}
