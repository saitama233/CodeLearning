struct binnode {
    elementtype element;
    position    leftchild;
    position    nextsibling;
};

struct collection {
    int     currentsize;
    bintree thetrees[maxtrees];
};

bintree
combine_trees(bintree t1, bintree t2)
{
    if (t1->element > t2->element)
        return combine_trees(t2, t1);
    t2->nextsibling = t1->leftchild;
    t1->leftchild = t2;

    return t1;
}

binqueue 
merge(binqueue h1, binqueue h2)
{
    bintree t1, t2, carry = NULL;
    int i, j;

    if (h1->currentsize + h2->currentsize > capacity)
        printf("merge would exceed capacity");
    h1->currentsize += h2->currentsize;
    for (i = 0, j = 1; j <= h1->currentsize; i++, j *= 2) {
        t1 = h1->thetrees[i];
        t2 = t2->thetrees[i];
        switch (!!T1 + 2 * !!T2 + 4 * !!carry) {
            case 0:
            case 1:
                break;
            case 2:
                h1->thetrees[i] = t2;
                h2->thetrees[i] = NULL;
                break;
            case 4:
                h1->thetrees[i] = carry;
                carry = NULL;
                break;
            case 3:
                carry = combine_trees(t1, t2);
                h1->thetrees[i] = h2->thetrees[i] = NULL;
                break;
            case 5:
                carry = combine_trees(t1, carry);
                h1->thetrees = NULL;
                break;
            case 6:
                carry = combine_trees(t2, carry);
                h2->thetrees = NULL;
                break;
            case 7:
                h1->thetrees[i] = carry;
                carry = combine_trees(t1,t2);
                h2->thetrees[i] = NULL;
                break;
        }
    }
}

elementtype
deletemin(binqueue h)
{
    int i, j;
    int mintree;
    binqueue deletedqueue;
    position deletedtree, oldroot;
    elementtype minitem;

    if (isempty(h)) {
        printf("empty binomial queue!\n");
        return -infinity;
    }

    minitem = infinity;
    for (i = 0; i < maxtrees; i++) {
        if (h->thetrees[i] && h->thetrees[i]->element < minitem) {
            minitem = h->thetrees[i]->element;
            mintree = i;
        }
    }
    deletedtree = h->thetrees[mintree];
    oldroot = deletedtree;
    deletedtree = deletedtree->leftchild;
    free(oldroot);

    deletedqueue = initialize();
    deletedqueue->currentsize = (1 << mintree) - 1;
    for (j = mintree - 1; j >= 0; j--) {
        deletedqueue->thetrees[j] = deletedtree;
        deletedtree = deletedtree->nextsibling;
        deletedqueue->thetrees[j]->nextsibling = NULL;
    }

    h->thetrees[mintree] = NULL;
    h->currentsize -= deletedqueue->currentsize + 1;

    merge(h, deletedqueue);
    return minitem;
}
