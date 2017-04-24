;生成段描述符
;参数：段基质，段界限，段属性
%macro DESCRIPOR_MAKE 3
	dw	(%2) & 0ffffh
	dw	(%1) & 0ffffh
	db	((%1) >> 16) & 0ffh
	dw	(((%2) >> 8) & 0f00h) | ((%3) & 0f0ffh)
	db	((%1) >> 24) & 0ffh
%endmacro

;修改段描述符基址
;参数：描述符地址，新的基址
;注意：本宏会修改eax的值
%macro DESCRIPOR_SET_BASE 2
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, (%2)
	mov	[(%1) + 2], ax
	shr	eax, 16
	mov	[(%1) + 4], al
	mov	[(%1) + 7], ah
%endmacro

;Gate: Selector, Offset, Dcount, Attr
%macro GATE_MAKE 4
	dw (%2 & 0FFFFh)
	dw %1
	dw (%3 & 1Fh) | ((%4 << 8) & 0FF00h)
	dw ((%2 >> 16) & 0FFFFh)
%endmacro

%macro GATE_SET_BASE 2
%endmacro