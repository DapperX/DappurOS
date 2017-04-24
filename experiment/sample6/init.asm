%include "const.inc"
%include "macro.inc"

extern os_start

[SECTION GDT]
ALIGN 32
GdtNull:	DESCRIPOR_MAKE	0, 0, 0
GdtVideo:	DESCRIPOR_MAKE	ADDR_VIDEO, 0ffffh, DA_DRW
GdtStack:	DESCRIPOR_MAKE	0, StackTop, DA_DRWA + DA_32
GdtRunTest:	DESCRIPOR_MAKE	0, LEN_RUNTEST - 1, DA_32 + DA_C

LEN_GDT	equ	$ - $$
GdtPtr:
	dw	LEN_GDT - 1
	dd	GdtNull

SEL_GDT_VIDEO	equ	GdtVideo - GdtNull
SEL_GDT_STACK	equ	GdtStack - GdtNull
SEL_GDT_RUNTEST	equ	GdtRunTest - GdtNull

[SECTION IDT]
ALIGN	32
[BITS	32]
LABEL_IDT:
; 门                        目标选择子,            偏移, DCount, 属性
%rep 255
	GATE_MAKE	SEL_GDT_RUNTEST, SpuriousHandler, 0, DA_386IGate
%endrep

IdtLen		equ	$ - LABEL_IDT
IdtPtr		dw	IdtLen - 1	; 段界限
		dd	0		; 基地址
; END of [SECTION IDT]

[SECTION GS]
ALIGN 32
[BITS 32]
GlobalStack:
	times 1024 db 0
StackTop	equ	$ - GlobalStack -1


[SECTION INT]
ALIGN 32
[BITS 32]
RunTest:
_SpuriousHandler:
SpuriousHandler	equ	_SpuriousHandler - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, '!'
	mov	[gs:((80 * 0 + 75) * 2)], ax	; 屏幕第 0 行, 第 75 列。
	iretd
LEN_RUNTEST	equ	$ - $$

[section .text]
global _start

_start:
	lgdt [GdtPtr]

	mov edx,eax

	;init selector
	DESCRIPOR_SET_BASE GdtRunTest, RunTest

	;init select reg
	mov ax,SEL_GDT_VIDEO
	mov gs,ax

	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, '#'
	mov	[gs:((80 * 0 + 75) * 2)], ax	; 屏幕第 0 行, 第 75 列。

	;init stack
	mov	ax, SEL_GDT_STACK
	mov	ss, ax
	mov	esp, StackTop

	push ebx
	push edx
	call os_start
	add esp, 8

	mov eax, 0x00550077

	;int 80

	hlt