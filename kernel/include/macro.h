#ifndef _MACRO_H
#define _MACRO_H

// #include "kernel.h"

#define MACRO_WRAP(args) args

#define CAT_(arg1, arg2) arg1##arg2
#define CAT(arg1, arg2) CAT_(arg1, arg2)

#define STR_(s) #s
#define STR(s) STR_(s)

// count the number of parameters (most 7 parameters)
#define COUNT_PARM_(_1, _2, _3, _4, _5, _6, _7, _8, CNT, ...) CNT
#define COUNT_PARM(...) \
	MACRO_WRAP(COUNT_PARM_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define JMP_INPLACE(target) do{\
	asm volatile(\
	/* recover `esp` and `ebp` */\
		"leave\n\t"\
		"jmp	*%0\n\t"\
	::\
		"a"(target)\
	:\
		"esp","memory"\
	);\
}while(0)

#define NESTFUNC_BEGIN(func_name) do{ \
	asm volatile("jmp" STR(func_name) "_inner_exit"); \
	asm volatile( \
		".func" STR(func_name) "\n\t" \
		STR(func_name) ":\n\t" \
	); \
}while(0)

#define NESTFUNC_END(func_name) do{ \
	asm volatile(".endfunc"); \
	asm volatile(STR(func_name) "_inner_exit:"); \
}while(0)

#define NESTFUNC_GET_ADDR(func_name, addr) \
	asm volatile("movl $" STR(func_name) ", %0": "=g"(addr));

#endif //_MACRO_H