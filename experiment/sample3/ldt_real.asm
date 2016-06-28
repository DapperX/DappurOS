;本代码由Dapper编写
;使用NASM语法
;
;本代码用于练习载入LDT和从保护模式回到实模式

%include "const.inc"
%include "macro.inc"

org 7c00h
	jmp	Main


[SECTION LDT]
ALIGN 32
LdtNull:
LdtRunTest2:	DESCRIPOR_MAKE	0, LEN_RUNTEST2 - 1, DA_32 + DA_C

LEN_LDT	equ	$ - $$

SEL_LDT_RUNTEST2	equ	LdtRunTest2 - LdtNull + SA_TIL

[SECTION GDT]
ALIGN 32
GdtNull:	DESCRIPOR_MAKE	0, 0, 0
GdtCodeS:	DESCRIPOR_MAKE	0, 0ffffh, DA_C + DA_16
GdtDataS:	DESCRIPOR_MAKE	0, 0ffffh, DA_DRW
GdtCodeG:	DESCRIPOR_MAKE	0, 0ffffh, DA_C + DA_32
GdtDataG:	DESCRIPOR_MAKE	0, 0ffffh, DA_DRW
GdtData:	DESCRIPOR_MAKE	0, SIZE_DATA - 1, DA_DR
GdtVideo:	DESCRIPOR_MAKE	ADDR_VIDEO, 0ffffh, DA_DRW
GdtRunTest1:	DESCRIPOR_MAKE	0, LEN_RUNTEST1 - 1, DA_32 + DA_C
GdtLdt:		DESCRIPOR_MAKE	0, LEN_LDT -1, DA_LDT

LEN_GDT	equ	$ - $$
GdtPtr:
	dw	LEN_GDT - 1
	dd	GdtNull

SEL_GDT_CODES	equ	GdtCodeS - GdtNull
SEL_GDT_DATAS	equ	GdtDataS - GdtNull
SEL_GDT_CODEG	equ	GdtCodeG - GdtNull
SEL_GDT_DATAG	equ	GdtDataG - GdtNull
SEL_GDT_DATA	equ	GdtData - GdtNull
SEL_GDT_VIDEO	equ	GdtVideo - GdtNull
SEL_GDT_RUNTEST1	equ	GdtRunTest1 - GdtNull
SEL_GDT_LDT		equ	GdtLdt - GdtNull


[SECTION .data]
ALIGN 32
Data:
String:	db	"In Protect Mode Now ~"
OFFICE_STRING	equ	String - $$

SIZE_DATA	equ	$ - $$


[SECTION .s16]
ALIGN 32
[BITS 16]
Main:
	;初始化地址寄存器
	mov	ax, cs
	mov	ds, ax

	mov	[RefreshCs + 3], ax

	;初始化描述符
	DESCRIPOR_SET_BASE	GdtCodeS, GotoRealMode
	DESCRIPOR_SET_BASE	GdtRunTest1, RunTest1
	DESCRIPOR_SET_BASE	LdtRunTest2, RunTest2
	DESCRIPOR_SET_BASE	GdtData, Data
	DESCRIPOR_SET_BASE	GdtLdt, LdtNull

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

	mov	ax, SEL_GDT_LDT
	lldt	ax

	jmp	dword SEL_LDT_RUNTEST2:0

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;回到实模式
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EnterReal:
	mov	ax, cs
	mov	ds, ax

	in	al, 92h
	and	al, ~10b
	out	92h, al

	mov	ax, 0b800h
	mov	ds, ax
	mov	ah, 0ch
	mov	al, 'W'
	mov	[812], ax

	hlt


[SECTION .s16_GotoRealMode]
ALIGN 32
[BITS 16]
GotoRealMode:
	cli

	mov	ax, SEL_GDT_DATAS
	mov	ds, ax
	mov	ss, ax
	mov	es,	ax
	mov	fs, ax
	mov	gs, ax

	mov	eax, cr0
	and	al,	~1
	mov	cr0, eax
RefreshCs:
	jmp	0:EnterReal


[SECTION .s32]
ALIGN 32
[BITS 32]
RunTest1:
	mov	ax, SEL_GDT_LDT
	lldt	ax

	jmp	SEL_LDT_RUNTEST2:0

LEN_RUNTEST1	equ	$ - $$

[SECTION t]
ALIGN 32
[BITS 32]
RunTest2:
	mov	ax, SEL_GDT_VIDEO
	mov	es, ax
	mov	edi, (80 * 4 + 7) * 2
	mov	ah, 0ch
	mov	al, 'Z'
	mov	[es:edi], ax

	jmp	SEL_GDT_CODES:0

LEN_RUNTEST2	equ	$ - $$