#include "mm.h"
#include "macro.h"
#include "arch/x86/page.h"
#include "assert.h"
#include "debug.h"

kernelCall *const KCT = (kernelCall*)(OFFSET_KCT+ADDR_HIGH_MEMORY);
static kernelCall_noarg callList[];

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
	KCT[KERNEL_CALL_INIT](KERNEL_CALL_SELF_DEFINED+8,
		"[mm] layer_add %u %p %u\n",index,page_begin,cnt);

	struct mm_layer *layer = &list_layer[index];
	while(cnt--)
	{
	//	layer->stack[layer->top_stack++] = page_begin<<12;
	//	layer->bitmap_stack[page_begin>>index>>3] |= 1u<<((page_begin>>index)&7);
		page_begin += 1u<<index;
	}
	return page_begin;
}

uint_var init_buddySystem()
{
	const info_memory *mem_total;
	const u32 cnt_mem_total = (u32)KCT[KERNEL_CALL_INIT](KERNEL_CALL_SELF_DEFINED+4,&mem_total);
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

	for(u32 i=0;i<cnt_mem_total;++i)
	{
		u32 page_begin = align((u32)mem_total[i].begin,12)>>12;
		u32 page_end = (u32)mem_total[i].end>>12;
		KCT[KERNEL_CALL_INIT](KERNEL_CALL_SELF_DEFINED+8,
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

uint_var module_init()
{
	/*
		TODO:
			get memory information
			set interrupt entry
	*/
	init_buddySystem();
	return 0;
}

uint_var module_exit()
{
	return 0;
}

static kernelCall_noarg callList[]={
	[KERNEL_CALL_INIT]=(kernelCall_noarg)module_init,
	[KERNEL_CALL_EXIT]=(kernelCall_noarg)module_exit,
};

void module_kernelCall(u32 index,...)
{
	KASSERT(index<LEN_ARRAY(callList));
	KASSERT(callList[index]);
	TEMPLATE_CALL_DISTRIBUTE(callList);
}