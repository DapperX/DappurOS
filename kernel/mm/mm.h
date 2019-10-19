#ifndef _MM_H
#define _MM_H

#include "base.h"
#include "kernel.h"

#define PADDR_ERR ((msize)~(usize)0)

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
*/
extern u32 module_kernelCall_index;
extern kCall_dispatch module_kernelCall_entry;

#endif //_MM_H
