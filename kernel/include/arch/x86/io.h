#ifndef _ARCH_IO_H
#define _ARCH_IO_H ARCH_X86

#include "base.h"

static inline void io_out_b(u16 port, u8 value)
{
	asm volatile(
		"outb %b0, %w1"
		:
		: "a"(value), "Nd"(port)
	);
}

static inline u8 io_in_b(u16 port)
{
	u8 value;
	asm volatile(
		"inb %w1, %b0"
		: "=a"(value)
		: "Nd"(port)
	);
	return value;
}

static inline void io_delay(void)
{
	// Conventionally invoke a dummy access to 0x80 (POST codes)
	io_out_b(0x80, 0x0);
	// Alternatively, several NOP instructions may also help
	// asm volatile("nop\n\tnop\n\tnop\n\tnop\n\t");
}


#endif // _ARCH_IO_H