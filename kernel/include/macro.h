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

#define JMP_INPLACE(target) {\
	asm volatile(\
	/* recover `esp` and `ebp` */\
		"leave\n\t"\
		"jmp	*%0\n\t"\
	::\
		"a"(target)\
	:\
		"esp","memory"\
	);\
}

#endif //_MACRO_H