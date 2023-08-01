#pragma once

#include<bits/stdc++.h>

//用顺序表或者带有尾指针的链表实现
typedef int TypeElement;

class Stack
{
public:
	Stack();
	Stack(const Stack& stack);
	void StackPrint();
	void StackPush(TypeElement x);
	void CheckCapacityFull();
	void StackPop();
	TypeElement StackTop();
	bool StackEmpty();
	size_t StackSize();
private:
	TypeElement* arr;
	size_t size;
	size_t capacity;
};