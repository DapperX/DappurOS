#ifndef _KERNEL_H
#define _KERNEL_H

#define KERNEL_VERSION "0.0.1"

// for now only support at most 3.75GB address
// just be convenient to avoid special memory area such as SLIC
#define MIN_MEMORY 0xf0000000
#define OFFSET_HIGH_MEMORY 0xc0000000
#define ADDR_PAGE_DIRECTORY 0x0
#define ADDR_REVERSE_PAGE_TABLE 0x101A00
#define ADDR_KERNEL_CALL_TABLE 0x101900

#define INT_LINUX 0x80
#define INT_DEBUG 0x81
#define INT_LEGENCY 0x82
#define INT_MODULE 0xA0

typedef int_var (*kernelCall)(u32 index, ...);
typedef struct{
	u32 funcId;
	char *name;
}module_info;
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
#define MODULE_TYPE_PROCESS 2
#define MODULE_TYPE_FS 3

#define TEMPLATE_CALL_DISTRIBUTE(callList) {\
	/* 恢复堆栈至进入本函数前的状态 */\
	/* recover stack back to the status before entering this function */\
	asm volatile("leave"); \
	/* remove parameter `index` */\
	asm volatile( \
		"pop	%eax\n\t" \
		"movl	(%esp), %edx\n\t" \
		"movl	%eax, (%esp)\n\t" \
	); \
	asm volatile( \
		"shll	$2, %%edx\n\t" \
		"addl	%%edx, %%eax\n\t" \
		"jmp	*%%eax" \
	: \
	: \
		"a"(callList) \
	); \
}

#endif //_KERNEL_H