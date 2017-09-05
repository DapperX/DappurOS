#include "string.h"

int kstrcmp(char *a,char *b)
{
	while(*a || *b)
		if(*(a++)!=*(b++)) return *a<*b?-1:1;
	return 0;
}