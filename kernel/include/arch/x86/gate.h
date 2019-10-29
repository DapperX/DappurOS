#ifndef _ARCH_GATE_H
#define _ARCH_GATE_H ARCH_X86

#define GATE_TYPE_TSS_286 0x1
#define GATE_TYPE_LDT 0x2
#define GATE_TYPE_TSS_286_BUSY 0x3
#define GATE_TYPE_CALL_286 0x4
#define GATE_TYPE_TASK 0x5
#define GATE_TYPE_INT_286 0x6
#define GATE_TYPE_TRAP_286 0x7
#define GATE_TYPE_TSS_386 0x9
#define GATE_TYPE_TSS_386_BUSY 0xb
#define GATE_TYPE_CALL_386 0xc
#define GATE_TYPE_INT_386 0xe
#define GATE_TYPE_TRAP_386 0xf

#define DA_16 0x0
#define DA_32 0x4000

#define DA_DPL_0 0x00
#define DA_DPL_1 0x20
#define DA_DPL_2 0x40
#define DA_DPL_3 0x60

#define DA_DR 0x90
#define DA_DRW 0x92
#define DA_DRWA 0x93
#define DA_C 0x98
#define DA_CR 0x9a
#define DA_CCO 0x9c
#define DA_CCOR 0x9e

#define SA_TI_LDT 4

struct gate_t
{
	u16 offset_low;
	u16 selector;
	u8 reserve:5;
	u8 zero:3;
	u8 type:4;
	u8 is_storage:1;
	u8 DPL:2;
	u8 present:1;
	u16 offset_high;
};

inline static struct gate_t gen_gate_int(u8 type, u8 DPL, u16 selector, u32 offset)
{
	return (struct gate_t){
		.offset_low = (u16)(offset&0x0000ffff),
		.selector = selector,
		.reserve = 0,
		.zero = 0,
		.type = type,
		.is_storage = 0,
		.DPL = DPL,
		.present = 1,
		.offset_high = (u16)((offset&0xffff0000)>>16)
	};
}

#endif // _ARCH_GATE_H