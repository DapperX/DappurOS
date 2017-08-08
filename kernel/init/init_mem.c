#include "multiboot.h"
#include "base.h"

void init_c_start(int eax,int ebx)
{
	kputs("DappurOS initializing...");
	kprintf("eax: %d\n",eax);
	kprintf("ebx: %d\n",ebx);
}