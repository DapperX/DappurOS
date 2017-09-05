%include "const.inc"
%include "macro.inc"


[SECTION PDE_kernel]
%rep 1024
	dd 0
%endrep

[SECTION IDT]
ALIGN	32
IdtBegin:
%rep 256
	GATE_MAKE	0, 0, 0, DA_386IGate
%endrep

IdtLen		equ	$ - $$
IdtPtr:		dw	IdtLen - 1
			dd	IdtBegin
;end of[SECTION IDT]


[SECTION GDT]
ALIGN 32
GdtBegin:
GdtNull:		DESCRIPOR_MAKE	0, 0, 0
GdtKernelCode:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_C | DA_DPL_0 | DA_G
GdtKernelData:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_DRWA | DA_DPL_0 | DA_G
GdtUserCode:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_C | DA_DPL_3 | DA_G
GdtUserData:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_DRWA | DA_DPL_3 | DA_G
GdtLdt:			DESCRIPOR_MAKE	0, LEN_LDT -1, DA_LDT
GdtVideo:		DESCRIPOR_MAKE	ADDR_VIDEO, 0x0ffff, DA_16 | DA_DRW | DA_DPL_3
%rep 9
	DESCRIPOR_MAKE	0, 0, 0
%endrep

LEN_GDT	equ	$ - $$
GdtPtr:
	dw	LEN_GDT - 1
	dd	GdtBegin

SEL_GDT_CODE_KERNEL	equ GdtKernelCode - GdtBegin
SEL_GDT_DATA_KERNEL	equ GdtKernelData - GdtBegin
SEL_GDT_CODE_USER	equ GdtUserCode - GdtBegin
SEL_GDT_DATA_USER	equ GdtUserData - GdtBegin
SEL_GDT_LDT			equ GdtLdt - GdtBegin
SEL_GDT_VIDEO		equ GdtVideo - GdtBegin
;end of[SECTION GDT]


[SECTION LDT]
ALIGN 32
LdtBegin:
LdtUserCode:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_C | DA_DPL_3
LdtUserData:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_DRWA | DA_DPL_3

LEN_LDT	equ	$ - $$

SEL_LDT_CODE_USER	equ	LdtUserCode - LdtBegin + SA_TIL
SEL_LDT_DATA_USER	equ	LdtUserData - LdtBegin + SA_TIL
;end of[SECTION LDT]


STACK_TOP_TEMP	equ 0x7c00
extern init_memory

[SECTION start]
global module_start

module_start:
	lgdt [GdtPtr]
	jmp SEL_GDT_CODE_KERNEL:init_register
;end of [SECTION start]

[SECTION .text]
init_register:
	mov edx, eax

	;init video
	mov ax, SEL_GDT_VIDEO
	mov gs, ax

	;init data seg
	mov ax, SEL_GDT_DATA_KERNEL
	mov ds, ax

	;init stack
	mov	ss, ax
	mov esp, STACK_TOP_TEMP
	mov ebp, esp

	DESCRIPOR_SET_BASE	GdtLdt, LdtBegin
	mov ax, SEL_GDT_LDT
	lldt ax

	lidt [IdtPtr]

	push ebx
	push edx
	call init_memory
	; `init_memory` should never return, or the startup has failed
	; if so, halt 
	hlt