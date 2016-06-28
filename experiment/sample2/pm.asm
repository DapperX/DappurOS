DA_32	equ	4000h
DA_DRW	equ	92h
DA_C	equ	98h

ADDR_VIDEO	equ	0b8000h

;生成段描述符
;参数：段基质，段界限，段属性
%macro DESCRIPOR_MAKE 3
	dw	%2 & 0ffffh
	dw	%1 & 0ffffh
	db	(%1 >> 16) & 0ffh
	dw	((%2 >> 8) & 0f00h) | (%3 & 0f0ffh)
	db	(%1 >> 24) & 0ffh
%endmacro

org 7c00h
	jmp	Main

[SECTION GDT]
GdtNull:	DESCRIPOR_MAKE	0, 0, 0
GdtCode32:	DESCRIPOR_MAKE	0, LEN_CODE32 - 1, DA_32 + DA_C
GdtVideo:	DESCRIPOR_MAKE	ADDR_VIDEO, 0ffffh, DA_DRW

LEN_GDT	equ	$ - GdtNull
GdtPtr:
	dw	LEN_GDT - 1
	dd	GdtNull

SEL_GDT_CODE32	equ	GdtCode32 - GdtNull
SEL_GDT_VIDEO	equ	GdtVideo - GdtNull

[SECTION .s16]
[BITS 16]
Main:
	;初始化地址寄存器
	mov	ax, cs
	mov	ds, ax

	;初始化32位代码描述符
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, Code32
	mov	[GdtCode32 + 2], ax
	shr	eax, 16
	mov	[GdtCode32 + 4], al
	mov	[GdtCode32 + 7], ah

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
	jmp	dword SEL_GDT_CODE32:0

[SECTION .s32]
[BITS 32]
Code32:
	;直接写显存
	mov	ax, SEL_GDT_VIDEO
	mov	es, ax
	mov	edi, (80 * 4 + 6) * 2
	mov	ah, 0ch
	mov	al, 'H'
	mov	[es:edi], ax

	hlt

LEN_CODE32	equ	$ - Code32