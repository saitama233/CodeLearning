#ifndef hashquad_h
#define hashquad_h

typedef unsigned int index;
typedef index position;

struct hashtbl;
typedef struct hashtbl *hashtable;

hashtable initialize_table(int tablesize);
void destroy_table(hashtable h);
position find(hashtable h, elementtype key);
void insert(hashtable h, elementtype key);
elementtype retrieve(hashtable h, position p);
hashtable rehash(hashtable h);

#endif
