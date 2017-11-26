#ifdef _MM_H
#define _MM_H

#include "base.h"
#include "kernel.h"

static int_var module_init();
static int_var module_exit();
static int_var alloc_page();
static int_var free_page();
/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
	0: alloc_page
	1: free_page
*/
int_var module_kernelCall(uint index,...);

uint module_kernelCall_index=MODULE_TYPE_MM;
kernelCall module_kernelCall_entry=module_kernelCall;

#endif //_MM_H