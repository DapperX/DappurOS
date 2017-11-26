#include "print.h"

static int xpos=0;
static int ypos=0;
static int offset_column=0;

inline void write_video(int pos,char c,char color)
{
	asm volatile(
		"movb	%1, %%gs:(%0)\n\t"
		"movb	%2, %%gs:1(%0)\n\t"
	:
	:
		"r"(pos),"r"(c),"r"(color)
	:
		"memory"
	);
}

void kcls()
{
	for(int i=0;i<CNT_COLUMN*CNT_ROW*2;++i) write_video(i,0,COLOR_WB);

	xpos=0,ypos=0,offset_column=0;
}

static void itoa(char *s,int base,int d)
{
	char *p=s;
	uint ud=(uint)d;
	uint divisor=10;

	switch(base)
	{
		case 'b':divisor=2;break;
		case 'o':divisor=8;break;
		case 'p':p[0]='0',p[1]='x',p+=2,s+=2;
		case 'x':divisor=16;break;
	}
	if(base=='d' && d<0) *(p++)='-',s++,ud=(uint)-d;

	do{
		uint r=ud%divisor;
		*(p++)=(char)((r<10)?(r+'0'):(r+'a'-10));
	}while(ud/=divisor);

	for(char *i=s,*j=p-1;i<j;i++,j--)
	{
		register char t=*i;
		*i=*j;
		*j=t;
	}
	*p='\0';
}

void kputchar(int c)
{
	int newline=0;
	if(c=='\n'||c=='\r') newline=1;
	if(!newline)
	{
		write_video((ypos+offset_column+xpos*CNT_COLUMN)*2,(char)c,COLOR_WB);
		if(++ypos>=CNT_COLUMN/2-1) newline=1;
	}
	if(newline)
	{
		ypos=0;
		if(++xpos>=CNT_ROW) xpos=0,offset_column=CNT_COLUMN/2-offset_column;
	}
}

void kputs_nonewline(char *s)
{
	if(!s) s="(null)";
	while(*s) kputchar(*s++);
}

void kputs(char *s)
{
	kputs_nonewline(s);
	kputchar('\n');
}

void kprint_int(const int x)
{
	char s[12];
	itoa(s,'d',x);
	kputs(s);
}

void kprintf(const char *format,...)
{
	char *arg;
	int c;
	char s[64];

	__builtin_va_start(arg,format);

	while((c=*format++))
	{
		if(c!='%')
		{
			kputchar (c);
			continue;
		}

		c=*format++;
		if(!c) break;
		
		switch (c)
		{
			case 'b':
			case 'o':
			case 'd':
			case 'u':
			case 'x':
			case 'p':
				itoa(s,c, __builtin_va_arg(arg,int));
				kputs_nonewline(s);
				break;

			case 's':
				kputs_nonewline(__builtin_va_arg(arg,char*));
				break;

			default:
				kputchar(__builtin_va_arg(arg,int));
				break;
		}
	}
	__builtin_va_end(arg);
}