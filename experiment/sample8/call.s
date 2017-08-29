	.file	"call.c"
	.section	.rodata
.LC0:
	.string	"%s %lld %d\n"
	.text
	.globl	x
	.type	x, @function
x:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -12(%ebp)
	subl	$12, %esp
	pushl	20(%ebp)
	pushl	-12(%ebp)
	pushl	-16(%ebp)
	pushl	8(%ebp)
	pushl	$.LC0
	call	printf
	addl	$32, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	x, .-x
	.section	.rodata
.LC1:
	.string	"%s %f %lf\n"
	.text
	.globl	y
	.type	y, @function
y:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	16(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	20(%ebp), %eax
	movl	%eax, -12(%ebp)
	flds	12(%ebp)
	subl	$8, %esp
	pushl	-12(%ebp)
	pushl	-16(%ebp)
	leal	-8(%esp), %esp
	fstpl	(%esp)
	pushl	8(%ebp)
	pushl	$.LC1
	call	printf
	addl	$32, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	y, .-y
	.section	.rodata
.LC3:
	.string	"Wrong callee id"
	.text
	.globl	f
	.type	f, @function
f:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	cmpl	$0, 8(%ebp)
	je	.L4
	subl	$12, %esp
	pushl	$.LC3
	call	puts
	addl	$16, %esp
	jmp	.L3
.L4:
	leal	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	addl	$12, %eax
	movl	(%eax), %ebx
	movl	-16(%ebp), %eax
	addl	$4, %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	-16(%ebp), %ecx
	movl	(%ecx), %ecx
	pushl	%ebx
	pushl	%edx
	pushl	%eax
	pushl	%ecx
	call	x
	addl	$16, %esp
.L3:
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L6
	call	__stack_chk_fail
.L6:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	f, .-f
	.globl	g
	.type	g, @function
g:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	cmpl	$0, 8(%ebp)
	je	.L8
	subl	$12, %esp
	pushl	$.LC3
	call	puts
	addl	$16, %esp
	jmp	.L7
.L8:
	leal	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	addl	$12, %eax
	fldl	(%eax)
	fnstcw	-26(%ebp)
	movzwl	-26(%ebp), %eax
	movb	$12, %ah
	movw	%ax, -28(%ebp)
	fldcw	-28(%ebp)
	fistpl	-32(%ebp)
	fldcw	-26(%ebp)
	movl	-32(%ebp), %ebx
	movl	-16(%ebp), %eax
	addl	$4, %eax
	flds	(%eax)
	fldcw	-28(%ebp)
	fistpq	-40(%ebp)
	fldcw	-26(%ebp)
	movl	-40(%ebp), %eax
	movl	-36(%ebp), %edx
	movl	-16(%ebp), %ecx
	movl	(%ecx), %ecx
	pushl	%ebx
	pushl	%edx
	pushl	%eax
	pushl	%ecx
	call	x
	addl	$16, %esp
.L7:
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L11
	call	__stack_chk_fail
.L11:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	g, .-g
	.globl	kernelCallTable
	.data
	.align 4
	.type	kernelCallTable, @object
	.size	kernelCallTable, 8
kernelCallTable:
	.long	f
	.long	g
	.section	.rodata
.LC4:
	.string	"f-x"
.LC7:
	.string	"g-y"
	.text
	.globl	main
	.type	main, @function
main:
.LFB4:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$4, %esp
	movl	kernelCallTable, %eax
	subl	$12, %esp
	pushl	$5
	pushl	$0
	pushl	$1
	pushl	$.LC4
	pushl	$0
	call	*%eax
	addl	$32, %esp
	movl	kernelCallTable+4, %eax
	subl	$8, %esp
	fldl	.LC5
	leal	-8(%esp), %esp
	fstpl	(%esp)
	fldl	.LC6
	leal	-8(%esp), %esp
	fstpl	(%esp)
	pushl	$.LC7
	pushl	$0
	call	*%eax
	addl	$32, %esp
	fldl	.LC5
	leal	-8(%esp), %esp
	fstpl	(%esp)
	flds	.LC8
	leal	-4(%esp), %esp
	fstps	(%esp)
	pushl	$.LC7
	call	y
	addl	$16, %esp
	movl	$0, %eax
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC5:
	.long	-1717986918
	.long	1070176665
	.align 8
.LC6:
	.long	-1610612736
	.long	1069128089
	.align 4
.LC8:
	.long	1036831949
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
