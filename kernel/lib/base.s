	.file	"base.c"
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
	subl	$8, %esp
	movl	12(%ebp), %edx
	movl	16(%ebp), %eax
	movb	%dl, -4(%ebp)
	movb	%al, -8(%ebp)
	movl	8(%ebp), %eax
	movzbl	-4(%ebp), %edx
	movzbl	-8(%ebp), %ecx
#APP
# 8 "base.c" 1
	movb %dl, %gs:(%eax)
	movb %cl, %gs:1(%eax)
	
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
	.globl	kcls
	.type	kcls, @function
kcls:
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
	pushl	$7
	pushl	$0
	pushl	-4(%ebp)
	call	write_video
	addl	$12, %esp
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
	.size	kcls, .-kcls
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
	movl	%eax, -28(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	$10, -20(%ebp)
	movl	12(%ebp), %eax
	cmpl	$111, %eax
	je	.L7
	cmpl	$111, %eax
	jg	.L8
	cmpl	$98, %eax
	je	.L9
	jmp	.L6
.L8:
	cmpl	$112, %eax
	je	.L10
	cmpl	$120, %eax
	je	.L11
	jmp	.L6
.L9:
	movl	$2, -20(%ebp)
	jmp	.L6
.L7:
	movl	$8, -20(%ebp)
	jmp	.L6
.L10:
	movl	-28(%ebp), %eax
	movb	$48, (%eax)
	movl	-28(%ebp), %eax
	addl	$1, %eax
	movb	$120, (%eax)
	addl	$2, -28(%ebp)
	addl	$2, 8(%ebp)
.L11:
	movl	$16, -20(%ebp)
	nop
.L6:
	cmpl	$100, 12(%ebp)
	jne	.L12
	cmpl	$0, 16(%ebp)
	jns	.L12
	movl	-28(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -28(%ebp)
	movb	$45, (%eax)
	addl	$1, 8(%ebp)
	movl	16(%ebp), %eax
	negl	%eax
	movl	%eax, -24(%ebp)
.L12:
	movl	-24(%ebp), %eax
	movl	$0, %edx
	divl	-20(%ebp)
	movl	%edx, -8(%ebp)
	movl	-28(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -28(%ebp)
	cmpl	$9, -8(%ebp)
	ja	.L13
	movl	-8(%ebp), %edx
	addl	$48, %edx
	jmp	.L14
.L13:
	movl	-8(%ebp), %edx
	addl	$87, %edx
.L14:
	movb	%dl, (%eax)
	movl	-24(%ebp), %eax
	movl	$0, %edx
	divl	-20(%ebp)
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	.L12
	movl	8(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-28(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -12(%ebp)
	jmp	.L15
.L16:
	movl	-16(%ebp), %eax
	movzbl	(%eax), %ebx
	movl	-12(%ebp), %eax
	movzbl	(%eax), %edx
	movl	-16(%ebp), %eax
	movb	%dl, (%eax)
	movl	-12(%ebp), %eax
	movb	%bl, (%eax)
	addl	$1, -16(%ebp)
	subl	$1, -12(%ebp)
.L15:
	movl	-16(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jb	.L16
	movl	-28(%ebp), %eax
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
	.globl	kputchar
	.type	kputchar, @function
kputchar:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -4(%ebp)
	cmpl	$10, 8(%ebp)
	je	.L18
	cmpl	$13, 8(%ebp)
	jne	.L19
.L18:
	movl	$1, -4(%ebp)
.L19:
	cmpl	$0, -4(%ebp)
	jne	.L20
	movl	8(%ebp), %eax
	movsbl	%al, %ecx
	movl	xpos, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	ypos, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	pushl	$7
	pushl	%ecx
	pushl	%eax
	call	write_video
	addl	$12, %esp
	movl	ypos, %eax
	addl	$1, %eax
	movl	%eax, ypos
	movl	ypos, %eax
	cmpl	$79, %eax
	jle	.L20
	movl	$1, -4(%ebp)
.L20:
	cmpl	$0, -4(%ebp)
	je	.L22
	movl	$0, ypos
	movl	xpos, %eax
	addl	$1, %eax
	movl	%eax, xpos
	movl	xpos, %eax
	cmpl	$23, %eax
	jle	.L22
	movl	$0, xpos
.L22:
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	kputchar, .-kputchar
	.section	.rodata
.LC0:
	.string	"(null)"
	.text
	.globl	kputs
	.type	kputs, @function
kputs:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	cmpl	$0, 8(%ebp)
	jne	.L25
	movl	$.LC0, 8(%ebp)
	jmp	.L25
.L26:
	movl	8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, 8(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	%eax
	call	kputchar
	addl	$4, %esp
.L25:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L26
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	kputs, .-kputs
	.globl	kprint_int
	.type	kprint_int, @function
kprint_int:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	pushl	8(%ebp)
	pushl	$100
	leal	-24(%ebp), %eax
	pushl	%eax
	call	itoa
	addl	$12, %esp
	leal	-24(%ebp), %eax
	movl	%eax, -28(%ebp)
	jmp	.L28
.L29:
	movl	-28(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	%eax
	call	kputchar
	addl	$4, %esp
	addl	$1, -28(%ebp)
.L28:
	movl	-28(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L29
	nop
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L30
	call	__stack_chk_fail
.L30:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	kprint_int, .-kprint_int
	.globl	kprintf
	.type	kprintf, @function
kprintf:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$104, %esp
	movl	8(%ebp), %eax
	movl	%eax, -92(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	leal	12(%ebp), %eax
	movl	%eax, -84(%ebp)
	jmp	.L32
.L40:
	cmpl	$37, -80(%ebp)
	je	.L33
	pushl	-80(%ebp)
	call	kputchar
	addl	$4, %esp
	jmp	.L32
.L33:
	movl	-92(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -92(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%eax, -80(%ebp)
	cmpl	$0, -80(%ebp)
	je	.L42
	movl	-80(%ebp), %eax
	subl	$98, %eax
	cmpl	$22, %eax
	ja	.L36
	movl	.L38(,%eax,4), %eax
	jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L38:
	.long	.L37
	.long	.L36
	.long	.L37
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L36
	.long	.L37
	.long	.L37
	.long	.L36
	.long	.L36
	.long	.L39
	.long	.L36
	.long	.L37
	.long	.L36
	.long	.L36
	.long	.L37
	.text
.L37:
	movl	-84(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -84(%ebp)
	movl	(%eax), %eax
	pushl	%eax
	pushl	-80(%ebp)
	leal	-76(%ebp), %eax
	pushl	%eax
	call	itoa
	addl	$12, %esp
	leal	-76(%ebp), %eax
	pushl	%eax
	call	kputs
	addl	$4, %esp
	jmp	.L32
.L39:
	movl	-84(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -84(%ebp)
	movl	(%eax), %eax
	pushl	%eax
	call	kputs
	addl	$4, %esp
	jmp	.L32
.L36:
	movl	-84(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -84(%ebp)
	movl	(%eax), %eax
	pushl	%eax
	call	kputchar
	addl	$4, %esp
	nop
.L32:
	movl	-92(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -92(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%eax, -80(%ebp)
	cmpl	$0, -80(%ebp)
	jne	.L40
	jmp	.L35
.L42:
	nop
.L35:
	nop
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L41
	call	__stack_chk_fail
.L41:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	kprintf, .-kprintf
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
