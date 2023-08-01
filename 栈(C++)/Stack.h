#pragma once

#include<bits/stdc++.h>

//��˳�����ߴ���βָ�������ʵ��
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