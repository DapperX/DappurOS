#ifndef _MEMORY_H
#define _MEMORY_H


#include "base.h"

void* kmemset(void *dest,byte val,u32 size);
void* kmemcpy(void *dest,const void *src,u32 size);

#endif //_MEMORY_H