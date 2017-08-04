	.file	"init.c"
	.section	.rodata
.LC0:
	.string	"eax: %d\n"
.LC1:
	.string	"ebx: %d\n"
	.text
	.globl	init_c_start
	.type	init_c_start, @function
init_c_start:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	subl	$12, %esp
	pushl	$65
	call	kputchar
	addl	$16, %esp
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC0
	call	kprintf
	addl	$16, %esp
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	$.LC1
	call	kprintf
	addl	$16, %esp
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	init_c_start, .-init_c_start
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
