#include "{%moduleName%}.h"

kernelCall *const kernelCallTable=(kernelCall*)(ADDR_KERNEL_CALL_TABLE+ADDR_HIGH_MEMORY);

int_var module_init()
{
}

int_var module_exit()
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