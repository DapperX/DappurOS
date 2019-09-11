#ifndef _ARCH_X86_ATOMIC_H
#define _ARCH_X86_ATOMIC_H

void atomic_or(usize *const x, const usize y)
{
	asm volatile(
		"orl %1, %0"
	:
		"=m"(*x)
	:
		"r"(y)
	:
		"cc"
	);
}

void atomic_and(usize *const x, const usize y)
{
	asm volatile(
		"andl %1, %0"
	:
		"=m"(*x)
	:
		"r"(y)
	:
		"cc"
	);
}

void atomic_xor(usize *const x, const usize y)
{
	asm volatile(
		"xorl %1, %0"
	:
		"=m"(*x)
	:
		"r"(y)
	:
		"cc"
	);
}

#endif //_ARCH_X86_ATOMIC_H