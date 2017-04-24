#include "kernel.c"

int main()
{
	char a[]="\0\0\0\0abcde";
	a[0]=9;
	cc(12345,a);
}