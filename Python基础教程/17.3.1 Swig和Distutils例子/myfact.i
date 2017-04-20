%module myfact

%{
#define SWIG_FILE_WITH_INIT
#include "myfact.h"
%}

int fact(int n);