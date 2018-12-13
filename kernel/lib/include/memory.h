#ifndef _MEMORY_H
#define _MEMORY_H


#include "base.h"

void* kmemset(void *dest,byte val,usize size);
void* kmemcpy(void *dest,const void *src,usize size);
void* kmemmove(void *dest,const void *src,usize size);

#endif //_MEMORY_H