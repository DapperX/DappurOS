;本代码由Dappur编写
;使用NASM语法
;
;本代码用于练习调用门的建立和使用

%include "const.inc"
%include "macro.inc"

org 7c00h
	jmp	Main

[SECTION GDT]
ALIGN 32
GdtNull:	DESCRIPOR_MAKE	0, 0, 0
GdtVideo:	DESCRIPOR_MAKE	ADDR_VIDEO, 0ffffh, DA_DRW
GdtStack:	DESCRIPOR_MAKE	0, StackTop, DA_DRWA + DA_32
GdtRunTest:	DESCRIPOR_MAKE	0, LEN_RUNTEST - 1, DA_32 + DA_C
GdtRunTest2:	DESCRIPOR_MAKE	0, LEN_RUNTEST2 - 1, DA_32 + DA_C
GdtGate:	GATE_MAKE	SEL_GDT_RUNTEST2, 0, 0, DA_386CGate + DA_DPL_0

LEN_GDT	equ	$ - $$
GdtPtr:
	dw	LEN_GDT - 1
	dd	GdtNull

SEL_GDT_VIDEO	equ	GdtVideo - GdtNull
SEL_GDT_STACK	equ	GdtStack - GdtNull
SEL_GDT_RUNTEST	equ	GdtRunTest - GdtNull
SEL_GDT_RUNTEST2	equ	GdtRunTest2 - GdtNull
SEL_GDT_GATE	equ	GdtGate - GdtNull


[SECTION GS]
ALIGN 32
[BITS 32]
GlobalStack:
	times 64 db 0
StackTop	equ	$ - GlobalStack -1


[SECTION .s16]
ALIGN 32
hMain:
	;初始化地址寄存器
	mov	ax, cs
	mov	ds, ax

	;初始化描述符
	DESCRIPOR_SET_BASE	GdtRunTest, RunTest
	DESCRIPOR_SET_BASE	GdtRunTest2, RunTest2
	DESCRIPOR_SET_BASE	GdtStack, GlobalStack

	;加载GDTR
	lgdt	[GdtPtr]

	;关闭中断
	cli

	;开启A20地址线
	in	al, 92h
	or	al, 10b
	out	92h, al

	;切换到保护模式
	mov	eax, cr0
	or	eax, 1b
	mov	cr0, eax

	jmp	dword SEL_GDT_RUNTEST:0

[SECTION t]
ALIGN 32
[BITS 32]
RunTest:
	mov	ax, SEL_GDT_STACK
	mov	ss, ax
	mov	esp, StackTop

	mov	ax, SEL_GDT_VIDEO
	mov	es, ax
	mov	edi, (80 * 4 + 7) * 2
	mov	ah, 0ch
	mov	al, 'D'
	mov	[es:edi], ax

	call SEL_GDT_GATE:0

	hlt

LEN_RUNTEST	equ	$ - $$

RunTest2:
	mov	ax, SEL_GDT_VIDEO
	mov	es, ax
	mov	edi, (80 * 4 + 8) * 2
	mov	ah, 0ch
	mov	al, 'F'
	mov	[es:edi], ax

	ret

LEN_RUNTEST2	equ	$ - $$