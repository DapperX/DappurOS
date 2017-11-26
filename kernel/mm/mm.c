#include "mm.h"

uint *const pageDirectory=(uint*)(OFFSET_HIGH_MEMORY+ADDR_PAGE_DIRECTORY);
uint **const reversePageTable=(uint**)(OFFSET_HIGH_MEMORY+ADDR_REVERSE_PAGE_TABLE);
kernelCall *const kernelCallTable=(kernelCall*)(OFFSET_HIGH_MEMORY+ADDR_KERNEL_CALL_TABLE);

int_var module_init()
{
	// cancel pageTable_temp
	uint *const pageDirectory=(uint*)ADDR_PAGE_DIRECTORY;
	for(register uint i=0;i<(ADDR_HIGH_MEMORY>>22);++i)
		pageDirectory[i]=0;
}

int_var module_exit()
{
}

int_var alloc_page()
{
}

int_var free_page()
{	
}

int_var module_kernelCall(uint index,...)
{
	static kernelCall callList[]={
		[0]=module_init,
		[1]=module_exit,
	};
	kassert(index>=KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	kassert(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}