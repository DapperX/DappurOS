#include "arch/x86/page.h"
#include "memory.h"
#include "assert.h"
#include "math.h"
#include "debug.h"
#include "init_mem.h"
#include "mm.private.h"
#include "largeFrame.h"

KCALL_DISPATCH usize module_kernelCall(u32 funct);
u32 module_kernelCall_index = MODULE_TYPE_MM;
kCall_dispatch module_kernelCall_entry = module_kernelCall;

static kernelCall callList[];

u32 *const pageDirectory = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_DIRECTORY);
u32 *const pageTable_init = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_INIT);
u32 *const pageTable_kernel = (u32*)(ADDR_HIGH_MEMORY+OFFSET_PAGE_TABLE_KERNEL);

struct mm_layer list_layer[MAX_ORDER]={0};
static struct LF_manager_t LF_manager;
/*
	Memory Management Block (MMB)
	The collection of memory information on a node
*/
struct mmb_t{
	u32 cnt_page_total;
};
static struct mmb_t mmb;

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

// Return whether `index` is new in `layer`
bool layer_insert(struct mm_layer *const layer, u32 index)
{
	if(bitmap_get_value(layer->present, index)) return false;
	bitmap_set(layer->present, index);
	if(!bitmap_get_value(layer->instack, index))
	{
		stack_push(&layer->address, index, msize);
		bitmap_set(layer->instack, index);
	}
	return true;
}

u32 add_layer(const u32 order, u32 page_begin, u32 cnt)
{
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"[mm] add_layer %u %p %u\n",order,page_begin,cnt);

	struct mm_layer *layer = &list_layer[order];
	while(cnt--)
	{
		layer_insert(layer, page_begin>>order);
		page_begin += 1u<<order;
	}
	return page_begin;
}

// Label `index` within `layer` as erased
// The actual deletion will happened later in `layer_pop`
// Return whether `index` exists in `layer`
bool layer_erase(struct mm_layer *const layer, u32 index)
{
	if(!bitmap_get_value(layer->present, index)) return false;
	return bitmap_clear(layer->present, index), true;
}

// Return the index of an available area within `layer`
// If the `layer` is empty, it returns PADDR_ERR instead
msize layer_pop(struct mm_layer *const layer)
{
	while(!stack_empty(&layer->address))
	{
		msize index = stack_pop(&layer->address, msize);
		bitmap_clear(layer->instack, index);
		if(layer_erase(layer, index)) return index;
	}
	return PADDR_ERR;
}

usize buddySystem_init()
{
	const info_memory *mem_total, mem_used_init;
	const u32 cnt_mem_total = (u32)KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+4, &mem_total);
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+5, &mem_used_init);
	const u32 cnt_page_total = (u32)(mem_total[cnt_mem_total-1].end>>PAGE_BITWIDTH);

	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "buddySystem_init\n");
	u32 begin = (u32)mem_used_init.begin;
	u32 end = (u32)(u32)mem_used_init.end;
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
		"mem_used_init: %p:%p\n", begin, end);
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "cnt_page_total: %u\n", cnt_page_total);

	// Settle pointers in the layer
	// [Assumption] The memory area following the used ones is available.
	byte *address = (byte*)mem_used_init.end-ADDR_LOW_MEMORY+ADDR_HIGH_MEMORY;
	for(u32 i=0; i<MAX_ORDER; ++i)
	{
		struct mm_layer *layer = &list_layer[i];
		address = (byte*)align((usize)address, log2i(sizeof(msize)));
		stack_init(&layer->address, 0, address);
		address += (cnt_page_total>>i)*sizeof(msize);
		address = (byte*)align((usize)address, log2i(sizeof(*layer->present)));
		layer->present = (void*)address;
		address += div_ceil(cnt_page_total>>i, sizeof(*layer->present)*BITS_PER_BYTE)*sizeof(*layer->present);
		address = (byte*)align((usize)address, log2i(sizeof(*layer->instack)));
		layer->instack = (void*)address;
		address += div_ceil(cnt_page_total>>i, sizeof(*layer->instack)*BITS_PER_BYTE)*sizeof(*layer->instack);
	}
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "[Done] alloc address for BS\n");

	// The contiguous frames with number larger than 2^MAX_ORDER is managered by largeFrame
	address = (byte*)align((usize)address, log2i(sizeof(usize)));
	void *const addr_LF_extra = address;
	address += LF_init(NULL, NULL, (LF_idx_t)(cnt_page_total>>MAX_ORDER));
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8, "[Done] alloc address for LF\n");
	DEBUG_BREAKPOINT;
	/*
	// Since the memory will be allocated for buddy system below,
	// the record for initially used memory will be meaningless.
	KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_VALIDITY,
		MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+5, false);
	*/
	
	// Manually allocate memory for the memory manager itself
	// because the memory management is not available yet
	// [Assumption] mem_used_init.end is aligned to pages
	u32 offset_used = (u32)(address - ADDR_HIGH_MEMORY);
	byte *paddr_pageTable = (byte*)align((usize)address-ADDR_HIGH_MEMORY, PAGE_BITWIDTH);
	for(u32 offset=(u32)mem_used_init.end-ADDR_LOW_MEMORY; offset<offset_used; offset+=PAGE_SIZE)
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
		*addr_PTE_kernel = (0x100000+offset)|PTE_P|PTE_R;
	}

	for(u32 i=0;i<cnt_mem_total;++i)
	{
		u32 page_begin = (u32)align((usize)mem_total[i].begin,PAGE_BITWIDTH)>>PAGE_BITWIDTH;
		u32 page_end = (u32)mem_total[i].end>>PAGE_BITWIDTH;

		KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
			"%u:%u\n",page_begin,page_end);

		// Align `page_begin` to some bound (at most up to 4K*2^MAX_ORDER = 4M)
		while(page_begin<page_end && (page_begin&((1u<<(MAX_ORDER))-1)))
		{
			u32 lowbit = (u32)bit_lowest(page_begin);
			if(page_begin+(1u<<lowbit)>page_end) break;
			page_begin = add_layer(lowbit,page_begin,1);
		}

		// Handle the 4M pages
		LF_init(&LF_manager, addr_LF_extra, (LF_idx_t)(cnt_page_total>>MAX_ORDER));
		LF_modify_range(&LF_manager, (LF_idx_t)(page_begin>>MAX_ORDER), (LF_idx_t)(page_end>>MAX_ORDER), 1);

		u32 cnt_page = page_end - page_begin;
		cnt_page &= ((1u<<(MAX_ORDER))-1);
		KCALL(MODULE_TYPE_CONTROL, KERNEL_CALL_SELF_DEFINED+8,
				"%u %p\n",cnt_page,page_begin<<PAGE_BITWIDTH);

		// Fill the rest of pages into the layer
		while(cnt_page)
		{
			u32 highbit = (u32)log2i(cnt_page);
			page_begin = add_layer(highbit,page_begin,1);
			cnt_page -= 1u<<highbit;
		}
	}

	mmb.cnt_page_total = cnt_page_total;
	return 0;
}

