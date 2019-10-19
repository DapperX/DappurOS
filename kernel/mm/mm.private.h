#ifndef _MM_PRIVATE_H
#define _MM_PRIVATE_H

#include "mm.h"
#include "bitmap.h"
#include "stack.h"

#define MAX_ORDER 10
struct mm_layer{
	struct stack address;
	bitmap_item *present;
	bitmap_item *instack;
};

// expose inner functions for testing or debugging
#ifndef NDEBUG
extern struct mm_layer list_layer[MAX_ORDER];

usize module_init();
usize module_exit();

bool layer_insert(struct mm_layer *const layer, u32 index);
msize layer_pop(struct mm_layer *const layer);

usize buddySystem_init();
msize buddySystem_allocate(u32 order);
void buddySystem_free(msize paddr, u32 order);
#endif

#endif //_MM_PRIVATE_H