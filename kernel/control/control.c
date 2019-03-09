#include "control.h"
#include "print.h"
#include "memory.h"
#include "macro.h"
#include "assert.h"
#include "debug.h"
#include "arch/x86/page.h"
#include "kernel.h"
#include "init_mem.h"

u32 *const pageDirectory = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_DIRECTORY);
kCall_dispatch *const kernelCallTable = (kCall_dispatch*)(ADDR_HIGH_MEMORY+OFFSET_KCT);
static kernelCall callList[];

#define CNT_MEM_TOTAL 4
u32 cnt_mem_total = 0;
static info_memory mem_total[CNT_MEM_TOTAL];
static info_memory mem_used_init = (info_memory){111ull, 222ull};

// upper align `val` to multiple of 2**`bit`
static inline u32 align(u32 val,u32 bit)
{
	register u32 mask=(1u<<bit)-1;
	return (val+mask)&(~mask);
}

usize load_module(char *moduleName)
{
	DISABLE(moduleName);
	return 0;
}

usize unload_module(u32 moduleId)
{
	DISABLE(moduleId);
	return 0;
}

u32 alloc_raw(u32 offset_available, u32 vaddr)
{
	u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
	u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);
	u32 *const addr_PTE_kernel = &pageTable_kernel[(vaddr>>12)&(256*1024-1)];
	u32 *const addr_PTE_init = &pageTable_init[((u32)addr_PTE_kernel>>12)&1023];

	// Mapping the page for the new page table first
	if(!(*addr_PTE_init&PTE_P))
	{
		*addr_PTE_init = (ADDR_LOW_MEMORY+offset_available)|PTE_P|PTE_R;
		offset_available += 4096;
		kmemset((byte*)((usize)addr_PTE_kernel&~4095u),0,4096);
	}
	pageDirectory[vaddr>>22] = ((*addr_PTE_init)&~4095u)|PDE_P|PDE_R;
	*addr_PTE_kernel = (ADDR_LOW_MEMORY+offset_available)|PTE_P|PTE_R;
	offset_available += 4096;
	return offset_available;
}

void fix_page()
{
	u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
	u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);

	// Clear the temporary page mapping (low memory areas)
	pageDirectory[0] = 0;
	// Clear the initial reserved area #1
	kmemset((byte*)pageTable_init+2048, 0, 2048);
	// Merge the init page table into the kernel page table
	pageTable_init[((u32)&pageTable_kernel[0]>>12)&1023]
		= pageTable_init[((u32)pageTable_init>>12)&1023];
}

u32 init_stack(u32 offset_available)
{
	// Allocate the kernel stack
	offset_available = alloc_raw(offset_available,ADDR_STACK-4096);

	// Backtrace the whole stack to modify all stored EBPs
	u32 *ebp;
	asm volatile("movl %%ebp, %0":"=r"(ebp));
	while(ebp>=(u32*)ADDR_HIGH_MEMORY)
	{
		u32 ebp_old = *ebp;
		*ebp += ADDR_STACK-(ADDR_HIGH_MEMORY+OFFSET_GDT);
		ebp = (u32*)ebp_old;
	}

	// Transfer the stack to the formal position
	kmemcpy((byte*)ADDR_STACK-1024,(byte*)ADDR_HIGH_MEMORY+OFFSET_GDT-1024,1024);

	// Since the compiler doesn't allow to declare using EBP in asm,
	// the following code that modifies EBP register should be left at the end of the function,
	// to avoid unexpected behaviors due to the optimization.
	asm volatile(
		"addl %0, %%esp\n\t"
		"addl %0, %%ebp\n\t"
	::
		"i"(ADDR_STACK-(ADDR_HIGH_MEMORY+OFFSET_GDT))
	:
		"esp"
	);
	// Allocate the auxiliary stack
	// offset_available = alloc_raw(offset_available,ADDR_STACK);
	// *(u32*)ADDR_STACK = ADDR_STACK;
	return offset_available;
}

static void init_IDT()
{
	kmemset((byte*)(ADDR_HIGH_MEMORY+OFFSET_IDT), 0, 2048);
}

