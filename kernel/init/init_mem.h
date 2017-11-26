#include "base.h"
#include "page.h"
#include "kernel.h"

// the start of physical address of kernel
#define OFFSET_LOW_MEMORY 0x100000

uint *const pageDirectory=(uint*)(OFFSET_LOW_MEMORY+ADDR_PAGE_DIRECTORY);
uint *const pageTable_kernel=(uint*)(OFFSET_LOW_MEMORY+ADDR_PAGE_DIRECTORY+4*1024);
uint *const pageTable_temp=(uint*)0x32000;

// reversePageTable[i]: high 30 bits show the virtual page address(4bit align) used by i-th physical page
// low 2 bits show the attribute of this page
uint len_reversePageTable=0;
uint **const reversePageTable=(uint**)(OFFSET_LOW_MEMORY+ADDR_REVERSE_PAGE_TABLE);

kernelCall *const kernelCallTable=(kernelCall*)(OFFSET_LOW_MEMORY+ADDR_KERNEL_CALL_TABLE);

uint ACPI_addr=0,ACPI_len=0;