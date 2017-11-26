#ifdef _{%moduleName_upperCase%}_H
#define _{%moduleName_upperCase%}_H

#include "base.h"
#include "kernel.h"

static int_var module_init();
static int_var module_exit();

/*
	kernelCall (index starts from KERNEL_CALL_SELF_DEFINED)
*/
int_var module_kernelCall(uint index,...);

uint module_kernelCall_index=MODULE_TYPE_{%moduleName_upperCase%};
kernelCall module_kernelCall_entry=module_kernelCall;

#endif //_{%moduleName_upperCase%}_H