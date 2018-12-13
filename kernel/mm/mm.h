#ifndef _MM_H
#define _MM_H

#include "base.h"
#include "kernel.h"

static usize module_init();
static usize module_exit();

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
*/
KCALL_DISPATCH usize module_kernelCall(u32 funct);

u32 module_kernelCall_index = MODULE_TYPE_MM;
kCall_dispatch module_kernelCall_entry = module_kernelCall;

#endif //_MM_H
