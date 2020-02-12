#include "arch/x86/io.h"
#include "arch/x86/gate.h"
#include "assert.h"
#include "kernel.h"
#include "8259A.h"
#include "interrupt.h"

struct gate_t *IDT = (struct gate_t*)(ADDR_HIGH_MEMORY+OFFSET_IDT);

// Initialize 8259A PIC
void PIC_init(void)
{
	// Disable all interruptions
	PIC_set_mask(0xffff);

	// ICW1: Initialize the master PIC, edge trigger, cascade
	io_out_b(PORT_PIC0_CTRL, ICW1_ID|ICW1_ICW4), io_delay();
	// ICW2: Map IRQ0:IRQ7 -> INT0x20:INT0x27
	io_out_b(PORT_PIC0_DATA, OFFSET_INTR_EXT), io_delay();
	// ICW3: Connect the slave at IRQ2 port
	io_out_b(PORT_PIC0_DATA, 1<<2), io_delay();
	// ICW4: x86 processor, non-buffered, fully nested, normal EOI
	io_out_b(PORT_PIC0_DATA, ICW4_8086), io_delay();

	// ICW1: Initialize the slave PIC, edge trigger, cascade
	io_out_b(PORT_PIC1_CTRL, ICW1_ID|ICW1_ICW4), io_delay();
	// ICW2: Map IRQ0:IRQ7 -> INT0x28:INT0x2f
	io_out_b(PORT_PIC1_DATA, OFFSET_INTR_EXT+CNT_PIC_PORT), io_delay();
	// ICW3: Connect to IRQ2 port on the master
	io_out_b(PORT_PIC1_DATA, 2), io_delay();
	// ICW4: the same as above
	io_out_b(PORT_PIC1_DATA, ICW4_8086), io_delay();

	// Enable all interruptions
	PIC_set_mask(0x0000);
}

void PIC_set_mask(u16 mask)
{
	io_out_b(PORT_PIC0_DATA, (u8)(mask&0xff)), io_delay();
	io_out_b(PORT_PIC1_DATA, (u8)(mask>>8)), io_delay();
}

void intr_init(u16 cnt, struct gate_t entry)
{
	KASSERT(sizeof(entry)==sizeof(u64));
	for(u16 i=0; i<cnt; ++i)
		IDT[i] = entry;
	u64 idtr = (u64)(cnt*sizeof(struct gate_t)-1)|(((u64)(u32)IDT)<<16);
	asm volatile ("lidt %0": : "m"(idtr));
}