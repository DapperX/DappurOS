#ifndef _CONTROL_H
#define _CONTROL_H

#include "base.h"
#include "kernel.h"

static uint_var module_init();
static uint_var module_exit();
static uint_var load_module(char *moduleName);
static uint_var unload_module(u32 moduleId);
static uint_var get_memory_total(const info_memory **p);

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
	0: load_module
	1: unload_module
*/
void module_kernelCall_init(u32 index,...);
void module_kernelCall(u32 index,...);

u32 module_kernelCall_index = MODULE_TYPE_CONTROL;
kernelCall module_kernelCall_entry = (kernelCall)module_kernelCall_init;

// exclude `control` module itself
u32 module_preload[] = {MODULE_TYPE_MM};
char *module_need_load[] = {"fs.mod"};

#endif //_CONTROL_H