static void init_clock()
{
}

usize kernelcall_dummy()
{
	u32 ret_addr;
	asm volatile(
		"mov	4(%%ebp),%0\n\t"
	:
		"=r"(ret_addr)
	);
	kprintf("Dummy Dummy Call from %p\n", ret_addr);
	return 0;
}

usize module_init(info_header* bootInfo)
{
	kcls();
	kputs("[Control] Initializing...");
	
	u32 offset_available = 0;

	// Collect bootInfo
	// info_header *bootInfo = (info_header*)(ADDR_HIGH_MEMORY+OFFSET_BOOTINFO);
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
				offset_available = (u32)(last->end-ADDR_LOW_MEMORY); // $unsafe$
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
		kputs("Error: critical bootInfo not found");
		HALT;
	}
	kprintf("offset_available: %u\n",offset_available);

	fix_page();
	offset_available = init_stack(offset_available);
	init_IDT();
	init_clock();

	mem_used_init = (info_memory){ADDR_LOW_MEMORY+OFFSET_MAPPING, ADDR_LOW_MEMORY+offset_available};

	kputs("Ready to init loaded modules");
	DEBUG_BREAKPOINT;

	// Initialize loaded modules
	for(u32 i=0;i<LEN_ARRAY(module_preload);++i)
	{
		KASSERT(kernelCallTable[module_preload[i]]);
		KCALL(module_preload[i], KERNEL_CALL_INIT);
	}

	kprintf("Done");
	HALT;

	// Disable the functions only used for initializing loaded modules
	callList[KERNEL_CALL_SELF_DEFINED+4] = (kernelCall)kernelcall_dummy;
	callList[KERNEL_CALL_SELF_DEFINED+5] = (kernelCall)kernelcall_dummy;

	for(u32 i=0;i<LEN_ARRAY(module_need_load);++i)
	{
		load_module(module_need_load[i]);
	}
	return 0;
}

usize module_exit()
{
	return 0;
}

usize get_memory_total(const info_memory **const p)
{
	*p = mem_total;
	return cnt_mem_total;
}

usize get_memory_used_init(info_memory *const p)
{
	*p = mem_used_init;
	return 0;
}

usize kernelCall_validity(const u32 index, const u32 funct, const bool valid)
{
	// KASSERT(funct<LEN_ARRAY(callList));
	KASSERT(callList[funct]);
	// Lacks of permission controlling for now
	if(index!=module_kernelCall_index)
		return KCALL(index, KERNEL_CALL_VALIDITY, funct, valid);
	register usize old_flag = (usize)callList[index]&KERNEL_CALL_FLAG_INVALID;
	callList[index] = (kernelCall)((usize)callList[index]&(valid?~KERNEL_CALL_FLAG_INVALID:KERNEL_CALL_FLAG_INVALID));
	return old_flag;
}

static kernelCall callList[] = {
	[KERNEL_CALL_INIT]=(kernelCall)module_init,
	[KERNEL_CALL_EXIT]=(kernelCall)module_exit,
	[KERNEL_CALL_VALIDITY]=(kernelCall)kernelCall_validity,
	[KERNEL_CALL_SELF_DEFINED+0]=(kernelCall)load_module,
	[KERNEL_CALL_SELF_DEFINED+1]=(kernelCall)unload_module,
	[KERNEL_CALL_SELF_DEFINED+4]=(kernelCall)get_memory_total,
	[KERNEL_CALL_SELF_DEFINED+5]=(kernelCall)get_memory_used_init,
	[KERNEL_CALL_SELF_DEFINED+8]=(kernelCall)kprintf
};

KCALL_DISPATCH usize module_kernelCall(u32 funct)
{
//	kprintf("[Control] funct: %d\n",funct);
	KASSERT(funct<LEN_ARRAY(callList));
	KASSERT(callList[funct]);
	// KASSERT(!((usize)(callList[funct])&KERNEL_CALL_FLAG_INVALID));
	JMP_INPLACE(callList[funct]);
	return 0;
}