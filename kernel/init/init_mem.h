#include "base.h"
#include "page.h"

// for now only support at most 3.75GB address
// just be convenient to avoid special memory area such as SLIC
#define MIN_MEMORY 0xf0000000
#define ADDR_HIGH_MEMORY 0xc0000000
// the start of physical address of kernel
#define OFFSET_KERNEL_LOW_MEMORY 0x100000

uint *const pageDirectory=(uint*)0x100000;
uint *const pageTable_kernel=(uint*)0x101000;
uint *const pageTable_temp=(uint*)0x32000;

// reversePageTable[i]: high 30 bits show the virtual page address(4bit align) used by i-th physical page
// low 2 bits show the attribute of this page
uint len_reversePageTable=0;
uint **const reversePageTable=(uint**)0x201A00;

typedef int (*kernelCall)(uint index, ...);
kernelCall *const kernelCallTable=(kernelCall*)0x201900;

uint ACPI_addr=0,ACPI_len=0;