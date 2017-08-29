#include "stdio.h"
typedef unsigned int uint;

void x(char *s,long long a,int b)
{
	printf("%s %lld %d\n",s,a,b);
}

void y(char *s,float a,double b)
{
	printf("%s %f %lf\n",s,a,b);
}

void f(uint id,...)
{
	if(id>0)
	{
		puts("Wrong callee id");
		return;
	}
	char *arg;
	__builtin_va_start(arg,id);
	x(*(char**)(arg+0),*(long long*)(arg+4),*(int*)(arg+12));
}

void g(uint id,...)//4,4,4,(4),8
{
	if(id>0)
	{
		puts("Wrong callee id");
		return;
	}
	char *arg;
	__builtin_va_start(arg,id);
	//y(*(char**)(arg+0),*(double*)(arg+4),*(double*)(arg+12));
	char *a=__builtin_va_arg(arg,char*);
	double b=__builtin_va_arg(arg,float);
	double c=__builtin_va_arg(arg,double);
	y(a,b,c);
	__builtin_va_end(arg);
}

typedef void (*kernelCallTable_t)(uint id,...);
kernelCallTable_t kernelCallTable[2]={
	[0]=f,
	[1]=g,
};

int main()
{
	kernelCallTable[0](0,"f-x",1ll,5);
	kernelCallTable[1](0,"g-y",(float)0.1,(double)0.2);
	y("g-y",(float)0.1,(double)0.2);
	return 0;
}