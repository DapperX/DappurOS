#ifndef _ARCH_PAGE_H
#define _ARCH_PAGE_H 1

#include "basic.h"

#define PAGE_BITWIDTH 12
#define PAGE_SIZE (1u<<PAGE_BITWIDTH)
#define PAGE_MASK (PAGE_SIZE-1)
#define PGDIR_BITWIDTH 10
#define PGDIR_SIZE (1u<<PGDIR_BITWIDTH)
#define PGDIR_MASK (PGDIR_SIZE-1)
#define PGTBL_BITWIDTH 10
#define PGTBL_SIZE (1u<<PGTBL_BITWIDTH)
#define PGTBL_MASK (PGTBL_SIZE-1)

// Present. If set, the page is actually in physical memory
// If clear, processor ignores the rest of bites
#define PDE_P (1<<0)
// Read/Write. If set, the page can be write(also lie on WP bit in CR0)
#define PDE_R (1<<1)
// User/Supervisor. If set, the page may be access to all
#define PDE_U (1<<2)
// Write-Through. If set, write-through instead of write-back
#define PDE_W (1<<3)
// Cache Disable, If set, the page will not be cached
#define PDE_D (1<<4)
// Accessed. If set, the page has been read or written to
#define PDE_A (1<<5)
// Page Size. If set, the page is 4MB rather than 4KB
#define PDE_S (1<<7)

// Same as PDE_P
#define PTE_P (1<<0)
// Same as PDE_R
#define PTE_R (1<<1)
// Same as PDE_U
#define PTE_U (1<<2)
// Same as PDE_W
#define PTE_W (1<<3)
// Same as PDE_D
#define PTE_C (1<<4)
// Same as PDE_A
#define PTE_A (1<<5)
// Dirty flag. If set, the page has been writen to
#define PTE_D (1<<6)
// Global. If set, prevent TLB from updating cache(need to enable this feature in CR4)
#define PTE_G (1<<8)

// Used. If set, corresponding page is used
// If clear, processor ignores the rest of bites
#define RPE_U (1<<0)
// Available. If set, it indicates the entry has used in special way(e.g. IO mapping) and cannot be modified
#define RPE_A (1<<1)

static inline void arch_invalidate_page(msize *vaddr)
{
	asm volatile(
		"invlpg	(%0)\n\t"
	:
	:
		"r"(vaddr)
	:
		"memory"
	);
}

static inline void arch_invalidate_page_all()
{
	asm volatile(
		"movl %%cr3, %%eax\n\t"
		"movl %%eax, %%cr3\n\t"
	: : :
		"memory", "eax"
	);
}

#endif // _ARCH_PAGE_H