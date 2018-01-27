#include "print.h"
#include "stdio.h"
#include "stdarg.h"

void write_video(i32 pos,char c,char color)
{
	printf("%c[%d;%x]",c,pos,color);
}

void kcls(){}

void kputchar(i32 c){putchar(c);}

void kputs(char *s){puts(s);}

void kprintf(const char *format,...)
{
	va_list arg;

	va_start(arg,format);
	vprintf(format,arg);
	va_end(arg);
}

void kprint_int(const i32 x){printf("%d",x);}