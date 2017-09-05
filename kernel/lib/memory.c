#include "memory.h"

void* kmemset(void *dest,byte val,uint size)
{
	register byte *p=(byte*)dest;
	register byte *end=(byte*)dest+size;
	while(p<end) *p++=val;
	return dest;
}

void* kmemcpy(void *dest,const void *src,uint size)
{
	register byte *p=(byte*)dest;
	register const byte *t=(const byte*)src;
	byte *end=(byte*)dest+size;
	while(p<end) *p++=*t++;
	return dest;
}