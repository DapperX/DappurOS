#ifndef _BASE_H
#define _BASE_H

#ifdef ARCH_X86
	#include "arch/x86/basic.h"
#endif

// `a` must be an array (CANNOT be a pointer)
#define LEN_ARRAY(a) (sizeof(a)/sizeof(*(a)))

#define DISABLE(x) (void)(x);

typedef char bool;
#define true ((bool)1)
#define false ((bool)0)
typedef unsigned char byte;
// typedef unsigned short int ushort;
// typedef unsigned int uint;
// typedef long long int lli;
// typedef unsigned long long int ull;

typedef long isize;
typedef unsigned long usize;

typedef signed char i8;
typedef unsigned char u8;
typedef signed short int i16;
typedef unsigned short int u16;
typedef signed int i32;
typedef unsigned int u32;
typedef signed long long int i64;
typedef unsigned long long int u64;

typedef u8 le8;
typedef u16 le16;
typedef u32 le32;
typedef u64 le64;

#endif //_BASE_H