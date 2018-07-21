#include "{%moduleName%}.h"
#include "assert.h"

kernelCall *const kernelCallTable=(kernelCall*)(ADDR_KCT+OFFSET_HIGH_MEMORY);

int_var module_init()
{
}

int_var module_exit()
{
}

int_var module_kernelCall(u32 index,...)
{
	static kernelCall callList[]={
		[0]=(kernelCall)module_init,
		[1]=(kernelCall)module_exit,
	};
	KASSERT(index>=KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	KASSERT(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}