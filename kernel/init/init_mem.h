#include "base.h"
#include "page.h"
#include "kernel.h"

// the start of physical address of kernel
#define OFFSET_LOW_MEMORY 0x100000

u32 *const pageDirectory=(u32*)(OFFSET_LOW_MEMORY+ADDR_PAGE_DIRECTORY);
u32 *const pageTable_kernel=(u32*)(OFFSET_LOW_MEMORY+ADDR_PAGE_DIRECTORY+4*1024);
u32 *const pageTable_temp=(u32*)0x32000;

// reversePageTable[i]: high 30 bits show the virtual page address(4bit align) used by i-th physical page
// low 2 bits show the attribute of this page
u32 len_reversePageTable=0;
u32 **const reversePageTable=(u32**)(OFFSET_LOW_MEMORY+ADDR_REVERSE_PAGE_TABLE);

kernelCall *const kernelCallTable=(kernelCall*)(OFFSET_LOW_MEMORY+ADDR_KERNEL_CALL_TABLE);

u32 ACPI_addr=0,ACPI_len=0;