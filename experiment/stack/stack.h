#ifndef _STACK_H
#define _STACK_H

typedef unsigned char byte;

struct stack
{
	int size;
	void *data;
};


int stack_size(const struct stack* const p)
{
	return p->size;
}
int stack_empty(const struct stack* const p)
{
	return stack_size(p)==0;
}

void* stack_pop_(struct stack* const p, const unsigned size_elem)
{
	return (byte*)(p->data)+(--p->size)*size_elem;
}
#define stack_pop(p, type_elem)\
	*(type_elem*)stack_pop_((p), sizeof(type_elem))

void* stack_top_(const struct stack* const p, const unsigned size_elem)
{
	return (byte*)(p->data)+(p->size-1)*size_elem;
}
#define stack_top(p, type_elem)\
	*(type_elem*)stack_top_((p), sizeof(type_elem))

void* stack_push_(struct stack* const p, const unsigned size_elem)
{
	return (byte*)(p->data)+(p->size++)*size_elem;
}
#define stack_push(p, elem, type_elem)\
	*(type_elem*)stack_push_((p), sizeof(type_elem)) = elem

#endif //_STACK_H