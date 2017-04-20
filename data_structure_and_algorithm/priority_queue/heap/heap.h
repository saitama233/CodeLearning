#ifndef binheap_h
#define binheap_h

struct heapstruct;
typedef struct heapstruct *priority_queue;

priority_queue initialize(int maxelements);
void destroy(priority_queue h);
void make_empty(priority_queue h);
void insert(priority_queue h, elementtype x);
elementtype deletemin(priority_queue h);
elementtype findmin(priority_queue h);
int isempty(priority_queue h);
int isfull(priority_queue h);

#endif
