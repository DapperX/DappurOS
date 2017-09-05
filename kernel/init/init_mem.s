	.file	"init_mem.c"
	.globl	pageDirectory
	.section	.rodata
	.align 4
	.type	pageDirectory, @object
	.size	pageDirectory, 4
pageDirectory:
	.long	1048576
	.globl	pageTable_kernel
	.align 4
	.type	pageTable_kernel, @object
	.size	pageTable_kernel, 4
pageTable_kernel:
	.long	1052672
	.globl	pageTable_temp
	.align 4
	.type	pageTable_temp, @object
	.size	pageTable_temp, 4
pageTable_temp:
	.long	204800
	.globl	len_reversePageTable
	.bss
	.align 4
	.type	len_reversePageTable, @object
	.size	len_reversePageTable, 4
len_reversePageTable:
	.zero	4
	.globl	reversePageTable
	.section	.rodata
	.align 4
	.type	reversePageTable, @object
	.size	reversePageTable, 4
reversePageTable:
	.long	2103808
	.globl	kernelCallTable
	.align 4
	.type	kernelCallTable, @object
	.size	kernelCallTable, 4
kernelCallTable:
	.long	2103552
	.globl	ACPI_addr
	.bss
	.align 4
	.type	ACPI_addr, @object
	.size	ACPI_addr, 4
ACPI_addr:
	.zero	4
	.globl	ACPI_len
	.align 4
	.type	ACPI_len, @object
	.size	ACPI_len, 4
ACPI_len:
	.zero	4
	.globl	cnt_module
	.align 4
	.type	cnt_module, @object
	.size	cnt_module, 4
cnt_module:
	.zero	4
	.globl	module
	.align 4
	.type	module, @object
	.size	module, 20
module:
	.zero	20
	.comm	size_reserveMemory,4,4
	.text
	.type	align8, @function
align8:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	addl	$7, %eax
	andl	$-8, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	align8, .-align8
	.type	align12, @function
align12:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	addl	$4095, %eax
	andl	$-4096, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	align12, .-align12
	.type	align, @function
align:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	12(%ebp), %eax
	movl	$1, %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	leal	-1(%eax), %ebx
	movl	8(%ebp), %eax
	leal	(%ebx,%eax), %edx
	movl	%ebx, %eax
	notl	%eax
	andl	%edx, %eax
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	align, .-align
	.section	.rodata
.LC0:
	.string	"Modules: %s\n"
.LC1:
	.string	"%d %d\n"
	.text
	.globl	handle_tag_module
	.type	handle_tag_module, @function
handle_tag_module:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	8(%ebp), %eax
	addl	$16, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC0
	call	kprintf
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	12(%eax), %edx
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	subl	$4, %esp
	pushl	%edx
	pushl	%eax
	pushl	$.LC1
	call	kprintf
	addl	$16, %esp
	movl	cnt_module, %eax
	cmpl	$4, %eax
	ja	.L9
	movl	cnt_module, %eax
	leal	1(%eax), %edx
	movl	%edx, cnt_module
	movl	8(%ebp), %edx
	movl	%edx, module(,%eax,4)
.L9:
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	handle_tag_module, .-handle_tag_module
	.section	.rodata
	.align 4
.LC2:
	.string	"Warning: mmap version is incompatible"
.LC3:
	.string	"len_reversePageTable:%d\n"
	.text
	.globl	handle_tag_mmap
	.type	handle_tag_mmap, @function
handle_tag_mmap:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	je	.L11
	subl	$12, %esp
	pushl	$.LC2
	call	kputs
	addl	$16, %esp
