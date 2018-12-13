#include "fs.h"
#include "macro.h"
#include "assert.h"

kernelCall *const kernelCallTable=(kernelCall*)(OFFSET_KCT+ADDR_HIGH_MEMORY);
static kernelCall callList[];

usize module_init()
{
	return 0;
}

usize module_exit()
{
	return 0;
}

static kernelCall callList[]={
	[0]=(kernelCall)module_init,
	[1]=(kernelCall)module_exit,
};

KCALL_DISPATCH usize module_kernelCall(u32 funct)
{
	KASSERT(funct<LEN_ARRAY(callList));
	KASSERT(callList[funct]);
	JMP_INPLACE(callList[funct]);
	return 0;
}
