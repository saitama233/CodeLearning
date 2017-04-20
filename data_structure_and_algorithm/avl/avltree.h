#ifndef _avltree_h

typedef int elementtype;

struct avlnode;
typedef struct avlnode *position;
typedef struct avlnode *avltree;

avltree make_empty(avltree t);
int height(position p);
void preorder_avltree(avltree tree);
void inorder_avltree(avltree tree);
void postorder_avltree(avltree tree);
position find(avltree t, elementtype x);
position findmin(avltree t);
position findmax(avltree t);
avltree insert_node(avltree t, elementtype x);
avltree delete_node(avltree t, elementtype x);
elementtype retrieve(position p);
void destroy_avltree(avltree tree);

#endif
