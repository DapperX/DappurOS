#ifndef _INIT_H
#define _INIT_H

#include "base.h"
#include "arch/x86/page.h"
#include "kernel.h"

// the start of physical address of kernel
#define ADDR_LOW_MEMORY 0x0
#define OFFSET_MAPPING OFFSET_PAGE_DIRECTORY

#define OFFSET_BOOTINFO OFFSET_IDT
#define BOOTINFO_END 0
#define BOOTINFO_MEM_TOTAL 1
#define BOOTINFO_MEM_USED 2
#define BOOTINFO_ACPI 5

// expose inner functions for testing or debugging
#ifndef NDEBUG
void* get_page_free(const u32 cnt,const u32 is_writable);
void elf_init_LMA(byte *const buffer);
void elf_relocate(byte *const buffer);
#endif

#endif //_INIT_H