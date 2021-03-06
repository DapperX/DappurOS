#ifndef _ASSERT_H
#define _ASSERT_H

#include "base.h"
#include "macro.h"
#include "print.h"

#ifdef NDEBUG
#define KASSERT(cond,...) ((void)0)
#else //NDEBUG
#define KASSERT(cond,...) (!!(cond)?(void)0:kassert_(STR(__FILE__),__LINE__,##__VA_ARGS__));

#endif

static void kassert_(char* msg,int number,...)
{
	// DISABLE(msg);
	// DISABLE(number);
	kprintf("ASSERT %s, %u\n", msg, number);

	asm volatile("cli\t\n");
	asm volatile("hlt\t\n");
}

#endif //_ASSERT_H