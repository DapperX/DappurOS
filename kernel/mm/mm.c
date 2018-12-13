#include "mm.h"
#include "arch/x86/page.h"
#include "memory.h"
#include "assert.h"
#include "debug.h"

static kernelCall callList[];

u32 *const pageDirectory = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_DIRECTORY);
u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);

#define CNT_LAYER 11
struct mm_layer{
	u32 top_stack;
	u32 *stack;
	u8 *bitmap_stack;
};
static struct mm_layer list_layer[CNT_LAYER];

// upper align `val` to multiple of 2**`bit`
static inline u32 align(u32 val,u32 bit)
{
	register u32 mask=(1u<<bit)-1;
	return (val+mask)&(~mask);
}

u32 layer_add(const u32 index, u32 page_begin, u32 cnt)
{
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"[mm] layer_add %u %p %u\n",index,page_begin,cnt);

//	struct mm_layer *layer = &list_layer[index];
	while(cnt--)
	{
	//	layer->stack[layer->top_stack++] = page_begin<<12;
	//	layer->bitmap_stack[page_begin>>index>>3] |= 1u<<((page_begin>>index)&7);
		page_begin += 1u<<index;
	}
	return page_begin;
}

usize init_buddySystem()
{
	const info_memory *mem_total;
	const u32 cnt_mem_total = (u32)KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+4,&mem_total);
	u32 cnt_page_total = (u32)(mem_total[cnt_mem_total-1].end>>12);

	// Settle pointers in the layer
	byte *address = (byte*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL+0x100000);
	for(u32 i=0;i<CNT_LAYER;++i)
	{
		list_layer[i].top_stack = 0;
		list_layer[i].stack = (u32*)address;
		address += align(cnt_page_total>>i,LOG2(SIZE_POINTER));
		list_layer[i].bitmap_stack = (u8*)address;
		address += align(((cnt_page_total>>i)+7)>>3,LOG2(SIZE_POINTER));
	}
	/*
	// Traversal the page table to get the physical memory usage
	u32 paddr_last = 0;
	// Scan the page directory
	for(u32 i=(ADDR_HIGH_MEMORY>>22);i<(MAX_MEMORY>>22);++i)
	{
		register u32 entry = pageDirectory[i];
		if((entry&PDE_P) && entry>paddr_last) paddr_last = entry;
	}
	// Scan the init page table
	for(u32 i=0;i<1024;++i)
	{
		register u32 entry = pageTable_init[i];
		if((entry&PTE_P) && entry>paddr_last) paddr_last = entry;
	}
	// Scan the kernel page table
	for(u32 i=0;i<256;++i)
	{
		// Check wether the page table entry is mapped
		if(!(pageTable_init[((u32)&pageTable_kernel[i*1024]>>12)&1023]&PTE_P)) continue;
		for(u32 j=0;j<1024;++j)
		{
			register u32 entry = pageTable_kernel[i*1024+j];
			if((entry&PTE_P) && entry>paddr_last) paddr_last = entry;
		}
	}
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"paddr_last: %p\n",paddr_last);
	DEBUG_BREAKPOINT;
	*/
	/*
	// Manually allocate memory for the buddy system
	// because the memory management is not available yet
	for(u32 i=(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL+0x100000);i<(u32)address;i+=4096)
	{
		u32 *const addr_PTE_kernel = &pageTable_kernel[((ADDR_STACK-4096)>>12)&1023];
		u32 *const addr_PTE_init = &pageTable_init[((u32)addr_PTE_kernel>>12)&1023];

		// Mapping the page table entry
		if(!(*addr_PTE_init&PTE_P))
		{
			*addr_PTE_init = (ADDR_LOW_MEMORY+offset_available)|PTE_P|PTE_R;
			offset_available += 4096;
			kmemset((byte*)((usize)addr_PTE_kernel&~4095u),0,4096);
		}

		pageDirectory[(ADDR_STACK-4096)>>22] = ((*addr_PTE_init)&~4095u)|PDE_P|PDE_R;
		*addr_PTE_kernel = (ADDR_LOW_MEMORY+offset_available)|PTE_P|PTE_R;
		offset_available += 4096;
	}
	*/

	for(u32 i=0;i<cnt_mem_total;++i)
	{
		u32 page_begin = align((u32)mem_total[i].begin,12)>>12;
		u32 page_end = (u32)mem_total[i].end>>12;

		KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
			"%u:%u\n",page_begin,page_end);

		// Align `page_begin` to some bound (up to 4K*2^CNT_LAYER = 4M)
		while(page_begin<page_end && (page_begin&((1u<<(CNT_LAYER-1))-1)))
		{
			u32 lowbit = (u32)__builtin_ctz(page_begin);
			if(page_begin+(1u<<lowbit)>page_end) break;
			page_begin = layer_add(lowbit,page_begin,1);
		}

		// Handle the 4M pages
		u32 cnt_page = page_end-page_begin;
		page_begin = layer_add(CNT_LAYER-1,page_begin,cnt_page>>(CNT_LAYER-1));
		cnt_page &= ((1u<<(CNT_LAYER-1))-1);

		KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
				"%u %p\n",cnt_page,page_begin<<12);

		// Fill the rest of pages into the layer
		while(cnt_page)
		{
			u32 highbit = 31-(u32)__builtin_clz(cnt_page);
			page_begin = layer_add(highbit,page_begin,1);
			cnt_page -= 1u<<highbit;
		}
	}
	return 0;
}

usize module_init()
{
	/*
		TODO:
			get memory information
			set interrupt entry
	*/
	init_buddySystem();
	return 0;
}

usize module_exit()
{
	return 0;
}

static kernelCall callList[]={
	[KERNEL_CALL_INIT]=(kernelCall)module_init,
	[KERNEL_CALL_EXIT]=(kernelCall)module_exit,
};

KCALL_DISPATCH usize module_kernelCall(u32 funct)
{
	KASSERT(funct<LEN_ARRAY(callList));
	KASSERT(callList[funct]);
	JMP_INPLACE(callList[funct]);
	return 0;
}