.L11:
	movl	8(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -24(%ebp)
	movl	8(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -28(%ebp)
	jmp	.L12
.L21:
	movl	-28(%ebp), %eax
	movl	16(%eax), %eax
	cmpl	$3, %eax
	jne	.L13
	movl	-28(%ebp), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, ACPI_addr
	movl	-28(%ebp), %eax
	movl	12(%eax), %edx
	movl	8(%eax), %eax
	movl	%eax, ACPI_len
.L13:
	movl	-28(%ebp), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	cmpl	$0, %edx
	jb	.L14
	cmpl	$0, %edx
	ja	.L22
	cmpl	$-268435457, %eax
	ja	.L22
.L14:
	movl	-28(%ebp), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	shrl	$12, %eax
	movl	%eax, -20(%ebp)
	movl	-28(%ebp), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, %ecx
	movl	-28(%ebp), %eax
	movl	12(%eax), %edx
	movl	8(%eax), %eax
	addl	%ecx, %eax
	shrl	$12, %eax
	movl	%eax, -16(%ebp)
	movl	-28(%ebp), %eax
	movl	16(%eax), %eax
	cmpl	$1, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	movl	len_reversePageTable, %ebx
	jmp	.L17
.L18:
	movl	$2103808, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	$1, (%eax)
	addl	$1, %ebx
.L17:
	cmpl	-20(%ebp), %ebx
	jb	.L18
	movl	-20(%ebp), %ebx
	jmp	.L19
.L20:
	movl	$2103808, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	%ebx, %edx
	sall	$12, %edx
	orl	-12(%ebp), %edx
	movl	%edx, (%eax)
	addl	$1, %ebx
.L19:
	cmpl	-16(%ebp), %ebx
	jb	.L20
	movl	len_reversePageTable, %eax
	cmpl	%eax, -16(%ebp)
	jbe	.L16
	movl	-16(%ebp), %eax
	movl	%eax, len_reversePageTable
	jmp	.L16
.L22:
	nop
.L16:
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	addl	%eax, -28(%ebp)
.L12:
	movl	-28(%ebp), %eax
	cmpl	-24(%ebp), %eax
	jb	.L21
	movl	len_reversePageTable, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC3
	call	kprintf
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	handle_tag_mmap, .-handle_tag_mmap
	.globl	get_page_free
	.type	get_page_free, @function
get_page_free:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	size_reserveMemory, %eax
	movl	%eax, -8(%ebp)
	movl	$0, -12(%ebp)
	jmp	.L24
.L25:
	movl	$204800, %edx
	movl	size_reserveMemory, %eax
	shrl	$12, %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	%eax, -4(%ebp)
	movl	$1048576, %edx
	movl	size_reserveMemory, %eax
	shrl	$22, %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	-4(%ebp), %edx
	orl	$3, %edx
	movl	%edx, (%eax)
	movl	size_reserveMemory, %eax
	orl	12(%ebp), %eax
	orl	$1, %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	movl	%edx, (%eax)
	movl	$2103808, %edx
	movl	size_reserveMemory, %eax
	shrl	$12, %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	-4(%ebp), %edx
	orl	$1, %edx
	movl	%edx, (%eax)
	movl	size_reserveMemory, %eax
	addl	$4096, %eax
	movl	%eax, size_reserveMemory
	addl	$1, -12(%ebp)
.L24:
	movl	-12(%ebp), %eax
	cmpl	8(%ebp), %eax
	jb	.L25
	movl	-8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	get_page_free, .-get_page_free
	.globl	elf_init_VMA
	.type	elf_init_VMA, @function
elf_init_VMA:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$68, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	%eax, -60(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	$0, -28(%ebp)
	movl	$0, -24(%ebp)
	movl	-60(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	-44(%ebp), %eax
	movl	32(%eax), %edx
	movl	-60(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -40(%ebp)
	movl	$0, -56(%ebp)
	jmp	.L28
.L31:
	movl	-56(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	8(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	.L38
	movl	-56(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	8(%eax), %eax
	movl	%eax, -36(%ebp)
	movl	-56(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	leal	(%edx,%eax), %ebx
	movl	-56(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	32(%eax), %edx
	movl	-36(%ebp), %eax
	movl	-28(%ebp,%eax,4), %eax
	pushl	%edx
	pushl	%eax
	call	align
	addl	$8, %esp
	movl	%eax, 12(%ebx)
	movl	-56(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %ecx
	movl	-56(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	20(%eax), %eax
	leal	(%ecx,%eax), %edx
	movl	-36(%ebp), %eax
	movl	%edx, -28(%ebp,%eax,4)
	jmp	.L30
.L38:
	nop
.L30:
	addl	$1, -56(%ebp)
.L28:
	movl	-44(%ebp), %eax
	movzwl	48(%eax), %eax
	movzwl	%ax, %eax
	cmpl	-56(%ebp), %eax
	ja	.L31
	movl	$0, -52(%ebp)
	jmp	.L32
.L33:
	movl	-52(%ebp), %eax
	movl	-28(%ebp,%eax,4), %eax
	pushl	%eax
	call	align12
	addl	$4, %esp
	shrl	$12, %eax
	movl	%eax, -32(%ebp)
	pushl	-52(%ebp)
	pushl	-32(%ebp)
	call	get_page_free
	addl	$8, %esp
	movl	%eax, %edx
	movl	-52(%ebp), %eax
	movl	%edx, -20(%ebp,%eax,4)
	addl	$1, -52(%ebp)
.L32:
	cmpl	$1, -52(%ebp)
	jbe	.L33
	movl	$0, -48(%ebp)
	jmp	.L34
.L36:
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	8(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	.L35
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	leal	(%edx,%eax), %ecx
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %ebx
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	8(%eax), %eax
	movl	-20(%ebp,%eax,4), %eax
	addl	%ebx, %eax
	movl	%eax, 12(%ecx)
.L35:
	addl	$1, -48(%ebp)
.L34:
	movl	-44(%ebp), %eax
	movzwl	48(%eax), %eax
	movzwl	%ax, %eax
	cmpl	-48(%ebp), %eax
	ja	.L36
	nop
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L37
	call	__stack_chk_fail
.L37:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	elf_init_VMA, .-elf_init_VMA
	.section	.rodata
.LC4:
	.string	"e_shentsize: %d\n"
.LC5:
	.string	"e_shnum: %d\n"
.LC6:
	.string	"#%s symtab:%d dist:%d\n"
.LC7:
	.string	"@symtab[%d]: %s\n"
.LC8:
	.string	"unsupported symbol section"
.LC9:
	.string	"R_386_32"
.LC10:
	.string	"R_386_PC32"
.LC11:
	.string	"buffer: %p\n"
	.text
	.globl	elf_relocate
	.type	elf_relocate, @function
elf_relocate:
.LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$68, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	%eax, -52(%ebp)
	movl	-52(%ebp), %eax
	movzwl	46(%eax), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC4
	call	kprintf
	addl	$16, %esp
	movl	-52(%ebp), %eax
	movzwl	48(%eax), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC5
	call	kprintf
	addl	$16, %esp
	movl	-52(%ebp), %eax
	movl	32(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -48(%ebp)
	movl	-52(%ebp), %eax
	movzwl	50(%eax), %eax
	movzwl	%ax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -44(%ebp)
	movl	$0, -64(%ebp)
	jmp	.L40
.L51:
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	4(%eax), %eax
	cmpl	$9, %eax
	jne	.L52
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	28(%eax), %ebx
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	24(%eax), %ecx
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	-44(%ebp), %eax
	addl	%edx, %eax
	pushl	%ebx
	pushl	%ecx
	pushl	%eax
	pushl	$.LC6
	call	kprintf
	addl	$16, %esp
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	20(%eax), %ecx
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	36(%eax), %ebx
	movl	%ecx, %eax
	movl	$0, %edx
	divl	%ebx
	movl	%eax, -40(%ebp)
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -36(%ebp)
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	24(%eax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -32(%ebp)
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	24(%eax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	24(%eax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -28(%ebp)
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	28(%eax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -24(%ebp)
	movl	-64(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	28(%eax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	$0, -60(%ebp)
	jmp	.L43
.L50:
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-36(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -16(%ebp)
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-36(%ebp), %eax
	addl	%edx, %eax
	movl	4(%eax), %eax
	shrl	$8, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-32(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	(%eax), %edx
	movl	-28(%ebp), %eax
	addl	%eax, %edx
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %ecx
	movl	-36(%ebp), %eax
	addl	%ecx, %eax
	movl	4(%eax), %eax
	shrl	$8, %eax
	subl	$4, %esp
	pushl	%edx
	pushl	%eax
	pushl	$.LC7
	call	kprintf
	addl	$16, %esp
	movl	$0, -56(%ebp)
	movl	-12(%ebp), %eax
	movzwl	14(%eax), %eax
	movzwl	%ax, %eax
	testl	%eax, %eax
	je	.L53
	testl	%eax, %eax
	js	.L44
	subl	$65521, %eax
	cmpl	$1, %eax
	jbe	.L53
.L44:
	movl	-12(%ebp), %eax
	movzwl	14(%eax), %eax
	movzwl	%ax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %eax
	movl	%eax, -56(%ebp)
	jmp	.L46
.L53:
	nop
.L46:
	cmpl	$0, -56(%ebp)
	jne	.L47
	subl	$12, %esp
	pushl	$.LC8
	call	kputs
	addl	$16, %esp
	jmp	.L48
.L47:
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-36(%ebp), %eax
	addl	%edx, %eax
	movl	4(%eax), %eax
	movzbl	%al, %eax
	cmpl	$1, %eax
	jne	.L49
	subl	$12, %esp
	pushl	$.LC9
	call	kputs
	addl	$16, %esp
	movl	-24(%ebp), %edx
	movl	-16(%ebp), %eax
	addl	%eax, %edx
	movl	-24(%ebp), %ecx
	movl	-16(%ebp), %eax
	addl	%ecx, %eax
	movl	(%eax), %eax
	movl	%eax, %ebx
	movl	-12(%ebp), %eax
	movl	4(%eax), %ecx
	movl	-56(%ebp), %eax
	addl	%ecx, %eax
	addl	%ebx, %eax
	subl	$1073741824, %eax
	movl	%eax, (%edx)
	jmp	.L48
.L49:
	subl	$12, %esp
	pushl	$.LC10
	call	kputs
	addl	$16, %esp
	movl	-24(%ebp), %edx
	movl	-16(%ebp), %eax
	addl	%eax, %edx
	movl	-24(%ebp), %ecx
	movl	-16(%ebp), %eax
	addl	%ecx, %eax
	movl	(%eax), %ecx
	movl	-56(%ebp), %ebx
	movl	-20(%ebp), %eax
	subl	%eax, %ebx
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	subl	-16(%ebp), %eax
	addl	%ebx, %eax
	addl	%ecx, %eax
	movl	%eax, (%edx)
.L48:
	addl	$1, -60(%ebp)
.L43:
	movl	-60(%ebp), %eax
	cmpl	-40(%ebp), %eax
	jb	.L50
	jmp	.L42
.L52:
	nop
.L42:
	addl	$1, -64(%ebp)
.L40:
	movl	-52(%ebp), %eax
	movzwl	48(%eax), %eax
	movzwl	%ax, %eax
	cmpl	-64(%ebp), %eax
	ja	.L51
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC11
	call	kprintf
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	elf_relocate, .-elf_relocate
	.globl	load_elf
	.type	load_elf, @function
load_elf:
.LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	32(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	movl	$0, -20(%ebp)
	jmp	.L55
.L58:
	movl	-20(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	je	.L59
	movl	-20(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	20(%eax), %ecx
	movl	-20(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	leal	(%edx,%eax), %ebx
	movl	-20(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %eax
	subl	$4, %esp
	pushl	%ecx
	pushl	%ebx
	pushl	%eax
	call	kmemcpy
	addl	$16, %esp
	jmp	.L57
.L59:
	nop
.L57:
	addl	$1, -20(%ebp)
.L55:
	movl	-16(%ebp), %eax
	movzwl	48(%eax), %eax
	movzwl	%ax, %eax
	cmpl	-20(%ebp), %eax
	ja	.L58
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	load_elf, .-load_elf
	.section	.rodata
.LC12:
	.string	"module_kernelCall_index"
.LC13:
	.string	"module_kernelCall_entry"
.LC14:
	.string	"KCT[0]: %p\n"
	.text
	.globl	set_kernelCall
	.type	set_kernelCall, @function
set_kernelCall:
.LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$52, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	movl	32(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -28(%ebp)
	movl	$0, -48(%ebp)
	jmp	.L61
.L68:
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	4(%eax), %eax
	cmpl	$2, %eax
	jne	.L69
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	20(%eax), %ecx
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	36(%eax), %ebx
	movl	%ecx, %eax
	movl	$0, %edx
	divl	%ebx
	movl	%eax, -24(%ebp)
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -20(%ebp)
	movl	-48(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	24(%eax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	16(%eax), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -16(%ebp)
	movl	$0, -44(%ebp)
	movl	$0, -40(%ebp)
	movl	$0, -36(%ebp)
	jmp	.L64
.L67:
	movl	-36(%ebp), %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-20(%ebp), %eax
	addl	%edx, %eax
	movzwl	14(%eax), %eax
	movzwl	%ax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	12(%eax), %edx
	movl	-36(%ebp), %eax
	sall	$4, %eax
	movl	%eax, %ecx
	movl	-20(%ebp), %eax
	addl	%ecx, %eax
	movl	4(%eax), %eax
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	movl	-36(%ebp), %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-20(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	subl	$8, %esp
	pushl	$.LC12
	pushl	%eax
	call	kstrcmp
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L65
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -44(%ebp)
.L65:
	movl	-36(%ebp), %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-20(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	subl	$8, %esp
	pushl	$.LC13
	pushl	%eax
	call	kstrcmp
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L66
	movl	-12(%ebp), %eax
	subl	$1073741824, %eax
	movl	%eax, -40(%ebp)
.L66:
	addl	$1, -36(%ebp)
.L64:
	movl	-36(%ebp), %eax
	cmpl	-24(%ebp), %eax
	jb	.L67
	cmpl	$0, -40(%ebp)
	je	.L63
	movl	$2103552, %edx
	movl	-44(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	movl	-40(%ebp), %eax
	movl	%eax, (%edx)
	jmp	.L63
.L69:
	nop
.L63:
	addl	$1, -48(%ebp)
.L61:
	movl	-32(%ebp), %eax
	movzwl	48(%eax), %eax
	movzwl	%ax, %eax
	cmpl	-48(%ebp), %eax
	ja	.L68
	movl	$2103552, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC14
	call	kprintf
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	set_kernelCall, .-set_kernelCall
	.globl	load_module
	.type	load_module, @function
load_module:
.LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$module+20, %eax
	pushl	$cmp_default_pointer
	pushl	$4
	pushl	%eax
	pushl	$module
	call	ksort
	addl	$16, %esp
	movl	$1052672, -24(%ebp)
	movl	$0, -20(%ebp)
	jmp	.L71
.L72:
	movl	-20(%ebp), %eax
	movl	module(,%eax,4), %eax
	movl	4(%eax), %eax
	movl	-20(%ebp), %edx
	movl	module(,%edx,4), %edx
	movl	8(%edx), %edx
	subl	$4, %esp
	pushl	%eax
	pushl	-24(%ebp)
	pushl	%edx
	call	kmemcpy
	addl	$16, %esp
	movl	-20(%ebp), %eax
	movl	module(,%eax,4), %eax
	movl	-24(%ebp), %edx
	movl	%edx, 8(%eax)
	movl	-20(%ebp), %eax
	movl	module(,%eax,4), %eax
	movl	4(%eax), %eax
	addl	%eax, -24(%ebp)
	addl	$1, -20(%ebp)
.L71:
	movl	cnt_module, %eax
	cmpl	%eax, -20(%ebp)
	jb	.L72
	movl	$0, -16(%ebp)
	jmp	.L73
.L74:
	movl	-16(%ebp), %eax
	movl	module(,%eax,4), %eax
	movl	8(%eax), %eax
	movl	%eax, -12(%ebp)
	subl	$12, %esp
	pushl	-12(%ebp)
	call	elf_init_VMA
	addl	$16, %esp
	subl	$12, %esp
	pushl	-12(%ebp)
	call	elf_relocate
	addl	$16, %esp
	subl	$12, %esp
	pushl	-12(%ebp)
	call	load_elf
	addl	$16, %esp
	subl	$12, %esp
	pushl	-12(%ebp)
	call	set_kernelCall
	addl	$16, %esp
	addl	$1, -16(%ebp)
.L73:
	movl	cnt_module, %eax
	cmpl	%eax, -16(%ebp)
	jb	.L74
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	load_module, .-load_module
	.globl	init_pageDirectory
	.type	init_pageDirectory, @function
init_pageDirectory:
.LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 3, -12
	movl	$1024, -8(%ebp)
	movl	$0, %ebx
	jmp	.L76
.L77:
	movl	$1048576, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	$0, (%eax)
	addl	$1, %ebx
.L76:
	cmpl	-8(%ebp), %ebx
	jb	.L77
	movl	$0, %ebx
	jmp	.L78
.L79:
	movl	$1048576, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	$204800, %ecx
	movl	%ebx, %edx
	sall	$12, %edx
	addl	%ecx, %edx
	orl	$3, %edx
	movl	%edx, (%eax)
	addl	$1, %ebx
.L78:
	movl	size_reserveMemory, %eax
	shrl	$22, %eax
	cmpl	%ebx, %eax
	ja	.L79
	movl	$768, %ebx
	jmp	.L80
.L81:
	movl	$1048576, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	$1052672, %ecx
	leal	4193536(%ebx), %edx
	sall	$12, %edx
	addl	%ecx, %edx
	orl	$3, %edx
	movl	%edx, (%eax)
	addl	$1, %ebx
.L80:
	cmpl	-8(%ebp), %ebx
	jb	.L81
	nop
	addl	$16, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE12:
	.size	init_pageDirectory, .-init_pageDirectory
	.globl	init_pageTable_temp
	.type	init_pageTable_temp, @function
init_pageTable_temp:
.LFB13:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	$0, %ebx
	jmp	.L83
.L84:
	movl	$204800, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	%ebx, %edx
	sall	$12, %edx
	orl	$3, %edx
	movl	%edx, (%eax)
	addl	$1, %ebx
.L83:
	movl	size_reserveMemory, %eax
	shrl	$12, %eax
	cmpl	%ebx, %eax
	ja	.L84
	nop
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE13:
	.size	init_pageTable_temp, .-init_pageTable_temp
	.globl	init_pageTable_kernel
	.type	init_pageTable_kernel, @function
init_pageTable_kernel:
.LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	$786432, %ebx
	jmp	.L86
.L87:
	movl	$1052672, %edx
	leal	1072955392(%ebx), %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	$0, (%eax)
	addl	$1, %ebx
.L86:
	cmpl	$1048575, %ebx
	jbe	.L87
	movl	$0, %ebx
	jmp	.L88
.L89:
	movl	$1052672, %edx
	leal	0(,%ebx,4), %eax
	addl	%edx, %eax
	movl	$204800, %ecx
	leal	0(,%ebx,4), %edx
	addl	%ecx, %edx
	movl	(%edx), %edx
	subl	$1073741824, %edx
	movl	%edx, (%eax)
	addl	$1, %ebx
.L88:
	movl	size_reserveMemory, %eax
	shrl	$12, %eax
	cmpl	%ebx, %eax
	ja	.L89
	nop
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE14:
	.size	init_pageTable_kernel, .-init_pageTable_kernel
	.globl	handle_tag
	.data
	.align 4
	.type	handle_tag, @object
	.size	handle_tag, 28
handle_tag:
	.zero	12
	.long	handle_tag_module
	.zero	8
	.long	handle_tag_mmap
	.text
	.type	enable_page, @function
enable_page:
.LFB15:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	$1048576, %eax
#APP
# 302 "init_mem.c" 1
	movl %eax, %cr3
	movl %cr0, %eax
	orl $0x80010000, %eax
	movl %eax, %cr0
	
# 0 "" 2
#NO_APP
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE15:
	.size	enable_page, .-enable_page
	.section	.rodata
.LC15:
	.string	"##pageDirectory"
.LC16:
	.string	"%x "
.LC17:
	.string	"##pageTable_kernel"
.LC18:
	.string	"##pageTable_temp"
	.text
	.globl	debug_output
	.type	debug_output, @function
debug_output:
.LFB16:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	subl	$12, %esp
	pushl	$.LC15
	call	kputs
	addl	$16, %esp
	movl	$0, -20(%ebp)
	jmp	.L92
.L94:
	movl	$1048576, %edx
	movl	-20(%ebp), %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC16
	call	kprintf
	addl	$16, %esp
	movl	-20(%ebp), %eax
	notl	%eax
	andl	$3, %eax
	testl	%eax, %eax
	jne	.L93
	subl	$12, %esp
	pushl	$10
	call	kputchar
	addl	$16, %esp
.L93:
	addl	$1, -20(%ebp)
.L92:
	cmpl	$31, -20(%ebp)
	jbe	.L94
	subl	$12, %esp
	pushl	$.LC17
	call	kputs
	addl	$16, %esp
	movl	$0, -16(%ebp)
	jmp	.L95
.L97:
	movl	$1052672, %edx
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC16
	call	kprintf
	addl	$16, %esp
	movl	-16(%ebp), %eax
	notl	%eax
	andl	$3, %eax
	testl	%eax, %eax
	jne	.L96
	subl	$12, %esp
	pushl	$10
	call	kputchar
	addl	$16, %esp
.L96:
	addl	$1, -16(%ebp)
.L95:
	cmpl	$31, -16(%ebp)
	jbe	.L97
	subl	$12, %esp
	pushl	$.LC18
	call	kputs
	addl	$16, %esp
	movl	$0, -12(%ebp)
	jmp	.L98
.L100:
	movl	$204800, %edx
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC16
	call	kprintf
	addl	$16, %esp
	movl	-12(%ebp), %eax
	notl	%eax
	andl	$3, %eax
	testl	%eax, %eax
	jne	.L99
	subl	$12, %esp
	pushl	$10
	call	kputchar
	addl	$16, %esp
.L99:
	addl	$1, -12(%ebp)
.L98:
	cmpl	$31, -12(%ebp)
	jbe	.L100
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE16:
	.size	debug_output, .-debug_output
	.section	.rodata
.LC19:
	.string	"DappurOS initializing..."
.LC20:
	.string	"magic: %x\n"
.LC21:
	.string	"mbi: %p\n"
.LC22:
	.string	"enable page"
.LC23:
	.string	"allons-y"
	.text
	.globl	init_memory_
	.type	init_memory_, @function
init_memory_:
.LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	subl	$12, %esp
	pushl	$.LC19
	call	kputs
	addl	$16, %esp
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC20
	call	kprintf
	addl	$16, %esp
	cmpl	$920085129, 8(%ebp)
	jne	.L108
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	$.LC21
	call	kprintf
	addl	$16, %esp
	movl	12(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	.L109
	movl	12(%ebp), %eax
	movl	(%eax), %edx
	movl	12(%ebp), %eax
	subl	$4, %esp
	pushl	%edx
	pushl	%eax
	pushl	$1048576
	call	kmemcpy
	addl	$16, %esp
	movl	$1048584, -12(%ebp)
	jmp	.L105
.L107:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$6, %eax
	ja	.L106
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	handle_tag(,%eax,4), %eax
	testl	%eax, %eax
	je	.L106
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	handle_tag(,%eax,4), %eax
	subl	$12, %esp
	pushl	-12(%ebp)
	call	*%eax
	addl	$16, %esp
.L106:
	movl	-12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	subl	$12, %esp
	pushl	%eax
	call	align8
	addl	$16, %esp
	movl	%eax, -12(%ebp)
.L105:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L107
	movl	$2103552, %eax
	subl	$4, %esp
	pushl	$256
	pushl	$0
	pushl	%eax
	call	kmemset
	addl	$16, %esp
	movl	$2103808, %edx
	movl	len_reversePageTable, %eax
	sall	$2, %eax
	addl	%edx, %eax
	subl	$12, %esp
	pushl	%eax
	call	align12
	addl	$16, %esp
	movl	%eax, size_reserveMemory
	call	init_pageDirectory
	call	init_pageTable_temp
	call	debug_output
	call	load_module
	call	init_pageTable_kernel
	subl	$12, %esp
	pushl	$.LC22
	call	kputs
	addl	$16, %esp
	call	enable_page
	movl	$286, %eax
	movl	$80, %edx
	movl	$12, %ecx
#APP
# 367 "init_mem.c" 1
	movb %dl, %gs:(%eax)
	movb %cl, %gs:1(%eax)
	
# 0 "" 2
#NO_APP
	subl	$12, %esp
	pushl	$.LC23
	call	kputs
	addl	$16, %esp
	movl	$2103552, %eax
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	$0
	call	*%eax
	addl	$16, %esp
	jmp	.L101
.L108:
	nop
	jmp	.L101
.L109:
	nop
.L101:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE17:
	.size	init_memory_, .-init_memory_
	.section	.rodata
.LC24:
	.string	"Startup has failed"
	.text
	.globl	init_memory
	.type	init_memory, @function
init_memory:
.LFB18:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	init_memory_
	addl	$16, %esp
	subl	$12, %esp
	pushl	$.LC24
	call	kputs
	addl	$16, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE18:
	.size	init_memory, .-init_memory
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
