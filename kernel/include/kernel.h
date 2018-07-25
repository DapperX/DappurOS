#ifndef _KERNEL_H
#define _KERNEL_H

#include "macro.h"

#define KERNEL_VERSION "0.1.0"

// for now only support at most 3.75GB address
// just be convenient to avoid special memory area such as SLIC
#define MAX_MEMORY 0xf0000000
#define ADDR_HIGH_MEMORY 0xc0000000
#define OFFSET_PAGE_DIRECTORY 0x0
#define OFFSET_PAGE_TABLE_INIT 0x1000
#define OFFSET_PAGE_TABLE_KERNEL 0x200000
#define OFFSET_IDT 0x2000
#define OFFSET_GDT 0x2800
#define OFFSET_LDT 0x2880
#define OFFSET_KCT 0x2900
#define ADDR_STACK 0xe0000000

#define INT_LINUX 0x80
#define INT_DEBUG 0x81
#define INT_LEGENCY 0x82
#define INT_MODULE 0xA0

#define SIZE_POINTER 4

typedef uint_var (*kernelCall)(u32 index, ...);
typedef void (*kernelCall_noret)(u32 index, ...);
typedef uint_var (*kernelCall_noarg)();

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
#define KERNEL_CALL_SELF_DEFINED 8

#define MODULE_TYPE_CONTROL 0
#define MODULE_TYPE_MM 1
#define MODULE_TYPE_FS 2
#define MODULE_TYPE_PROCESS 3

#define TEMPLATE_CALL_DISTRIBUTE(callList) {\
	asm volatile( \
	/* 恢复堆栈至进入本函数前的状态 */\
	/* recover stack back to the status before entering this function */\
		"leave\n\t" \
	/* then remove parameter `index` */\
	/* pop the return address to `eax` */\
		"pop	%%eax\n\t" \
	/* save the first argument to `edx` */\
		"movl	(%%esp), %%edx\n\t" \
	/* replace it with the return address */\
		"movl	%%eax, (%%esp)\n\t" \
	/* calculate the jump address */\
		"shll	%1, %%edx\n\t" \
		"addl	%0, %%edx\n\t" \
		"jmp	*(%%edx)\n\t" \
	:: \
		"b"(callList),"i"(LOG2(SIZE_POINTER)) \
	: \
		"eax","edx","esp","memory" \
	); \
}

#endif //_KERNEL_H