msize largeFrame_pop(u32 order)
{
	const LF_len_t cnt_LF_require = (LF_len_t)(1u<<(order-MAX_ORDER));
	const LF_len_t cnt_LF_max = LF_get_max(&LF_manager, 0, (LF_idx_t)(mmb.cnt_page_total>>MAX_ORDER));
	if(cnt_LF_max<cnt_LF_require) return PADDR_ERR;
	const LF_idx_t pos=LF_get_pos_overall(&LF_manager, (LF_len_t)cnt_LF_require);
	if(pos==-1) return PADDR_ERR;
	LF_modify_range(&LF_manager, pos, (LF_idx_t)(pos+cnt_LF_require), 0);
	return pos==-1?PADDR_ERR:(msize)pos;
}

void largeFrame_push(msize paddr, u32 order)
{
	const LF_idx_t pos=(LF_idx_t)paddr;
	LF_modify_range(&LF_manager, pos, (LF_idx_t)(pos+(1<<(order-MAX_ORDER))), 1);
}

/*
	Allocate a continuous physical memory area in size of `cnt_frame`*PAGE_SIZE
	and return the physical address of the area (aligned to PAGE_SIZE)
	If the allocation fails, it returns PADDR_ERR.
	Notice that PADDR_ERR is never possible as a result of physical frame allocation.
*/
msize buddySystem_allocate(u32 order)
{
	if(order>=MAX_ORDER)
	{
		const msize res=largeFrame_pop(order);
		return res!=PADDR_ERR?res<<MAX_ORDER<<PAGE_BITWIDTH:PADDR_ERR;
	}

	// Find a layer which at least contains a area not deleted
	u32 order_ex = order;
	msize paddr = PADDR_ERR;
	for(; order_ex<MAX_ORDER; ++order_ex)
	{
		// Get the target phsyical address
		paddr = layer_pop(&list_layer[order_ex]);
		if(paddr!=PADDR_ERR) break;
	}
	if(paddr==PADDR_ERR)
	{
		order_ex = MAX_ORDER;
		paddr = largeFrame_pop(order_ex);
	}
	// There is no area matching the given size of frames
	if(paddr==PADDR_ERR) return PADDR_ERR;

	// Replenish the rest of area to the lower layers
	for(u32 i=order_ex; i>order; --i)
	{
		paddr <<= 1;
		KASSERT(layer_insert(&list_layer[i-1], paddr^1));
	}
	return paddr<<order<<PAGE_BITWIDTH;
}

void buddySystem_free(msize paddr, u32 order)
{
	// Make sure the given `paddr` is available
	KASSERT(!(paddr&((1u<<order)-1)));
	paddr >>= (order+PAGE_BITWIDTH);
	if(order>=MAX_ORDER)
	{
		largeFrame_push(paddr, order);
		return;
	}

	for(; order<MAX_ORDER; ++order, paddr>>=1)
	{
		struct mm_layer *const layer = &list_layer[order];
		KASSERT(!bitmap_get_value(layer->present, paddr));
		if(!layer_erase(layer, paddr^1)) break;
	}

	if(order==MAX_ORDER)
		largeFrame_push(paddr, order);
	else
		KASSERT(layer_insert(&list_layer[order], paddr));
}

usize module_init()
{
	/*
		TODO:
			get memory information
			set interrupt entry
	*/
	buddySystem_init();
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
