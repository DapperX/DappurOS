#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <unistd.h>
#include "time.h"
#include "base.h"
#include "mm.private.h"

#define SIZE_MEM (32*0x100000)
#define CNT_FRAME (SIZE_MEM>>12)
#define CNT_FRAME_INIT (200<<MAX_ORDER)
#define Q 300

struct frame{
	msize addr;
	u32 order;
}request[Q];

byte mem_status[CNT_FRAME];

void halt(const char *s)
{
	puts("HALT!");
	puts(s);
	exit(-1);
}

static void init(unsigned seed)
{
	if(!seed) seed = (unsigned)(time(NULL)^getpid());
	printf("seed: %d\n", seed);
	srand(seed);
	// buddySystem_init();
	for(u32 i=0; i<=MAX_ORDER; ++i)
	{
		struct mm_layer *layer = &list_layer[i];
		stack_init(&layer->address, 0, calloc(1, sizeof(msize)*CNT_FRAME));
		layer->present = calloc(1, CNT_FRAME);
		layer->instack = calloc(1, CNT_FRAME);
	}
}

static void cleanup()
{
	for(u32 i=0; i<=MAX_ORDER; ++i)
	{
		struct mm_layer *layer = &list_layer[i];
		free(layer->address.data);
		free(layer->present);
		free(layer->instack);
	}
}

// Return the number of available frames
u32 get_size_real()
{
	u32 cnt_available=0;
	for(u32 i=0; i<CNT_FRAME_INIT; ++i)
		if(!(mem_status[i]&1)) cnt_available++;
	return cnt_available;
}

// Return the number of available frames
u32 get_size_test()
{
	u32 cnt_available=0;
	for(u32 i=0; i<=MAX_ORDER; ++i)
	{
		const struct mm_layer *const layer = &list_layer[i];
		for(u32 j=0; j<layer->address.size; ++j)
		{
			msize index = ((msize*)layer->address.data)[j];
			if(bitmap_get_value(layer->present, index))
				cnt_available += 1u<<i;
		}
	}
	return cnt_available;
}

bool check_available(const msize addr, const u32 order)
{
	const u32 size = 1u<<order;
	if(addr+size>CNT_FRAME_INIT) halt("fail to check available");
	for(u32 i=0; i<size; ++i)
		if(mem_status[addr+i]&1) return false;
	return true;
}

bool check_existent(const u32 order)
{
	const u32 size = 1u<<order;
	for(msize i=0; i<CNT_FRAME_INIT; i+=size)
		if(check_available(i, order)) return true;
	return false;
}

void set_status(const msize addr, const u32 order, byte status)
{
	const u32 size = 1u<<order;
	if(addr+size>CNT_FRAME_INIT) halt("fail to check available");
	for(u32 i=0; i<size; ++i)
		mem_status[addr+i] = status;
}

void run_test()
{
	for(u32 i=0; i<CNT_FRAME_INIT>>MAX_ORDER; ++i)
	{
		bool ret = layer_insert(&list_layer[MAX_ORDER], i);
		if(!ret) halt("fail to insert");
	}

	for(u32 i=0; i<Q;)
	{
		u32 order = (u32)rand()%MAX_ORDER;
		msize addr = buddySystem_allocate(order);
		if(addr==PADDR_ERR)
		{
			if(check_existent(order)) halt("fail to alloc");
			continue;
		}
		if(!check_available(addr, order)) halt("false alloc");
		set_status(addr, order, 1);
		request[i++] = (struct frame){addr, order};
	}

	u32 page_size_real = get_size_real();
	u32 page_size_calc = CNT_FRAME_INIT;
	for(u32 i=0; i<Q; ++i)
		page_size_calc -= 1u<<request[i].order;
	u32 page_size_test = get_size_test();

	printf("page_size_real: %u\n", page_size_real);
	printf("page_size_calc: %u\n", page_size_calc);
	printf("page_size_test: %u\n", page_size_test);

	puts("------ finish allocation ------");

	for(u32 i=0; i<Q; ++i)
	{
		buddySystem_free(request[i].addr, request[i].order);
	}
	printf("page_size_real: %u\n", CNT_FRAME_INIT);
	printf("page_size_test: %u\n", get_size_test());
	struct mm_layer *const layer = &list_layer[MAX_ORDER];
	msize paddr=PADDR_ERR;
	u32 cnt_frame=0;
	do{
		paddr = layer_pop(layer);
		cnt_frame++;
		printf("index: %u\n", paddr<<MAX_ORDER);
	}while(paddr!=PADDR_ERR);
	if(cnt_frame!=(CNT_FRAME_INIT>>MAX_ORDER)+1) halt("fail to free");
}

int main(int argc, char **argv)
{
	init((unsigned)(argc>1?atoi(argv[1]):0));
	run_test();
	cleanup();
	return 0;
}