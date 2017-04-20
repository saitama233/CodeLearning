struct treenode {
    elementtype     element;
    priority_queue  left;
    priority_queue  right;
    int             npl;
};

priority_queue 
merge(priority_queue h1, priority_queue h2)
{
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;
    if (h1->element < h2->element)
        return merge1(h1, h2);
    else
        return merge1(h2, h1);
}

static priority_queue 
merge1(priority_queue h1, priority_queue h2)
{
    if (h1->left == NULL)
        h1->left = h2;
    else {
        h1->right = merge(h1->right, h2);
        if (h1->left->npl < h1->right->npl)
            swapchildren(h1);
        h1->npl = h1->right->npl + 1;
    }
    return h1;
}

priority_queue 
insert1(priority_queu h, elementtype x)
{
    priority_queue singlenode;

    singlenode = (struct treenode *)malloc(sizeof(struct treenode));
    if (singlenode == NULL)
        printf("out of space\n");
    else {
        singlenode->element = x;
        singlenode->npl = 0;
        singlenode->left = singlenode->right = NULL;
        h = merge(singlenode, h);
    }
    return h;
}

priority_queue
deletemin1(priority_queue h)
{
    priority_queue leftheap, rightheap;

    if (isempty(h)) {
        printf("priority queue is empty\n");
        return h;
    }
    leftheap = h->left;
    rightheap = h->right;
    free(h);
    return merge(leftheap, rightheap);
}
