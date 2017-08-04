;生成段描述符
;参数：段基质，段界限，段属性
%macro DESCRIPOR_MAKE 3
	dw	(%2) & 0xffff
	dw	(%1) & 0xffff
	db	((%1) >> 16) & 0x00ff
	dw	(((%2) >> 8) & 0x0f00) | ((%3) & 0xf0ff)
	db	((%1) >> 24) & 0x00ff
%endmacro

;修改段描述符基址
;参数：描述符地址，新的基址
;注意：本宏会修改eax的值
%macro DESCRIPOR_SET_BASE 2
	;xor	eax, eax
	;mov	ax, cs
	;shl	eax, 4
	;add	eax, (%2)
	mov	eax, (%2)
	mov	[(%1) + 2], ax
	shr	eax, 16
	mov	[(%1) + 4], al
	mov	[(%1) + 7], ah
%endmacro

;Gate: Selector, Offset, Dcount, Attr
%macro GATE_MAKE 4
	dw (%2 & 0xffff)
	dw %1
	dw (%3 & 0x1f) | ((%4 << 8) & 0xff00)
	dw ((%2 >> 16) & 0xffff)
%endmacro

%macro GATE_SET_BASE 2
%endmacro