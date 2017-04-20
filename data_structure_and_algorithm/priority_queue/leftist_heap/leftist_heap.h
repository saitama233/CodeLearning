#ifndef leftheap_h
#define leftheap_h

struct treenode;
typedef struct treenode *priority_queue;

priority_queue initialize(void);
elementtype findmin(priority_queue h);
int isempty(priority_queue h);
priority_queue merge(priority_queue h1, priority_queue h2);

#define insert(h,x) (h = insert1(h, (x)))

priority_queue insert1(priority_queu h, elementtype x);
priority_queue deletemin1(priority_queue h);

#endif
