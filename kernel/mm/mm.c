#include "mm.h"
#include "arch/x86/page.h"
#include "memory.h"
#include "assert.h"
#include "math.h"
#include "debug.h"

static kernelCall callList[];

u32 *const pageDirectory = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_DIRECTORY);
u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);

#define CNT_LAYER 11
struct mm_layer{
	u32 stack_top;
	u32 *stack;
	usize *bitmap_present;
	usize *bitmap_instack;
};
static struct mm_layer list_layer[CNT_LAYER];

// upper align `val` to multiple of 2**`bit`
static inline usize align(usize val, u32 bit)
{
	register u32 mask=(1u<<bit)-1;
	return (val+mask)&(~mask);
}

void TLB_invalidate_page(u32 *pgdir, u32 *vaddr)
{
	arch_invalidate_page(vaddr);
}

u32 layer_add(const u32 index, u32 page_begin, u32 cnt)
{
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"[mmx] layer_addx %u %p %u\n",index,page_begin,cnt);

//	struct mm_layer *layer = &list_layer[index];
	while(cnt--)
	{
	//	layer->stack[layer->stack_top++] = page_begin<<PAGE_BITWIDTH;
	//	layer->bitmap_instack[page_begin>>index>>3] |= 1u<<((page_begin>>index)&7);
		page_begin += 1u<<index;
	}
	return page_begin;
}

usize init_buddySystem()
{
	const info_memory *mem_total;
	const u32 cnt_mem_total = (u32)KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+4, &mem_total);
	u32 cnt_page_total = (u32)(mem_total[cnt_mem_total-1].end>>PAGE_BITWIDTH);

	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "init_buddySystem\n");

	// Settle pointers in the layer
	byte *address = (byte*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL+0x100000);
	for(u32 i=0;i<CNT_LAYER;++i)
	{
		struct mm_layer *layer = &list_layer[i];
		layer->stack_top = 0;
		address = (byte*)align((usize)address, log2i(sizeof(*layer->stack)));
		layer->stack = (void*)address;
		address += cnt_page_total>>i;
		address = (byte*)align((usize)address, log2i(sizeof(*layer->bitmap_present)));
		layer->bitmap_present = (void*)address;
		address += round_ceil((cnt_page_total)>>i, sizeof(*layer->bitmap_present)*8);
		address = (byte*)align((usize)address, log2i(sizeof(*layer->bitmap_instack)));
		layer->bitmap_instack = (void*)address;
		address += round_ceil((cnt_page_total)>>i, sizeof(*layer->bitmap_instack)*8);
	}
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "[Done] alloc address\n");

	const info_memory mem_used_init;
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+5, &mem_used_init);
	u32 begin = (u32)mem_used_init.begin;
	u32 end = (u32)(u32)mem_used_init.end;
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"mem_used_init: %p:%p\n", begin, end);
	// Since the memory will be allocated for buddy system below,
	// the record for initially used memory will be meaningless.
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_VALIDITY,
		MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+5, false);
	DEBUG_BREAKPOINT;
	
	/*
	// Manually allocate memory for the buddy system
	// because the memory management is not available yet
	for(u32 i=(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL+0x100000);i<(u32)address;i+=4096)
	{
		u32 *const addr_PTE_kernel = &pageTable_kernel[((ADDR_STACK-4096)>>PAGE_BITWIDTH)&1023];
		u32 *const addr_PTE_init = &pageTable_init[((u32)addr_PTE_kernel>>PAGE_BITWIDTH)&1023];

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
		u32 page_begin = (u32)align((usize)mem_total[i].begin,PAGE_BITWIDTH)>>PAGE_BITWIDTH;
		u32 page_end = (u32)mem_total[i].end>>PAGE_BITWIDTH;

		KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
			"%u:%u\n",page_begin,page_end);

		// Align `page_begin` to some bound (at most up to 4K*2^CNT_LAYER = 4M)
		while(page_begin<page_end && (page_begin&((1u<<(CNT_LAYER-1))-1)))
		{
			u32 lowbit = (u32)bit_lowest(page_begin);
			if(page_begin+(1u<<lowbit)>page_end) break;
			page_begin = layer_add(lowbit,page_begin,1);
		}

		// Handle the 4M pages
		u32 cnt_page = page_end-page_begin;
		page_begin = layer_add(CNT_LAYER-1,page_begin,cnt_page>>(CNT_LAYER-1));
		cnt_page &= ((1u<<(CNT_LAYER-1))-1);

		KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
				"%u %p\n",cnt_page,page_begin<<PAGE_BITWIDTH);

		// Fill the rest of pages into the layer
		while(cnt_page)
		{
			u32 highbit = (u32)log2i(cnt_page);
			page_begin = layer_add(highbit,page_begin,1);
			cnt_page -= 1u<<highbit;
		}
	}
	return 0;
}

static usize module_init()
{
	/*
		TODO:
			get memory information
			set interrupt entry
	*/
	init_buddySystem();
	return 0;
}

static usize module_exit()
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