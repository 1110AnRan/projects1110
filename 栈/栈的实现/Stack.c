#include "Stack.h"

void StackInit(Stack* ps)
{
	assert(ps);

	ps->a = NULL;
	ps->top = ps->capacity = 0;
}

void StackPush(Stack* ps, STDataType x)
{
	assert(ps);
	
	if (ps->top == ps->capacity)
	{
		STDataType newcapacity = ps->capacity == 0 ? 4 : ps->capacity * 2;
		STDataType* tmp = realloc(ps->a, newcapacity*sizeof(STDataType));
		if (tmp == NULL)
		{
			printf("Realloc Failed\n");
			exit(-1);
		}
		else
		{
			ps->capacity = newcapacity;
			ps->a = tmp;
		}
	}
	ps->a[ps->top] = x;
	ps->top++;

}

void StackPop(Stack* ps)
{
	assert(ps);
	assert(ps->top > 0);
	ps->top--;
}

STDataType StackTop(Stack* ps)
{
	assert(ps);
	assert(ps->top > 0);
	return ps->a[ps->top - 1];
}

int StackSize(Stack* ps)
{
	assert(ps);

	return ps->top;
}

bool StackEmpty(Stack* ps)
{
	assert(ps);

	return ps->top == 0;
}

void StackDestroy(Stack* ps)
{
	assert(ps);
	free(ps->a);
	ps->a = NULL;
	ps->capacity = ps->top = 0;
}