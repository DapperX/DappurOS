#include "fs.h"
#include "assert.h"

kernelCall *const kernelCallTable=(kernelCall*)(OFFSET_KCT+ADDR_HIGH_MEMORY);
static kernelCall_noarg callList[];

uint_var module_init()
{
}

uint_var module_exit()
{
}

static kernelCall_noarg callList[]={
	[0]=(kernelCall_noarg)module_init,
	[1]=(kernelCall_noarg)module_exit,
};

uint_var module_kernelCall(u32 index,...)
{
	KASSERT(index>=KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	KASSERT(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}
