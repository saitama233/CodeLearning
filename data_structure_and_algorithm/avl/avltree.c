#include "avltree.h"
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

struct avlnode{
    elementtype element;
    avltree left;
    avltree right;
    int     height;
};

int height(position p)
{
    if (p == NULL)
        return 0;
    else
        return p->height;
}

void preorder_avltree(avltree tree)
{
    if (tree != NULL)
    {
        printf("%d ", tree->element);
        preorder_avltree(tree->left);
        preorder_avltree(tree->right);
    }
}

void inorder_avltree(avltree tree)
{
    if (tree != NULL)
    {
        inorder_avltree(tree->left);
        printf("%d ", tree->element);
        inorder_avltree(tree->right);
    }
}

void postorder_avltree(avltree tree)
{
    if (tree != NULL)
    {
        postorder_avltree(tree->left);
        postorder_avltree(tree->right);
        printf("%d ", tree->element);
    }
}

position findmin(avltree tree)
{
    if ( tree == NULL)
        return NULL;
    else if (tree->left == NULL)
        return tree;
    else return findmin(tree->left);
}

position findmax(avltree tree)
{
    if (tree != NULL)
        while (tree->right != NULL)
            tree = tree->right;
    return tree;
}

static position left_left_rotate(position k2)
{
    printf("left_left_rotate\n");
    position k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;

    return k1;
}

static position right_right_rotate(position k2)
{
    printf("right_right_rotate\n");
    position k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;

    return k1;
}

static position left_right_rotate(position k3)
{
    printf("left_right_rotate\n");
    k3->left = right_right_rotate(k3->left);
    return left_left_rotate(k3);
}

static position right_left_rotate(position k3)
{
    printf("right_left_rotate\n");
    k3->right = left_left_rotate(k3->right);
    return right_right_rotate(k3);
}

avltree insert_node(avltree t, elementtype x)
{
    if (t == NULL)
    {
        printf("key = %d\n", x);
        if (t = (struct avlnode *)malloc(sizeof(struct avlnode))) {
            printf("insert_malloc\n");
            t->element = x;
            t->left = t->right = NULL;
        }
        else
            printf("out of space!!!");
    }
    else if (x < t->element)
    {
        printf("x < t->element");
        t->left = insert_node(t->left, x);
        printf("after insert left\n");
        if (height(t->left) - height(t->right) == 2)
        {
            if (x < t->left->element)
                t = left_left_rotate(t);
            else
                t = left_right_rotate(t);
        }
    }
    else if (x > t->element)
    {
        printf("x > t->element");
        t->right = insert_node(t->right, x);
        if (height(t->right) - height(t->left) == 2)
        {
            if (x > t->right->element)
                t = right_right_rotate(t);
            else
                t = right_left_rotate(t);
        }
    }

    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}

position find(avltree t, elementtype x)
{
    if (t == NULL)
        return NULL;
    else if (x < t->element)
        return find(t->left, x);
    else if (x > t->element)
        return find(t->right, x);
    else
        return t;
}

avltree delete_node(avltree t, elementtype x)
{
    position node = find(t, x);
    if (t == NULL || node == NULL)
        return t;

    if (x < t->element)
    {
        t->left = delete_node(t->left, x);
        if (height(t->right) - height(t->left) == 2)
        {
            if (x < t->right->element)
                t = right_left_rotate(t);
            else
                t = right_right_rotate(t);
        }
    }
    else if (x > t->element)
    {
        t->right = delete_node(t->right, x);
        if (height(t->left) - height(t->right) == 2)
        {
            if (x < t->left->element)
                t = left_left_rotate(t);
            else
                t = left_right_rotate(t);
        }
    }
    else
    {
        if (t->left && t->right)
        {
            position min_node = findmin(t->right);
            t->element = min_node->element;
            t->right = delete_node(t->right, min_node->element);
        }
        else
            t = t->left ? t->left : t->right;
    }
    if (t)
        t->height = max(height(t->left), height(t->right)) + 1;

    return t;
}

void destroy_avltree(avltree tree)
{
    if (tree == NULL)
        return ;
    if (tree->left != NULL)
        destroy_avltree(tree->left);
    if (tree->right != NULL)
        destroy_avltree(tree->right);

    free(tree);
}
