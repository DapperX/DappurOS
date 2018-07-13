[SECTION multiboot_header noalloc]
ALIGN 8

MULTIBOOT_HEADER_MAGIC		equ	0xE85250D6
MULTIBOOT_HEADER_ARCH_I386	equ	0

header_start:
	dd MULTIBOOT_HEADER_MAGIC		; magic number (multiboot 2)
	dd MULTIBOOT_HEADER_ARCH_I386	; architecture 0 (protected mode i386)
	dd header_end - header_start	; header length
	;checksum
	dd 0x100000000 - (MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_ARCH_I386 + (header_end - header_start))

	;Multiboot information request
	dw 0x1
	dw 0x0
	dd 16
	dd 3 ; module
	dd 6 ; mmap

	;required end tag
	dw 0	; type
	dw 0	; flags
	dd 8	; size
header_end: