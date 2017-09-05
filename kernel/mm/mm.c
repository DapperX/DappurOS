#include "base.h"
/*
static int var_global;
static int var_global_init=1;

static void mm_start()
{
	//kputs("mm has been loaded");
	__asm__ __volatile__(
		"movb %1, %%gs:(%0)\n\t"
		"movb %2, %%gs:1(%0)\n\t"
	:
	:
		"r"((80*1+64)*2),"r"((char)'M'),"r"((char)0x0C)
	:
		"memory"
	);
}*/
char *s="Success\n";

void print()
{
	__asm__ __volatile__(
		"movb %1, %%gs:(%0)\n\t"
		"movb %2, %%gs:1(%0)\n\t"
	:
	:
		"r"((80*1+64)*2),"r"((char)'M'),"r"((char)0x0C)
	:
		"memory"
	);
}

void exit_module()
{
	__asm__ __volatile__(
		"movb %1, %%gs:(%0)\n\t"
		"movb %2, %%gs:1(%0)\n\t"
		"hlt\n\t"
	:
	:
		"r"((80*1+65)*2),"r"((char)'E'),"r"((char)0x0C)
	:
		"memory"
	);
}

void init_module()
{
	print();
	exit_module();
}

int module_kernelCall(uint index, ...)
{
	init_module();
	return 0;
}

uint module_kernelCall_index=0;
int (*module_kernelCall_entry)(uint index, ...)=module_kernelCall;