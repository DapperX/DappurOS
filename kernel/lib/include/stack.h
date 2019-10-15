#ifndef _STACK_H
#define _STACK_H

struct stack
{
	u32 size;
	void *data;
};


void stack_init(struct stack* const p, const u32 size, void *data);
u32 stack_size(const struct stack* const p);
bool stack_empty(const struct stack* const p);

void* stack_pop_(struct stack* const p, const u32 size_elem);
#define stack_pop(p, type_elem)\
	*(type_elem*)stack_pop_((p), sizeof(type_elem))

void* stack_top_(const struct stack* const p, const u32 size_elem);
#define stack_top(p, type_elem)\
	*(type_elem*)stack_top_((p), sizeof(type_elem))

void* stack_push_(struct stack* const p, const u32 size_elem);
#define stack_push(p, elem, type_elem)\
	*(type_elem*)stack_push_((p), sizeof(type_elem)) = elem

#endif //_STACK_H