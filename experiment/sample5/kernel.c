#include "multiboot.h"
#include "stdarg.h"

/*The number of columns.*/
#define COLUMNS (80)
/*The number of lines.*/
#define LINES (24)
/*The attribute of an character.*/
#define ATTRIBUTE (7)
/*The ADDR_VIDEO memory address.*/
#define ADDR_VIDEO (0xB8000)

/*Variables.*/
/*Save the X position.*/
static int xpos;
/*Save the Y position.*/
static int ypos;
/*Point to the ADDR_VIDEO memory.*/
static volatile unsigned char *video;

/*Clear the screen and initialize ADDR_VIDEO, XPOS and YPOS.*/
void cls()
{
	video=(unsigned char *)ADDR_VIDEO;

	for(int i=0;i<COLUMNS*LINES*2;++i) *(video+i)=0;

	xpos=0,ypos=0;
}

/*Convert the integer D to a string and save the string in BUF. If
	BASE is equal to 'd', interpret that D is decimal, and if BASE is
	equal to 'x', interpret that D is hexadecimal.*/
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

/*Put the character C on the screen.*/
void putchar(int c)
{
	if(c=='\n' || c=='\r')
	{
	newline:
		xpos=0;
		if(++ypos>=LINES) ypos=0;
		return;
	}

	*(video+(xpos+ypos*COLUMNS)*2)=c & 0xFF;
	*(video+(xpos+ypos*COLUMNS)*2+1)=ATTRIBUTE;

	if(++xpos>=COLUMNS) goto newline;
}

/*Format a string and print it on the screen, just like the libc
	function printf.*/
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

void cc(int eax,char* addr)
{
	cls();
	printf("eax: %d\n",eax);
	int size=*((int*)addr);
	printf("total size: %d\n",size);
	for(int i=0;i<size;++i)
	{
		printf("%d ",(int)addr[i]);
	}
}