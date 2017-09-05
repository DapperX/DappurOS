#ifndef _MEMORY_H
#define _MEMORY_H


#include "base.h"

void* kmemset(void *dest,byte val,uint size);
void* kmemcpy(void *dest,const void *src,uint size);

#endif //_MEMORY_H