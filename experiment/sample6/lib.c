#include "multiboot.h"
#include "stdarg.h"

#define CNT_COLUMN (80)
#define CNT_ROW (24)
#define ATTRIBUTE (7)

static int xpos;
static int ypos;

void write_video(int pos,char c)
{
	__asm__ __volatile__(
		"movb %1, %%gs:(%0)\n\t"
	:
	:
		"r"(pos),"r"(c)
	:
		"memory"
	);
}

void cls()
{
	int pos=2;
	short int c=(0xc<<8)+'G';
	__asm__ __volatile__(
		"movw %1, %%gs:(%0)\n\t"
	:
	:
		"r"(pos),"r"(c)
	:
		"memory"
	);
	for(int i=0;i<CNT_COLUMN*CNT_ROW*2;++i) write_video(i,0);

	xpos=0,ypos=0;
}

static void itoa(char*s, int base, int d)
{
	char *p=s;
	unsigned long ud=d;
	int divisor=10;

	if(base=='d' && d<0) *p++='-',s++,ud=-d;
	else if(base=='x') divisor=16;

	do{
		int r=ud%divisor;
		*p++=(r<10)?r+'0':r+'a'-10;
	}while(ud/=divisor);

	for(char *i=s,*j=p-1;i<j;i++,j--)
	{
		register char t=*i;
		*i=*j;
		*j=t;
	}
	*p='\0';
}

void putchar(int c)
{
	if(c=='\n' || c=='\r')
	{
	newline:
		xpos=0;
		if(++ypos>=CNT_ROW) ypos=0;
		return;
	}

	write_video((xpos+ypos*CNT_COLUMN)*2,c&0xFF);
	write_video((xpos+ypos*CNT_COLUMN)*2+1,ATTRIBUTE);

	if(++xpos>=CNT_COLUMN) goto newline;
}

void printf(const char *format, ...)
{
	va_list arg;
	int c;
	char buf[64];

	va_start(arg,format);
	while((c=*format++)!=0)
	{
		if(c!='%')
		{
			putchar(c);
			continue;
		}
		char *p;
		c=*format++;

		switch(c)
		{
			case 'd':
			case 'u':
			case 'x':
				itoa(buf,c,va_arg(arg,int));
				p=buf;
				goto string;
				break;

			case 's':
				p=va_arg(arg,char*);
				if(!p) p="(null)";
			string:
				while(*p) putchar(*p),p++;
				break;

			default:
				putchar(va_arg(arg,int));
				break;
		}
	}
	va_end(arg);
}