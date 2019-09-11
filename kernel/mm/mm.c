#include "mm.h"
#include "arch/x86/page.h"
#include "memory.h"
#include "assert.h"
#include "math.h"
#include "debug.h"
#include "init_mem.h"
#include "bitmap.h"

static kernelCall callList[];

u32 *const pageDirectory = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_DIRECTORY);
u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);

#define CNT_LAYER 11
struct mm_layer{
	u32 stack_top;
	u32 *stack;
	u32 *bitmap_present;
	u32 *bitmap_instack;
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

u32 layer_add(const u32 order, u32 page_begin, u32 cnt)
{
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"[mm] layer_add %u %p %u\n",order,page_begin,cnt);

	struct mm_layer *layer = &list_layer[order];
	while(cnt--)
	{
		layer->stack[layer->stack_top++] = page_begin<<PAGE_BITWIDTH;
		layer->bitmap_present[page_begin>>order>>3] |= 1u<<((page_begin>>order)&7);
		layer->bitmap_instack[page_begin>>order>>3] |= 1u<<((page_begin>>order)&7);
		page_begin += 1u<<order;
	}
	return page_begin;
}

usize buddySystem_init()
{
	const info_memory *mem_total, mem_used_init;
	const u32 cnt_mem_total = (u32)KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+4, &mem_total);
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+5, &mem_used_init);
	u32 cnt_page_total = (u32)(mem_total[cnt_mem_total-1].end>>PAGE_BITWIDTH);

	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "buddySystem_init\n");
	u32 begin = (u32)mem_used_init.begin;
	u32 end = (u32)(u32)mem_used_init.end;
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"mem_used_init: %p:%p\n", begin, end);
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "cnt_page_total: %u\n", cnt_page_total);

	// Settle pointers in the layer
	// [Assumption] The memory area following the used ones is available.
	byte *address = (byte*)mem_used_init.end-ADDR_LOW_MEMORY+ADDR_HIGH_MEMORY;
	for(u32 i=0;i<CNT_LAYER;++i)
	{
		struct mm_layer *layer = &list_layer[i];
		layer->stack_top = 0;
		address = (byte*)align((usize)address, log2i(sizeof(*layer->stack)));
		layer->stack = (void*)address;
		address += (cnt_page_total>>i)*sizeof(*layer->stack);
		address = (byte*)align((usize)address, log2i(sizeof(*layer->bitmap_present)));
		layer->bitmap_present = (void*)address;
		address += div_ceil(cnt_page_total>>i, sizeof(*layer->bitmap_present)*8)*sizeof(*layer->bitmap_present);
		address = (byte*)align((usize)address, log2i(sizeof(*layer->bitmap_instack)));
		layer->bitmap_instack = (void*)address;
		address += div_ceil(cnt_page_total>>i, sizeof(*layer->bitmap_instack)*8)*sizeof(*layer->bitmap_instack);
	}
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "[Done] alloc address\n");

	/*
	// Since the memory will be allocated for buddy system below,
	// the record for initially used memory will be meaningless.
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_VALIDITY,
		MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+5, false);
	*/
	DEBUG_BREAKPOINT;
	
	// Manually allocate memory for the buddy system
	// because the memory management is not available yet
	// [Assumption] mem_used_init.end is aligned to pages
	u32 offset_layer = (u32)(address - ADDR_HIGH_MEMORY);
	byte *paddr_pageTable = (byte*)align((usize)address, PAGE_BITWIDTH);
	for(u32 offset=(u32)mem_used_init.end-ADDR_LOW_MEMORY; offset<offset_layer; offset+=PAGE_SIZE)
	{
		u32 *const addr_PDE = &pageDirectory[(offset+ADDR_HIGH_MEMORY)>>PAGE_BITWIDTH>>PGTBL_BITWIDTH];
		u32 *const addr_PTE_kernel = &pageTable_kernel[offset>>PAGE_BITWIDTH];
		u32 *const addr_PTE_init = &pageTable_init[((u32)addr_PTE_kernel>>PAGE_BITWIDTH)&PGTBL_MASK];

		if(!(*addr_PDE&PDE_P))
		{
			*addr_PDE = (u32)paddr_pageTable|PDE_P|PDE_R;
			KASSERT(!(*addr_PTE_init&PTE_P));
			*addr_PTE_init = (u32)paddr_pageTable|PTE_P|PTE_R;
			paddr_pageTable += PAGE_SIZE;
			kmemset((void*)((usize)addr_PTE_kernel&~PAGE_MASK), 0, PAGE_SIZE);
		}
		*addr_PTE_kernel = (ADDR_LOW_MEMORY+offset)|PTE_P|PTE_R;
	}

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

/*
	Allocate a continuous physical memory area in size of `cnt_frame`*PAGE_SIZE
	and return the physical address of the area (aligned to PAGE_SIZE)
	If the allocation fails, it returns NULL.
	Notice that NULL is never possible as a result of physical frame allocation.
*/
void *buddySystem_allocate(u32 cnt_frame)
{
	u32 cnt_frame_aligned = log2i(cnt_frame);
	if(cnt_frame_aligned<cnt_frame) cnt_frame_aligned<<=1;
	// Detect the overflow
	KASSERT(cnt_frame_aligned>=cnt_frame);

	u32 order = bit_lowest(cnt_frame_aligned);
	if(order>CNT_LAYER-1)
	{
		order = CNT_LAYER-1;
		u32 cnt_block = cnt_frame_aligned>>order;
		// Unimplemented
		// Search continuous ones in bitmap
	}
	else
	{
		struct mm_layer *layer = &list_layer[order];
		/*
		struct mm_layer{
			u32 stack_top;
			u32 *stack;
			usize *bitmap_present;
			usize *bitmap_instack;
		};
		*/
		while(layer->stack_top>0)
		{
			u32 paddr = layer->stack[layer->stack_top-1];
			//if(layer->bitmap_present[paddr>>order])
		}
	}
}

/*
	Return the result whether the operation is successful.
	Zero means the success while otherwise means the error number.
*/
bool buddySystem_free(u32 paddr, u32 cnt_frame)
{
}

static usize module_init()
{
	/*
		TODO:
			get memory information
			set interrupt entry
	*/
	buddySystem_init();
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
