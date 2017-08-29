#ifndef HEADER_MEMORY
#define HEADER_MEMORY


#include "base.h"

void* kmemset(const void *dest,byte val,uint size);
void* kmemcpy(const void *dest,void *src,uint size);

#endif //HEADER_MEMORY