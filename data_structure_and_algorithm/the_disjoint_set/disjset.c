#include "disjset.h"

void initialize(disjset s)
{
    int i;
    for (i = numsets; i > 0; i--)
        s[i] = 0;
}

void setunion(disjset s, settype root1, settype root2)
{
    s[root2] = root1;
}

void setunion_high(disjset s, settype root1, settype root2)
{
    if (s[root1] < s[root2])
        s[root1] = root2;
    else {
        if (s[root1] == s[root2])
            s[root1]--;
        s[root2] = root1;
    }
}

settype find(disjset s, elementtype x)
{
    if (s[x] <= 0)
        return x;
    else
        return find(s, s[x]);
}

settype find_route_compress(disjset s, elementtype x)
{
    if (x[x] <= 0)
        return x;
    else
        return s[x] = find(s, s[x]);
}
