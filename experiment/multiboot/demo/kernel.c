/* kernel.c - the C part of the kernel */
/* Copyright (C) 1999  Free Software Foundation, Inc.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#include "multiboot.h"

/* Macros. */

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000

/* Variables. */
/* Save the X position. */
static int xpos;
/* Save the Y position. */
static int ypos;
/* Point to the video memory. */
static volatile unsigned char *video;

/* Forward declarations. */
void cmain (unsigned long magic, unsigned long addr);
static void cls (void);
static void itoa (char *buf, int base, int d);
static void putchar (int c);
void printf (const char *format, ...);

/* Check if MAGIC is valid and print the Multiboot information structure
	pointed by ADDR. */
void
cmain (unsigned long magic, unsigned long addr)
{
	multiboot_info_t *mbi;

	/* Clear the screen. */
	cls ();

	/* Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		{
			printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
			return;
		}

	/* Set MBI to the address of the Multiboot information structure. */
	mbi = (multiboot_info_t *) addr;

	/* Print out the flags. */
	printf ("flags = 0x%x\n", (unsigned) mbi->flags);

	/* Are mem_* valid? */
	if (CHECK_FLAG (mbi->flags, 0))
		printf ("mem_lower = %uKB, mem_upper = %uKB\n",
						(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	/* Is boot_device valid? */
	if (CHECK_FLAG (mbi->flags, 1))
		printf ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);

	/* Is the command line passed? */
	if (CHECK_FLAG (mbi->flags, 2))
		printf ("cmdline = %s\n", (char *) mbi->cmdline);

	/* Are mods_* valid? */
	if (CHECK_FLAG (mbi->flags, 3))
		{
			multiboot_module_t *mod;
			int i;

			printf ("mods_count = %d, mods_addr = 0x%x\n",
							(int) mbi->mods_count, (int) mbi->mods_addr);
			for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
					i < mbi->mods_count;
					i++, mod++)
				printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
								(unsigned) mod->mod_start,
								(unsigned) mod->mod_end,
								(char *) mod->cmdline);
		}

	/* Bits 4 and 5 are mutually exclusive! */
	if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
		{
			printf ("Both bits 4 and 5 are set.\n");
			return;
		}

	/* Is the symbol table of a.out valid? */
	if (CHECK_FLAG (mbi->flags, 4))
		{
			multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

			printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
							"strsize = 0x%x, addr = 0x%x\n",
							(unsigned) multiboot_aout_sym->tabsize,
							(unsigned) multiboot_aout_sym->strsize,
							(unsigned) multiboot_aout_sym->addr);
		}

	/* Is the section header table of ELF valid? */
	if (CHECK_FLAG (mbi->flags, 5))
		{
			multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

			printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
							" addr = 0x%x, shndx = 0x%x\n",
							(unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
							(unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
		}

	/* Are mmap_* valid? */
	if (CHECK_FLAG (mbi->flags, 6))
		{
			multiboot_memory_map_t *mmap;

			printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
							(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
			for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
					(unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
					mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
																		+ mmap->size + sizeof (mmap->size)))
				printf (" size = 0x%x, base_addr = 0x%x%x,"
								" length = 0x%x%x, type = 0x%x\n",
								(unsigned) mmap->size,
								mmap->addr >> 32,
								mmap->addr & 0xffffffff,
								mmap->len >> 32,
								mmap->len & 0xffffffff,
								(unsigned) mmap->type);
		}
}

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
static void
cls (void)
{
	int i;

	video = (unsigned char *) VIDEO;

	for (i = 0; i < COLUMNS * LINES * 2; i++)
		*(video + i) = 0;

	xpos = 0;
	ypos = 0;
}

/* Convert the integer D to a string and save the string in BUF. If
	BASE is equal to 'd', interpret that D is decimal, and if BASE is
	equal to 'x', interpret that D is hexadecimal. */
static void
itoa (char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	/* If %d is specified and D is minus, put `-' in the head. */
	if (base == 'd' && d < 0)
		{
			*p++ = '-';
			buf++;
			ud = -d;
		}
	else if (base == 'x')
		divisor = 16;

	/* Divide UD by DIVISOR until UD == 0. */
	do
		{
			int remainder = ud % divisor;

			*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		}
	while (ud /= divisor);

	/* Terminate BUF. */
	*p = 0;

	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
		{
			char tmp = *p1;
			*p1 = *p2;
			*p2 = tmp;
			p1++;
			p2--;
		}
}

/* Put the character C on the screen. */
static void
putchar (int c)
{
	if (c == '\n' || c == '\r')
		{
		newline:
			xpos = 0;
			ypos++;
			if (ypos >= LINES)
				ypos = 0;
			return;
		}

	*(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
	*(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

	xpos++;
	if (xpos >= COLUMNS)
		goto newline;
}

/* Format a string and print it on the screen, just like the libc
	function printf. */
void
printf (const char *format, ...)
{
	char *arg;
	int c;
	char buf[20];

	__builtin_va_start(arg,format);

	while ((c = *format++) != 0)
		{
			if (c != '%')
				putchar (c);
			else
				{
					char *p;

					c = *format++;
					switch (c)
						{
						case 'd':
						case 'u':
						case 'x':
							itoa (buf, c, __builtin_va_arg(arg,int));
							p = buf;
							goto string;
							break;

						case 's':
							p = __builtin_va_arg(arg,char*);
							if (! p)
								p = "(null)";

						string:
							while (*p)
								putchar (*p++);
							break;

						default:
							putchar (__builtin_va_arg(arg,int));
							break;
						}
				}
		}
	__builtin_va_end(arg);
}