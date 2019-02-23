#ifndef _KERNEL_H
#define _KERNEL_H

#include "macro.h"

#define KERNEL_VERSION "0.1.0"

// for now only support at most 3.75GB address
// just be convenient to avoid special memory area such as SLIC
#define MAX_MEMORY 0xf0000000
#define ADDR_HIGH_MEMORY 0xc0000000
#define OFFSET_PAGE_TABLE_KERNEL 0x0
#define OFFSET_PAGE_DIRECTORY (OFFSET_PAGE_TABLE_KERNEL+0x100000)
#define OFFSET_PAGE_TABLE_INIT (OFFSET_PAGE_DIRECTORY+0x1000)
#define OFFSET_IDT (OFFSET_PAGE_TABLE_INIT+0x1000)
#define OFFSET_GDT (OFFSET_IDT+0x800)
#define OFFSET_LDT (OFFSET_GDT+0x80)
#define OFFSET_KCT (OFFSET_LDT+0x80)
#define ADDR_STACK 0xf0000000

#define INT_LINUX 0x80
#define INT_DEBUG 0x81
#define INT_LEGENCY 0x82
#define INT_MODULE 0xA0

// #define SIZE_POINTER 4

typedef struct{
	u32 funcId;
	char *name;
}info_module;
typedef struct{
	u32 type;
	u32 cnt;
}info_header;
typedef struct{
	u64 begin;
	u64 end;
}info_memory;
typedef struct{
	u64 start;
	u64 len;
}info_ACPI;

#define KCALL_DISPATCH_PARM __attribute__((regparm(1)))
#define KCALL_DISPATCH_OPT __attribute__((optimize("-O0")))
#define KCALL_DISPATCH KCALL_DISPATCH_PARM KCALL_DISPATCH_OPT
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_2)(u32 funct);
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_3)(u32 funct, usize arg1);
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_4)(u32 funct, usize arg1, usize arg2);
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_5)(u32 funct, usize arg1, usize arg2, usize arg3);
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_6)(u32 funct, usize arg1, usize arg2, usize arg3, usize arg4);
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_7)(u32 funct, usize arg1, usize arg2, usize arg3, usize arg4, usize arg5);
typedef KCALL_DISPATCH_PARM usize (*kCall_dispatch_8)(u32 funct, usize arg1, usize arg2, usize arg3, usize arg4, usize arg5, usize arg6);
typedef kCall_dispatch_2 kCall_dispatch;
typedef usize (*kCall_dispatch_legacy)(u32 funct, ...);
typedef usize (*kernelCall)();

#define KCALL_2()
#define KCALL_3(arg) \
	,(usize)arg
#define KCALL_4(arg, ...) \
	,(usize)arg KCALL_3(__VA_ARGS__)
#define KCALL_5(arg, ...) \
	,(usize)arg KCALL_4(__VA_ARGS__)
#define KCALL_6(arg, ...) \
	,(usize)arg KCALL_5(__VA_ARGS__)
#define KCALL_7(arg, ...) \
	,(usize)arg KCALL_6(__VA_ARGS__)
#define KCALL_8(arg, ...) \
	,(usize)arg KCALL_7(__VA_ARGS__)

#define KCALL_(cntparm, index, funct, ...) \
	((CAT(kCall_dispatch_,cntparm)*)(ADDR_HIGH_MEMORY+OFFSET_KCT))[index](funct CAT(KCALL_,cntparm)(__VA_ARGS__))
#define KCALL(...) \
	KCALL_(COUNT_PARM(__VA_ARGS__), __VA_ARGS__)
#define KCALL_LEGACY(index, ...) \
	((kCall_dispatch_legacy*)(ADDR_HIGH_MEMORY+OFFSET_KCT))[index](__VA_ARGS__)

#define KCALL_ERROR ((usize)~0u)

/*
	kernelCall convention
	0: module_init
	1: module_exit
	2: module_get_information
	3-7: reverse
	8-: self-defined
*/
#define KERNEL_CALL_INIT 0
#define KERNEL_CALL_EXIT 1
#define KERNEL_CALL_GET_INFO 2
#define KERNEL_CALL_VALIDITY 3
#define KERNEL_CALL_SELF_DEFINED 8

#define MODULE_TYPE_CONTROL 0
#define MODULE_TYPE_MM 1
#define MODULE_TYPE_FS 2
#define MODULE_TYPE_PROCESS 3

#endif //_KERNEL_H