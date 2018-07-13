#ifndef _MEMORY_H
#define _MEMORY_H


#include "base.h"

void* kmemset(void *dest,byte val,uint_var size);
void* kmemcpy(void *dest,const void *src,uint_var size);
void* kmemmove(void *dest,const void *src,uint_var size);

#endif //_MEMORY_H