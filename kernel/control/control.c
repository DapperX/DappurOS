#include "control.h"
#include "assert.h"

kernelCall *const kernelCallTable=(kernelCall*)(ADDR_KERNEL_CALL_TABLE+OFFSET_HIGH_MEMORY);

int_var load_module(char *moduleName)
{
	return 0;
}

int_var unload_module(u32 moduleId)
{
	return 0;
}

int_var module_init()
{
	for(u32 i=0;i<LEN_ARRAY(module_preload);++i)
	{
		KASSERT(kernelCallTable[i]);
		kernelCallTable[i](KERNEL_CALL_INIT);
	}
	for(u32 i=0;i<LEN_ARRAY(module_need_load);++i)
	{
		load_module(module_need_load[i]);
	}
	return 0;
}

int_var module_exit()
{
	return 0;
}

int_var module_kernelCall(u32 index,...)
{
	static kernelCall callList[]={
		[0]=(kernelCall)module_init,
		[1]=(kernelCall)module_exit,
		[8]=(kernelCall)load_module,
		[9]=(kernelCall)unload_module
	};
	KASSERT(index>=KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	KASSERT(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);

	return 0;
}