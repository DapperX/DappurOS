%include "const.inc"
%include "macro.inc"

extern os_start

[SECTION GDT]
ALIGN 32
GdtNull:	DESCRIPOR_MAKE	0, 0, 0
GdtVideo:	DESCRIPOR_MAKE	ADDR_VIDEO, 0ffffh, DA_DRW
GdtStack:	DESCRIPOR_MAKE	0, StackTop - 1, DA_DRWA + DA_32
GdtData:	DESCRIPOR_MAKE	0, 10000h, DA_DRWA + DA_32
GdtRunTest:	DESCRIPOR_MAKE	0, LEN_RUNTEST - 1, DA_32 + DA_C
GdtMain:	DESCRIPOR_MAKE	0, LEN_MAIN - 1, DA_32 + DA_C

LEN_GDT	equ	$ - $$
GdtPtr:
	dw	LEN_GDT - 1
	dd	GdtNull

SEL_GDT_VIDEO	equ	GdtVideo - GdtNull
SEL_GDT_STACK	equ	GdtStack - GdtNull
SEL_GDT_DATA	equ	GdtData - GdtNull
SEL_GDT_RUNTEST	equ	GdtRunTest - GdtNull
SEL_GDT_MAIN	equ GdtMain - GdtNull

[SECTION IDT]
ALIGN	32
[BITS	32]
;extern BASE_IDT
LABEL_IDT:
; 门      目标选择子,    偏移, DCount, 属性
%rep 255
	GATE_MAKE	SEL_GDT_RUNTEST, 0, 0, DA_386IGate
%endrep

IdtLen		equ	$ - $$
IdtPtr:		dw	IdtLen - 1	; 段界限
			dd	LABEL_IDT;BASE_IDT	; 基地址
; END of [SECTION IDT]

[SECTION GS]
ALIGN 32
[BITS 32]
GlobalStack:
	times 1024 db 0
StackTop	equ	$ - $$


[SECTION INT]
ALIGN 32
[BITS 32]
RunTest:
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, '!'
	mov	[gs:((80 * 0 + 76) * 2)], ax	; 屏幕第 0 行, 第 76 列。
	iret
LEN_RUNTEST	equ	$ - $$

[section init]
global _start

_start:
	mov edx,eax

	;init selector
	DESCRIPOR_SET_BASE GdtRunTest, RunTest
	;DESCRIPOR_SET_BASE GdtStack, GlobalStack
	;DESCRIPOR_SET_BASE GdtData, GlobalStack
	;DESCRIPOR_SET_BASE GdtMain, Main
	lgdt [GdtPtr]
	jmp SEL_GDT_MAIN:Main

[SECTION .text]
Main:
	mov ax, SEL_GDT_DATA
	mov ds,ax
	;init video
	mov ax, SEL_GDT_VIDEO
	mov gs, ax
	;init stack
	mov	ax, ds
	mov	ss, ax
	mov esp, GlobalStack
	add	esp, StackTop
	mov ebp, esp

	;call SEL_GDT_RUNTEST:0
	lidt [IdtPtr]

	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, '#'
	mov	[gs:((80 * 0 + 75) * 2)], ax	; 屏幕第 0 行, 第 75 列。

	push ebx
	push edx

	call os_start
	add esp, 8

	int 80

	hlt

LEN_MAIN	equ	0x100000