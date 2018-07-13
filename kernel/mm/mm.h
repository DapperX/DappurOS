#ifndef _MM_H
#define _MM_H

#include "base.h"
#include "kernel.h"

static int_var module_init();
static int_var module_exit();

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
*/
int_var module_kernelCall(u32 index,...);

u32 module_kernelCall_index=MODULE_TYPE_MM;
kernelCall module_kernelCall_entry=module_kernelCall;

#endif //_MM_H