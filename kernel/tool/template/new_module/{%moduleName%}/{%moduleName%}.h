#ifndef _{%moduleName_upperCase%}_H
#define _{%moduleName_upperCase%}_H

#include "base.h"
#include "kernel.h"

static isize module_init();
static isize module_exit();

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
*/
isize module_kernelCall(u32 index,...);

u32 module_kernelCall_index=MODULE_TYPE_{%moduleName_upperCase%};
kernelCall module_kernelCall_entry=module_kernelCall;

#endif //_{%moduleName_upperCase%}_H