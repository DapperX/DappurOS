#include "assert.h"
#include "stack.h"

void stack_init(struct stack* const p, const u32 size, void *data)
{
	p->size=size, p->data=data;
}

u32 stack_size(const struct stack* const p)
{
	return p->size;
}

bool stack_empty(const struct stack* const p)
{
	return stack_size(p)==0;
}

void* stack_pop_(struct stack* const p, const u32 size_elem)
{
	KASSERT(p->size>0);
	return (byte*)(p->data)+(--p->size)*size_elem;
}

void* stack_top_(const struct stack* const p, const u32 size_elem)
{
	KASSERT(p->size>0);
	return (byte*)(p->data)+(p->size-1)*size_elem;
}

void* stack_push_(struct stack* const p, const u32 size_elem)
{
	return (byte*)(p->data)+(p->size++)*size_elem;
}