#ifdef _CONTROL_H
#define _CONTROL_H

#include "base.h"
#include "kernel.h"

static int_var module_init();
static int_var module_exit();
static int_var load_module(char *moduleName);
static int_var unload_module(uint moduleId);

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
	0: load_module
	1: unload_module
*/
int_var module_kernelCall(uint index,...);

uint module_kernelCall_index=MODULE_TYPE_CONTROL;
kernelCall module_kernelCall_entry=module_kernelCall;

// exclude `control` module itself
uint module_preload[]={1,2,3}
char *module_need_load[]={}

#endif //_CONTROL_H