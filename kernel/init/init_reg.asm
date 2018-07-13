%include "const.inc"
%include "macro.inc"


[SECTION .data]
ALIGN 8
extern LMA_GDT
extern LMA_LDT
; GDT
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

LEN_GDT	equ	$ - GdtBegin

SEL_GDT_CODE_KERNEL	equ GdtKernelCode - GdtBegin
SEL_GDT_DATA_KERNEL	equ GdtKernelData - GdtBegin
SEL_GDT_CODE_USER	equ GdtUserCode - GdtBegin
SEL_GDT_DATA_USER	equ GdtUserData - GdtBegin
SEL_GDT_LDT			equ GdtLdt - GdtBegin
SEL_GDT_VIDEO		equ GdtVideo - GdtBegin

; LDT
LdtBegin:
LdtUserCode:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_C | DA_DPL_3
LdtUserData:	DESCRIPOR_MAKE	0, 0xffffffff, DA_32 | DA_DRWA | DA_DPL_3

LEN_LDT	equ	$ - LdtBegin

SEL_LDT_CODE_USER	equ	LdtUserCode - LdtBegin + SA_TIL
SEL_LDT_DATA_USER	equ	LdtUserData - LdtBegin + SA_TIL


GdtPtr:
	dw	LEN_GDT - 1
	dd	LMA_GDT
; end of [SECTION .data]


[SECTION reserve nobits write]
ALIGN 4096
resb (12*1024)
; end of [SECTION reserve]


STACK_TOP_TEMP	equ 0x102000 ; 1M+8K


[SECTION start]
global module_start
module_start:
	; save eax
	mov edx, eax
	; initialize LDT entry
	DESCRIPOR_SET_BASE	GdtLdt, LMA_LDT
	; copy GDT to the target location
	cld
	mov edi, LMA_GDT
	mov esi, GdtBegin
	mov ecx, LEN_GDT
	rep movsb
	; copy LDT to the target location
	mov edi, LMA_LDT
	mov esi, LdtBegin
	mov ecx, LEN_LDT
	rep movsb

	lgdt [GdtPtr]
	jmp SEL_GDT_CODE_KERNEL:init_register
; end of [SECTION start]


[SECTION .text]
extern init_memory
init_register:
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

	mov ax, SEL_GDT_LDT
	lldt ax

	push ebx
	push edx
	call init_memory
	; `init_memory` should never return, or the startup has failed
	; if so, halt 
	hlt
; end of [SECTION .text]