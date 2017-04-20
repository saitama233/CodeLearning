enum kindofentry {legitimate, empty, deleted};

struct hashentry {
    elementtype         element;
    enum kindofentry    info;
};

typedef struct hashentry cell;

struct hashtbl
{
    int     tablesize;
    cell    *thecells;
};

hashtable initialize_table(int tablesize)
{
    hashtable h;
    int i;
    
    if (tablesize < mintablesize)
    {
        printf("table size too small!\n");
        return NULL;
    }

    h = (struct hashtbl *)malloc(sizeof(struct hashtbl));
    if (h == NULL)
        printf("out of space!\n");

    h->tablesize = nextprime(tablesize);

    h->thecells = (cell *)malloc(sizeof(cell)*h->tablesize);
    if (h->thecells == NULL)
        printf("out of space!\n");

    for (i = 0; i < h->tablesize; i++)
        h->thecells[i].info = empty;

    return h;
}

position find(hashtable h, elementtype key)
{
    position currentpos;
    int      collisionnum;

    collisionnum = 0;
    currentpos = hash(key, h->tablesize);
    while (h->thecells[currentpos].info != empty &&
           h->thecells[currentpos].element != key)
    {
        currentpos += 2 * ++collisionnum -1;
        if (currentpos >= h->tablesize)
            currentpos -= h->tablesize;
    }
    return currentpos;
}

void insert(hashtable h, elementtype key)
{
    position pos;

    pos = find(h, key);
    if (h->thecells[pos].info != legitimate)
    {
        h->thecells[pos].info = legitimate;
        h->thecells[pos].element = key;
    }
}

hashtable rehash(hashtable h)
{
    int     i, oldsize;
    cell    *oldcells;

    oldcells = h->thecells;
    oldsize = h->tablesize;

    h = initialize_table(2 * oldsize);

    for (i = 0; i < oldsize; i++)
    {
        if (oldcells[i].info == legitimate)
            insert(h, oldcells[i].element);
    }
    free(oldcells);

    return h;
}
