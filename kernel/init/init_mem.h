#ifndef _INIT_H
#define _INIT_H

#include "base.h"
#include "page.h"
#include "kernel.h"

// the start of physical address of kernel
#define OFFSET_LOW_MEMORY 0x100000
#define PAGE_TABLE_TEMP 0x32000

// expose inner functions to test or debug
#ifndef NDEBUG
void* get_page_free(const u32 cnt,const u32 is_writable);
void elf_init_LMA(byte *const buffer);
void elf_relocate(byte *const buffer);
#endif

#endif //_INIT_H