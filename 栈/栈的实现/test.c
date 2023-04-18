#include "Stack.h"

void TestStack()
{
	Stack st;
	StackInit(&st);

	StackPush(&st, 1);
	StackPush(&st, 2);
	StackPush(&st, 3);
	StackPush(&st, 4);

	printf("%d ", StackTop(&st));
	StackPop(&st);

	printf("%d ", StackTop(&st));
	StackPop(&st);

	StackPush(&st, 5);
	StackPush(&st, 6);

	while (!StackEmpty(&st))
	{
		printf("%d ", StackTop(&st));
		StackPop(&st);
	}

	//StackPop(&st);
	//StackPop(&st);
	//StackPop(&st);
	//StackPop(&st);
	//StackPop(&st);
	//printf("%d", StackTop(&st));
}
int main()
{
	TestStack();
	return 0;
}