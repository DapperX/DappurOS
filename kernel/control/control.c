#include "control.h"
#include "assert.h"

kernelCall *const kernelCallTable=(kernelCall*)(ADDR_KERNEL_CALL_TABLE+ADDR_HIGH_MEMORY);

int_var load_module(char *moduleName)
{

}

int_var unload_module(uint moduleId)
{

}

int_var module_init()
{
	for(uint i=0;i<LEN_ARRAY(module_preload);++i)
	{
		kassert(kernelCallTable[i]);
		kernelCallTable[i](KERNEL_CALL_INIT);
	}
	for(uint i=0;i<LEN_ARRAY(module_need_load);++i)
	{
		load_module(module_need_load[i]);
	}
	return 0;
}

int_var module_exit()
{
}

int_var module_kernelCall(uint index,...)
{
	static kernelCall callList[]={
		[0]=module_init,
		[1]=module_exit,
		[8]=load_module,
		[9]=unload_module
	};
	kassert(index>=KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	kassert(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}