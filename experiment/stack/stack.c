#include "stdio.h"
#include "stack.h"

int pool[100];

int main()
{
	struct stack a;
	a.size = 0;
	a.data = pool;
	stack_push(&a, 1, int);
	int y = 5;
	stack_push(&a, y, int);
	printf("size: %d\n", stack_size(&a));
	while(!stack_empty(&a))
	{
		printf("%d\n", stack_top(&a, int));
		printf("%d\n", stack_pop(&a, int));
	}
	return 0;
}