#ifndef _ALGORITHM_H
#define _ALGORITHM_H 1

#include "base.h"

bool cmp_default_char(void *x,void *y);
bool cmp_default_short(void *x,void *y);
bool cmp_default_long(void *x,void *y);
bool cmp_default_int(void *x,void *y);
bool cmp_default_unsigned(void *x,void *y);
bool cmp_default_pointer(void *x,void *y);

void kswap(void *x,void *y,uint size);
void ksort(void *begin,void *end,uint size_element,bool (*cmp)(void*,void*));

#endif // _ALGORITHM_H