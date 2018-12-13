#include "{%moduleName%}.h"
#include "macro.h"
#include "assert.h"

kernelCall *const kernelCallTable=(kernelCall*)(ADDR_KCT+OFFSET_HIGH_MEMORY);

isize module_init()
{
}

isize module_exit()
{
}

isize module_kernelCall(u32 index,...)
{
	static kernelCall callList[]={
		[0]=(kernelCall)module_init,
		[1]=(kernelCall)module_exit,
	};
	KASSERT(index>=KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	KASSERT(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}