#include "memory.h"

void* kmemset(void *dest,byte val,usize size)
{
	register byte *p=(byte*)dest;
	register byte *end=(byte*)dest+size;
	while(p<end) *p++=val;
	return dest;
}

void* kmemcpy(void *dest,const void *src,usize size)
{
	register byte *p=(byte*)dest;
	register const byte *t=(const byte*)src;
	byte *end=(byte*)dest+size;
	while(p<end) *p++=*t++;
	return dest;
}

void* kmemmove(void *dest,const void *src,usize size)
{
	asm volatile(
		"movw	%%ds, %%ax\n\t"
		"movw	%%ax, %%es\n\t"
	:::
		"ax"
	);
	if(dest<src)
	{
		asm volatile(
			"cld\n\t"
		::
			"D"(dest),"S"(src),"c"(size)
		);
	}
	else
	{
		asm volatile(
			"std\n\t"
		::
			"D"((char*)dest+size-1),"S"((char*)src+size-1),"c"(size)
		);
	}
	asm volatile(
		"rep movsb\n\t"
		"cld\n\t"
	:::
		"memory"
	);
}