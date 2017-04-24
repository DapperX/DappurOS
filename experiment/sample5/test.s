	.file	"test.c"
	.text
.Ltext0:
	.local	xpos
	.comm	xpos,4,4
	.local	ypos
	.comm	ypos,4,4
	.local	video
	.comm	video,4,4
	.type	cls, @function
cls:
.LFB0:
	.file 1 "kernel.c"
	.loc 1 23 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	.loc 1 24 0
	movl	$753664, video
.LBB2:
	.loc 1 26 0
	movl	$0, -4(%ebp)
	jmp	.L2
.L3:
	.loc 1 26 0 is_stmt 0 discriminator 3
	movl	video, %edx
	movl	-4(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
	addl	$1, -4(%ebp)
.L2:
	.loc 1 26 0 discriminator 1
	cmpl	$3839, -4(%ebp)
	jle	.L3
.LBE2:
	.loc 1 28 0 is_stmt 1
	movl	$0, xpos
	movl	$0, ypos
	.loc 1 29 0
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	cls, .-cls
	.type	itoa, @function
itoa:
.LFB1:
	.loc 1 35 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 3, -12
	.loc 1 36 0
	movl	8(%ebp), %eax
	movl	%eax, -28(%ebp)
	.loc 1 37 0
	movl	16(%ebp), %eax
	movl	%eax, -24(%ebp)
	.loc 1 38 0
	movl	$10, -20(%ebp)
	.loc 1 40 0
	cmpl	$100, 12(%ebp)
	jne	.L5
	.loc 1 40 0 is_stmt 0 discriminator 1
	cmpl	$0, 16(%ebp)
	jns	.L5
	.loc 1 40 0 discriminator 2
	movl	-28(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -28(%ebp)
	movb	$45, (%eax)
	addl	$1, 8(%ebp)
	movl	16(%ebp), %eax
	negl	%eax
	movl	%eax, -24(%ebp)
	jmp	.L6
.L5:
	.loc 1 41 0 is_stmt 1
	cmpl	$120, 12(%ebp)
	jne	.L6
	.loc 1 41 0 is_stmt 0 discriminator 1
	movl	$16, -20(%ebp)
.L6:
.LBB3:
	.loc 1 44 0 is_stmt 1
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	movl	$0, %edx
	divl	%ecx
	movl	%edx, %eax
	movl	%eax, -8(%ebp)
	.loc 1 45 0
	movl	-28(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -28(%ebp)
	cmpl	$9, -8(%ebp)
	jg	.L7
	.loc 1 45 0 is_stmt 0 discriminator 1
	movl	-8(%ebp), %edx
	addl	$48, %edx
	jmp	.L8
.L7:
	.loc 1 45 0 discriminator 2
	movl	-8(%ebp), %edx
	addl	$87, %edx
.L8:
	.loc 1 45 0 discriminator 4
	movb	%dl, (%eax)
.LBE3:
	.loc 1 46 0 is_stmt 1 discriminator 4
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	movl	$0, %edx
	divl	%ecx
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	.L6
.LBB4:
	.loc 1 48 0
	movl	8(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-28(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -12(%ebp)
	jmp	.L9
.L10:
.LBB5:
	.loc 1 50 0 discriminator 3
	movl	-16(%ebp), %eax
	movzbl	(%eax), %ebx
	.loc 1 51 0 discriminator 3
	movl	-12(%ebp), %eax
	movzbl	(%eax), %edx
	movl	-16(%ebp), %eax
	movb	%dl, (%eax)
	.loc 1 52 0 discriminator 3
	movl	-12(%ebp), %eax
	movb	%bl, (%eax)
.LBE5:
	.loc 1 48 0 discriminator 3
	addl	$1, -16(%ebp)
	subl	$1, -12(%ebp)
.L9:
	.loc 1 48 0 is_stmt 0 discriminator 1
	movl	-16(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jb	.L10
.LBE4:
	.loc 1 54 0 is_stmt 1
	movl	-28(%ebp), %eax
	movb	$0, (%eax)
	.loc 1 55 0
	nop
	addl	$32, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	itoa, .-itoa
	.type	putchar_, @function
putchar_:
.LFB2:
	.loc 1 59 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	.loc 1 60 0
	cmpl	$10, 8(%ebp)
	je	.L12
	.loc 1 60 0 is_stmt 0 discriminator 1
	cmpl	$13, 8(%ebp)
	jne	.L13
	jmp	.L12
.L17:
	.loc 1 71 0 is_stmt 1
	nop
.L12:
	.loc 1 63 0
	movl	$0, xpos
	.loc 1 64 0
	movl	ypos, %eax
	addl	$1, %eax
	movl	%eax, ypos
	movl	ypos, %eax
	cmpl	$23, %eax
	jle	.L16
	.loc 1 64 0 is_stmt 0 discriminator 1
	movl	$0, ypos
	.loc 1 65 0 is_stmt 1 discriminator 1
	jmp	.L16
.L13:
	.loc 1 68 0
	movl	video, %ecx
	movl	ypos, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	xpos, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	%ecx, %eax
	movl	8(%ebp), %edx
	movb	%dl, (%eax)
	.loc 1 69 0
	movl	video, %ecx
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
	addl	%ecx, %eax
	movb	$7, (%eax)
	.loc 1 71 0
	movl	xpos, %eax
	addl	$1, %eax
	movl	%eax, xpos
	movl	xpos, %eax
	cmpl	$79, %eax
	jg	.L17
	jmp	.L11
.L16:
	.loc 1 65 0
	nop
.L11:
	.loc 1 72 0
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	putchar_, .-putchar_
	.section	.rodata
.LC0:
	.string	"(null)"
	.text
	.globl	printf_tmp
	.type	printf_tmp, @function
printf_tmp:
.LFB3:
	.loc 1 77 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$104, %esp
	movl	8(%ebp), %eax
	movl	%eax, -92(%ebp)
	.loc 1 77 0
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	.loc 1 78 0
	leal	-92(%ebp), %eax
	movl	%eax, -88(%ebp)
	.loc 1 82 0
	addl	$4, -88(%ebp)
	.loc 1 83 0
	jmp	.L19
.L28:
.LBB6:
	.loc 1 85 0
	cmpl	$37, -80(%ebp)
	je	.L20
	.loc 1 87 0
	subl	$12, %esp
	pushl	-80(%ebp)
	call	putchar
	addl	$16, %esp
	.loc 1 88 0
	jmp	.L19
.L20:
	.loc 1 91 0
	movl	-92(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -92(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%eax, -80(%ebp)
	.loc 1 93 0
	movl	-80(%ebp), %eax
	cmpl	$115, %eax
	je	.L22
	cmpl	$115, %eax
	jg	.L23
	cmpl	$100, %eax
	je	.L24
	jmp	.L21
.L23:
	cmpl	$117, %eax
	je	.L24
	cmpl	$120, %eax
	jne	.L21
.L24:
	.loc 1 98 0
	movl	-88(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -88(%ebp)
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	%eax
	pushl	-80(%ebp)
	leal	-76(%ebp), %eax
	pushl	%eax
	call	itoa
	addl	$16, %esp
	.loc 1 99 0
	leal	-76(%ebp), %eax
	movl	%eax, -84(%ebp)
	.loc 1 100 0
	jmp	.L25
.L22:
	.loc 1 104 0
	movl	-88(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -88(%ebp)
	movl	(%eax), %eax
	movl	%eax, -84(%ebp)
	.loc 1 105 0
	cmpl	$0, -84(%ebp)
	jne	.L26
	.loc 1 105 0 is_stmt 0 discriminator 1
	movl	$.LC0, -84(%ebp)
.L25:
	.loc 1 107 0 is_stmt 1
	jmp	.L26
.L27:
	.loc 1 107 0 is_stmt 0 discriminator 2
	movl	-84(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	subl	$12, %esp
	pushl	%eax
	call	putchar
	addl	$16, %esp
	addl	$1, -84(%ebp)
.L26:
	.loc 1 107 0 discriminator 1
	movl	-84(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L27
	.loc 1 108 0 is_stmt 1
	jmp	.L19
.L21:
	.loc 1 111 0
	movl	-88(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -88(%ebp)
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	putchar
	addl	$16, %esp
	.loc 1 112 0
	nop
.L19:
.LBE6:
	.loc 1 83 0
	movl	-92(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -92(%ebp)
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%eax, -80(%ebp)
	cmpl	$0, -80(%ebp)
	jne	.L28
	.loc 1 115 0
	nop
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L29
	call	__stack_chk_fail
.L29:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	printf_tmp, .-printf_tmp
	.section	.rodata
.LC1:
	.string	"eax: %d\n"
.LC2:
	.string	"total size: %d\n"
.LC3:
	.string	"%d "
	.text
	.globl	cc
	.type	cc, @function
cc:
.LFB4:
	.loc 1 118 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	.loc 1 120 0
	subl	$8, %esp
	pushl	$1048580
	pushl	$.LC1
	call	printf_tmp
	addl	$16, %esp
	.loc 1 121 0
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	.loc 1 122 0
	subl	$8, %esp
	pushl	-12(%ebp)
	pushl	$.LC2
	call	printf
	addl	$16, %esp
.LBB7:
	.loc 1 123 0
	movl	$0, -16(%ebp)
	jmp	.L31
.L32:
	.loc 1 125 0 discriminator 3
	movl	-16(%ebp), %edx
	movl	12(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC3
	call	printf
	addl	$16, %esp
	.loc 1 123 0 discriminator 3
	addl	$1, -16(%ebp)
.L31:
	.loc 1 123 0 is_stmt 0 discriminator 1
	movl	-16(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jl	.L32
.LBE7:
	.loc 1 127 0 is_stmt 1
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	cc, .-cc
	.section	.rodata
.LC4:
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"abcde"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.file 2 "test.c"
	.loc 2 4 0
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
	subl	$20, %esp
	.loc 2 4 0
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	.loc 2 5 0
	movl	.LC4, %eax
	movl	%eax, -22(%ebp)
	movl	.LC4+4, %eax
	movl	%eax, -18(%ebp)
	movzwl	.LC4+8, %eax
	movw	%ax, -14(%ebp)
	.loc 2 6 0
	movb	$9, -22(%ebp)
	.loc 2 7 0
	subl	$8, %esp
	leal	-22(%ebp), %eax
	pushl	%eax
	pushl	$12345
	call	cc
	addl	$16, %esp
	movl	$0, %eax
	.loc 2 8 0
	movl	-12(%ebp), %edx
	xorl	%gs:20, %edx
	je	.L35
	call	__stack_chk_fail
.L35:
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x2e6
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF25
	.byte	0xc
	.long	.LASF26
	.long	.LASF27
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF7
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF8
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF9
	.uleb128 0x4
	.byte	0x4
	.long	0x78
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF10
	.uleb128 0x4
	.byte	0x4
	.long	0x85
	.uleb128 0x5
	.long	0x78
	.uleb128 0x6
	.string	"cls"
	.byte	0x1
	.byte	0x16
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0xb6
	.uleb128 0x7
	.long	.LBB2
	.long	.LBE2-.LBB2
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x1a
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.byte	0
	.uleb128 0x9
	.long	.LASF13
	.byte	0x1
	.byte	0x22
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x16a
	.uleb128 0xa
	.string	"s"
	.byte	0x1
	.byte	0x22
	.long	0x72
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xb
	.long	.LASF11
	.byte	0x1
	.byte	0x22
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0xa
	.string	"d"
	.byte	0x1
	.byte	0x22
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x8
	.string	"p"
	.byte	0x1
	.byte	0x24
	.long	0x72
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x8
	.string	"ud"
	.byte	0x1
	.byte	0x25
	.long	0x3a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xc
	.long	.LASF12
	.byte	0x1
	.byte	0x26
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xd
	.long	.LBB3
	.long	.LBE3-.LBB3
	.long	0x132
	.uleb128 0x8
	.string	"r"
	.byte	0x1
	.byte	0x2c
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x7
	.long	.LBB4
	.long	.LBE4-.LBB4
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x30
	.long	0x72
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x8
	.string	"j"
	.byte	0x1
	.byte	0x30
	.long	0x72
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x7
	.long	.LBB5
	.long	.LBE5-.LBB5
	.uleb128 0x8
	.string	"t"
	.byte	0x1
	.byte	0x32
	.long	0x78
	.uleb128 0x1
	.byte	0x53
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x9
	.long	.LASF14
	.byte	0x1
	.byte	0x3a
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x197
	.uleb128 0xa
	.string	"c"
	.byte	0x1
	.byte	0x3a
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xe
	.long	.LASF16
	.byte	0x1
	.byte	0x3e
	.long	.L12
	.byte	0
	.uleb128 0xf
	.long	.LASF18
	.byte	0x1
	.byte	0x4c
	.long	.LFB3
	.long	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x20a
	.uleb128 0xb
	.long	.LASF15
	.byte	0x1
	.byte	0x4c
	.long	0x7f
	.uleb128 0x3
	.byte	0x91
	.sleb128 -100
	.uleb128 0x10
	.uleb128 0x8
	.string	"arg"
	.byte	0x1
	.byte	0x4e
	.long	0x20a
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x8
	.string	"c"
	.byte	0x1
	.byte	0x4f
	.long	0x4f
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x8
	.string	"buf"
	.byte	0x1
	.byte	0x50
	.long	0x210
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.uleb128 0xe
	.long	.LASF17
	.byte	0x1
	.byte	0x6a
	.long	.L25
	.uleb128 0x7
	.long	.LBB6
	.long	.LBE6-.LBB6
	.uleb128 0x8
	.string	"p"
	.byte	0x1
	.byte	0x5a
	.long	0x72
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.byte	0
	.byte	0
	.uleb128 0x4
	.byte	0x4
	.long	0x72
	.uleb128 0x11
	.long	0x78
	.long	0x220
	.uleb128 0x12
	.long	0x6b
	.byte	0x3f
	.byte	0
	.uleb128 0x13
	.string	"cc"
	.byte	0x1
	.byte	0x75
	.long	.LFB4
	.long	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.long	0x275
	.uleb128 0xa
	.string	"eax"
	.byte	0x1
	.byte	0x75
	.long	0x25
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xb
	.long	.LASF19
	.byte	0x1
	.byte	0x75
	.long	0x72
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0xc
	.long	.LASF20
	.byte	0x1
	.byte	0x79
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x7
	.long	.LBB7
	.long	.LBE7-.LBB7
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x7b
	.long	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.uleb128 0x14
	.long	.LASF21
	.byte	0x2
	.byte	0x3
	.long	0x4f
	.long	.LFB5
	.long	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.long	0x29b
	.uleb128 0x8
	.string	"a"
	.byte	0x2
	.byte	0x5
	.long	0x29b
	.uleb128 0x2
	.byte	0x75
	.sleb128 -22
	.byte	0
	.uleb128 0x11
	.long	0x78
	.long	0x2ab
	.uleb128 0x12
	.long	0x6b
	.byte	0x9
	.byte	0
	.uleb128 0xc
	.long	.LASF22
	.byte	0x1
	.byte	0xf
	.long	0x4f
	.uleb128 0x5
	.byte	0x3
	.long	xpos
	.uleb128 0xc
	.long	.LASF23
	.byte	0x1
	.byte	0x11
	.long	0x4f
	.uleb128 0x5
	.byte	0x3
	.long	ypos
	.uleb128 0xc
	.long	.LASF24
	.byte	0x1
	.byte	0x13
	.long	0x2de
	.uleb128 0x5
	.byte	0x3
	.long	video
	.uleb128 0x4
	.byte	0x4
	.long	0x2e4
	.uleb128 0x15
	.long	0x2c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xa
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.value	0
	.value	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF5:
	.string	"short int"
.LASF9:
	.string	"sizetype"
.LASF21:
	.string	"main"
.LASF18:
	.string	"printf_tmp"
.LASF6:
	.string	"long long int"
.LASF16:
	.string	"newline"
.LASF8:
	.string	"long int"
.LASF17:
	.string	"string"
.LASF19:
	.string	"addr"
.LASF22:
	.string	"xpos"
.LASF14:
	.string	"putchar_"
.LASF13:
	.string	"itoa"
.LASF1:
	.string	"unsigned char"
.LASF26:
	.string	"test.c"
.LASF4:
	.string	"signed char"
.LASF7:
	.string	"long long unsigned int"
.LASF0:
	.string	"unsigned int"
.LASF2:
	.string	"short unsigned int"
.LASF27:
	.string	"/home/dapper/\346\241\214\351\235\242/DappurOS/experiment/sample5"
.LASF10:
	.string	"char"
.LASF23:
	.string	"ypos"
.LASF3:
	.string	"long unsigned int"
.LASF20:
	.string	"size"
.LASF12:
	.string	"divisor"
.LASF11:
	.string	"base"
.LASF15:
	.string	"format"
.LASF25:
	.string	"GNU C11 5.4.0 20160609 -mtune=generic -march=i686 -g -fstack-protector-strong"
.LASF24:
	.string	"video"
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
