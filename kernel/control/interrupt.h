#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "base.h"

#define CNT_PIC 2
#define CNT_PIC_PORT 8
#define OFFSET_INTR_EXT 0x20
#ifdef ARCH_X86
	#if (OFFSET_INTR_EXT<0x20)||(OFFSET_INTR_EXT+CNT_PIC*CNT_PIC_PORT>0x100)
	#error "Incorrect configuration of PIC"
	#endif
#endif
#define CNT_INTR 256

enum intr_status
{
	INTR_OFF,
	INTR_ON
};

struct intr_frame
{
	u32 eip;
	u16 cs, :16;
	u32 eflags;
	u32 esp;
	u16 ss, :16;
};

typedef void (*intr_handler_t)(struct intr_frame);

enum intr_status intr_get_status(void);
enum intr_status intr_enable(void);
enum intr_status intr_disable(void);

void PIC_init(void);
void PIC_set_mask(u16 mask);
void intr_init(u16 cnt, struct gate_t entry);

#endif //_INTERRUPT_H