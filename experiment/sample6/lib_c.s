	.file	"lib.c"
	.local	xpos
	.comm	xpos,4,4
	.local	ypos
	.comm	ypos,4,4
	.text
	.globl	write_video
	.type	write_video, @function
write_video:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$4, %esp
	movl	12(%ebp), %eax
	movb	%al, -4(%ebp)
	movl	8(%ebp), %eax
	movzbl	-4(%ebp), %edx
#APP
# 12 "lib.c" 1
	movb %dl, %gs:(%eax)
	
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	write_video, .-write_video
	.globl	cls
	.type	cls, @function
cls:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	.L3
.L4:
	pushl	$0
	pushl	-4(%ebp)
	call	write_video
	addl	$8, %esp
	addl	$1, -4(%ebp)
.L3:
	cmpl	$3839, -4(%ebp)
	jle	.L4
	movl	$0, xpos
	movl	$0, ypos
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	cls, .-cls
	.type	itoa, @function
itoa:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	$10, -16(%ebp)
	cmpl	$100, 12(%ebp)
	jne	.L6
	cmpl	$0, 16(%ebp)
	jns	.L6
	movl	-8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -8(%ebp)
	movb	$45, (%eax)
	addl	$1, 8(%ebp)
	movl	16(%ebp), %eax
	negl	%eax
	movl	%eax, -12(%ebp)
	jmp	.L7
.L6:
	cmpl	$120, 12(%ebp)
	jne	.L7
	movl	$16, -16(%ebp)
.L7:
	movl	-16(%ebp), %ecx
	movl	-12(%ebp), %eax
	movl	$0, %edx
	divl	%ecx
	movl	%edx, %eax
	movl	%eax, -28(%ebp)
	movl	-8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -8(%ebp)
	cmpl	$9, -28(%ebp)
	jg	.L8
	movl	-28(%ebp), %edx
	addl	$48, %edx
	jmp	.L9
.L8:
	movl	-28(%ebp), %edx
	addl	$87, %edx
.L9:
	movb	%dl, (%eax)
	movl	-16(%ebp), %ecx
	movl	-12(%ebp), %eax
	movl	$0, %edx
	divl	%ecx
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	.L7
	movl	8(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	-8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -24(%ebp)
	jmp	.L10
.L11:
	movl	-20(%ebp), %eax
	movzbl	(%eax), %ebx
	movl	-24(%ebp), %eax
	movzbl	(%eax), %edx
	movl	-20(%ebp), %eax
	movb	%dl, (%eax)
	movl	-24(%ebp), %eax
	movb	%bl, (%eax)
	addl	$1, -20(%ebp)
	subl	$1, -24(%ebp)
.L10:
	movl	-20(%ebp), %eax
	cmpl	-24(%ebp), %eax
	jb	.L11
	movl	-8(%ebp), %eax
	movb	$0, (%eax)
	nop
	addl	$32, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	itoa, .-itoa
	.globl	putchar
	.type	putchar, @function
putchar:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	cmpl	$10, 8(%ebp)
	je	.L13
	cmpl	$13, 8(%ebp)
	jne	.L14
	jmp	.L13
.L18:
	nop
.L13:
	movl	$0, xpos
	movl	ypos, %eax
	addl	$1, %eax
	movl	%eax, ypos
	movl	ypos, %eax
	cmpl	$23, %eax
	jle	.L17
	movl	$0, ypos
	jmp	.L17
.L14:
	movl	8(%ebp), %eax
	movsbl	%al, %ecx
	movl	ypos, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	xpos, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	pushl	%ecx
	pushl	%eax
	call	write_video
	addl	$8, %esp
	movl	ypos, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	xpos, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	$1, %eax
	pushl	$7
	pushl	%eax
	call	write_video
	addl	$8, %esp
	movl	xpos, %eax
	addl	$1, %eax
	movl	%eax, xpos
	movl	xpos, %eax
	cmpl	$79, %eax
	jg	.L18
	jmp	.L12
.L17:
	nop
.L12:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	putchar, .-putchar
	.globl	print_int
	.type	print_int, @function
print_int:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$48, %esp
	pushl	12(%ebp)
	pushl	$100
	leal	-34(%ebp), %eax
	pushl	%eax
	call	itoa
	addl	$12, %esp
	leal	-34(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L20
.L21:
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	%eax
	call	putchar
	addl	$4, %esp
	addl	$1, -4(%ebp)
.L20:
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L21
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	print_int, .-print_int
	.section	.rodata
.LC0:
	.string	"(null)"
	.text
	.globl	printf
	.type	printf, @function
printf:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$80, %esp
	leal	12(%ebp), %eax
	movl	%eax, -12(%ebp)
	jmp	.L23
.L32:
	cmpl	$37, -8(%ebp)
	je	.L24
	pushl	-8(%ebp)
	call	putchar
	addl	$4, %esp
	jmp	.L23
.L24:
	movl	8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, 8(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$115, %eax
	je	.L26
	cmpl	$115, %eax
	jg	.L27
	cmpl	$100, %eax
	je	.L28
	jmp	.L25
.L27:
	cmpl	$117, %eax
	je	.L28
	cmpl	$120, %eax
	jne	.L25
.L28:
	movl	-12(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -12(%ebp)
	movl	(%eax), %eax
	pushl	%eax
	pushl	-8(%ebp)
	leal	-76(%ebp), %eax
	pushl	%eax
	call	itoa
	addl	$12, %esp
	leal	-76(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L29
.L26:
#APP
# 103 "lib.c" 1
	nop
	
# 0 "" 2
#NO_APP
	movl	-12(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -12(%ebp)
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
#APP
# 105 "lib.c" 1
	nop
	
# 0 "" 2
#NO_APP
	cmpl	$0, -4(%ebp)
	jne	.L30
	movl	$.LC0, -4(%ebp)
.L29:
	jmp	.L30
.L31:
	movl	-4(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -4(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	%eax
	call	putchar
	addl	$4, %esp
.L30:
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L31
	jmp	.L23
.L25:
	movl	-12(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -12(%ebp)
	movl	(%eax), %eax
	pushl	%eax
	call	putchar
	addl	$4, %esp
	nop
.L23:
	movl	8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, 8(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%eax, -8(%ebp)
	cmpl	$0, -8(%ebp)
	jne	.L32
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	printf, .-printf
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
