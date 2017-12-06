#ifndef _disjset_h

typedef int disjset[numsets + 1];
typedef int settype;
typedef int elementtype;

void initialize(disjset s);
void setunion(disjset s, settype root1, settype root2);
settype find(disjset s, elementtype x);

#endif
