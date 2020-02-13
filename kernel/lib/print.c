#include "print.h"

static i32 xpos=0;
static i32 ypos=0;
static i32 offset_column=0;

inline void write_video(i32 pos,char c,char color)
{
	asm volatile(
		// "movb	%1, %%gs:(%0)\n\t"
		"movb	%1, (%0)\n\t"
		// "movb	%1, %%gs:(%0)\n\t"
		"movb	%2, 1(%0)\n\t"
	:
	:
		"r"(pos+0xb8000),"r"(c),"r"(color)
	:
		"memory"
	);
}

void kcls()
{
	for(i32 i=0;i<CNT_COLUMN*CNT_ROW*2;i+=2) write_video(i,0,COLOR_WB);

	xpos=0,ypos=0,offset_column=0;
}

static void itoa(char *s,i32 base,i32 d)
{
	char *p=s;
	u32 ud=(u32)d;
	u32 divisor=10u;

	switch(base)
	{
		case 'b':divisor=2;break;
		case 'o':divisor=8;break;
		case 'p':p[0]='0',p[1]='x',p+=2,s+=2;
		case 'x':divisor=16;break;
	}
	if(base=='d' && d<0) *(p++)='-',s++,ud=(u32)-d;

	do{
		u32 r=ud%divisor;
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

void kputchar(i32 c)
{
	i32 newline=0;
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

void kprint_int(const i32 x)
{
	char s[12];
	itoa(s,'d',x);
	kputs(s);
}

void kprintf(const char *format,...)
{
	char *arg;
	i32 c;
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
				itoa(s,c, __builtin_va_arg(arg,i32));
				kputs_nonewline(s);
				break;

			case 's':
				kputs_nonewline(__builtin_va_arg(arg,char*));
				break;

			default:
				kputchar(__builtin_va_arg(arg,i32));
				break;
		}
	}
	__builtin_va_end(arg);
}