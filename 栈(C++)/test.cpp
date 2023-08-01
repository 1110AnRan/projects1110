#include"Stack.h"

int main()
{
	Stack stack;
	stack.StackPush(10);
	stack.StackPush(20);
	stack.StackPush(30);
	stack.StackPush(40);
	//Stack newstack = stack;
	//newstack.StackPrint();


	stack.StackPrint();
	return 0;
}