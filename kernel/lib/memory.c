#include "memory.h"

void* kmemset(const void *dest,byte val,uint size)
{
	byte *p=(byte*)dest,*end=(byte*)dest+size;
	while(p<end) *p++=val;
	return dest;
}

void* kmemcpy(const void *dest,void *src,uint size)
{
	byte *p=(byte*)dest,*end=(byte*)dest+size,*t=(byte*)src;
	while(p<end) *p++=*t++;
	return dest;
}