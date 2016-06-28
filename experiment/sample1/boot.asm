org	07c00h	;load the program on addr 7c00
	mov		ax,cs
	mov		ds,ax
	mov		es,ax
	call	display_string
	jmp		$	;for(;;)
display_string:
	mov		ax,init_message
	mov		bp,ax	;addr at ES:BP
	mov		cx,15	;length of string
	mov		ax,1301h	;AH=13, AL=01
	mov		bx,000ch	;BH=0	BL=0ch
	mov		dl,0
	int		10h
	ret
init_message:
	db		"Hello, Hamster!"
	times	510-($-$$)	db	0
	dw		0xaa55