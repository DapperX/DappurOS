#include "control.h"
#include "print.h"
#include "memory.h"
#include "assert.h"
#include "debug.h"
#include "page.h"
#include "kernel.h"
#include "init_mem.h"

u32 *const pageDirectory = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_DIRECTORY);
kernelCall *const kernelCallTable = (kernelCall*)(ADDR_HIGH_MEMORY+OFFSET_KCT);
static kernelCall callList[];

#define CNT_MEM_TOTAL 4
u32 cnt_mem_total = 0;
static info_memory mem_total[CNT_MEM_TOTAL];

// upper align `val` to multiple of 2**`bit`
static inline u32 align(u32 val,u32 bit)
{
	register u32 mask=(1u<<bit)-1;
	return (val+mask)&(~mask);
}

int_var load_module(char *moduleName)
{
	DISABLE(moduleName);
	return 0;
}

int_var unload_module(u32 moduleId)
{
	DISABLE(moduleId);
	return 0;
}

int_var module_init()
{
	u32 canary = 0xDEADBEEF;
	kputs("[Control] Initializing...");
	
	u32 offset_available = 0;

	// Collect bootInfo
	info_header *bootInfo = (info_header*)(ADDR_HIGH_MEMORY+OFFSET_BOOTINFO);
	while(bootInfo->type!=BOOTINFO_END)
	{
		static u32 e = __builtin_ctz(sizeof(info_header));
		switch(bootInfo->type)
		{
			case BOOTINFO_MEM_TOTAL:
			{
				cnt_mem_total = bootInfo->cnt;
				if(cnt_mem_total>CNT_MEM_TOTAL)
				{
					kputs("Warning: Too many memory areas");
					cnt_mem_total = CNT_MEM_TOTAL;
				}
				kmemcpy(mem_total,bootInfo+1,sizeof(info_memory)*cnt_mem_total);
				bootInfo += 1+(align(sizeof(info_memory)*bootInfo->cnt,e)>>e);
				break;
			}
			case BOOTINFO_MEM_USED:
			{
				info_memory *last = &((info_memory*)(bootInfo+1))[bootInfo->cnt-1];
				offset_available = (u32)(last->start+last->len-ADDR_LOW_MEMORY); // $unsafe$
				bootInfo += 1+(align(sizeof(info_memory)*bootInfo->cnt,e)>>e);
				break;
			}
			case BOOTINFO_ACPI:
				bootInfo += 1+(align(sizeof(info_ACPI)*bootInfo->cnt,e)>>e);
				break;
		}
	}

	if(cnt_mem_total==0 || offset_available==0)
	{
		kputs("Warning: critical bootInfo not found");
		HALT;
	}

	kprintf("offset_available: %u\n",offset_available);
	DEBUG_BREAKPOINT;

	// Clear the temporary page mapping (low memory areas)
	pageDirectory[0] = 0;
	kmemset((byte*)ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT+2048,0,2048);

	// Initialize kernel stack
	u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
	u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);
	u32 *const addr_PTE_kernel = &pageTable_kernel[((ADDR_STACK-4096)>>12)&1023];
	u32 *const addr_PTE_init = &pageTable_init[((u32)addr_PTE_kernel>>12)&1023];

	// Mapping the page table entry
	if(!(*addr_PTE_init&PTE_P))
	{
		*addr_PTE_init = (ADDR_LOW_MEMORY+offset_available)|PTE_P|PTE_R;
		offset_available += 4096;
		kmemset((byte*)((uint_var)addr_PTE_kernel&~4095u),0,4096);
	}

	pageDirectory[(ADDR_STACK-4096)>>22] = ((*addr_PTE_init)&~4095u)|PDE_P|PDE_R;
	*addr_PTE_kernel = (ADDR_LOW_MEMORY+offset_available)|PTE_P|PTE_R;
	offset_available += 4096;

	// Transfer the stack to the formal position
	kmemcpy((byte*)ADDR_STACK-1024,(byte*)ADDR_HIGH_MEMORY+OFFSET_GDT-1024,1024);
	asm volatile(
		"addl %0, %%esp\n\t"
		"addl %0, %%ebp\n\t"
	::
		"i"(ADDR_STACK-(ADDR_HIGH_MEMORY+OFFSET_GDT))
	:
		"esp"
	);

	kprintf("canary: %x\n",canary);
	DEBUG_BREAKPOINT;

	// Initialize loaded modules
	for(u32 i=0;i<LEN_ARRAY(module_preload);++i)
	{
		KASSERT(kernelCallTable[i]);
		kernelCallTable[i](KERNEL_CALL_INIT);
	}
	for(u32 i=0;i<LEN_ARRAY(module_need_load);++i)
	{
		load_module(module_need_load[i]);
	}

	return 0;
}

int_var module_exit()
{
	return 0;
}

static kernelCall callList[]={
	[0]=(kernelCall)module_init,
	[1]=(kernelCall)module_exit,
	[8]=(kernelCall)load_module,
	[9]=(kernelCall)unload_module
};

int_var module_kernelCall(u32 index,...)
{
	kcls();
	kprintf("[Control] index: %d\n",index);
	KASSERT(index<KERNEL_CALL_SELF_DEFINED+LEN_ARRAY(callList));
	KASSERT(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}