#ifndef _CONTROL_H
#define _CONTROL_H

#include "base.h"
#include "kernel.h"

static usize module_init();
static usize module_exit();
static usize load_module(char *moduleName);
static usize unload_module(u32 moduleId);
static usize get_memory_total(const info_memory **p);

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
	0: load_module
	1: unload_module
*/
KCALL_DISPATCH usize module_kernelCall(u32 funct);

u32 module_kernelCall_index = MODULE_TYPE_CONTROL;
kCall_dispatch module_kernelCall_entry = module_kernelCall;

// exclude `control` module itself
u32 module_preload[] = {MODULE_TYPE_MM};
char *module_need_load[] = {"fs.mod"};

#endif //_CONTROL_H