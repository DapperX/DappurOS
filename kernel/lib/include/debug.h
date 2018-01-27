#ifndef _DEBUG_H
#define _DEBUG_H

#include "base.h"
#include "print.h"

#define HALT {asm volatile("hlt");}

#ifdef NDEBUG
#define DEBUG_WRAP(content) /\
*content*\
/
#define DEBUG_INT ((void)0)
#define DEBUG_CALL(func,...) func(__VA_ARGS__)
#else
#define DEBUG_WRAP(content) content
#define DEBUG_INT asm volatile("int $0x81");
#define DEBUG_BREAKPOINT asm volatile("xchg %%bx,%%bx"::);
#define DEBUG_CALL(func,...) debug_##func(__VA_ARGS__)
#endif


#endif //_DEBUG_H