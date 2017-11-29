#include "algorithm.h"

inline bool cmp_default_char(void *x,void *y){return *(char*)x<*(char*)y;}
inline bool cmp_default_short(void *x,void *y){return *(short*)x<*(short*)y;}
inline bool cmp_default_long(void *x,void *y){return *(long*)x<*(long*)y;}
inline bool cmp_default_int(void *x,void *y){return *(int*)x<*(int*)y;}
inline bool cmp_default_unsigned(void *x,void *y){return *(unsigned*)x<*(unsigned*)y;}
inline bool cmp_default_pointer(void *x,void *y){return *(char**)x<*(char**)y;}

void kswap(void *x,void *y,u32 size)
{
	byte *x_=(byte*)x;
	byte *y_=(byte*)y;
	byte t;
	while(size--)
	{
		t=*x_,*x_=*y_,*y_=t;
		x_++,y_++;
	}
}

void ksort(void *begin,void *end,u32 size_element,bool (*cmp)(void*,void*))
{
	for(byte* i=begin;i<(byte*)end;i+=size_element)
		for(byte *j=begin+size_element;j<(byte*)end;j+=size_element)
			if(cmp(i,j)) kswap(i,j,size_element);
}