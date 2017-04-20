#ifndef _hash_table_h
#define _hash_table_h

struct listnode;
typedef struct listnode *position;
struct hashtbl;
typedef struct hashtbl *hashtable;

hashtable initialize_table(int tablesize);
void destroy_table(hashtable h);
position find(hashtable h, elementtype key);
void insert(elementtype key, hashtable h);
elementtype retrieve(position p);

#endif
