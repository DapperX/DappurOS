	.file	"kernel.c"
	.section	.rodata
.LC0:
	.string	"OS starts running...\n"
.LC1:
	.string	"eax: %d\n"
.LC2:
	.string	"ebx: %d\n"
.LC3:
	.string	"addr: %d\n"
	.text
	.globl	os_start
	.type	os_start, @function
os_start:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$0, -12(%ebp)
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	subl	$8, %esp
	pushl	$6666
	pushl	$.LC1
	call	printf
	addl	$16, %esp
	subl	$8, %esp
	pushl	$9999
	pushl	$.LC2
	call	printf
	addl	$16, %esp
	subl	$8, %esp
	leal	8(%ebp), %eax
	pushl	%eax
	pushl	$.LC3
	call	printf
	addl	$16, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	os_start, .-os_start
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
