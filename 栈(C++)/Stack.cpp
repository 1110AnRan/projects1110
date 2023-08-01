#include"Stack.h"

Stack::Stack()
{
	size = 0;
	capacity = 1;
	arr = new TypeElement[capacity];
}

Stack::Stack(const Stack& stack)
{
	size = stack.size;
	capacity = stack.capacity;
	if (arr)
	{
		delete[]arr;
		arr = nullptr;
	}
	arr = new TypeElement[capacity];
	memcpy(arr, stack.arr, sizeof(TypeElement) * size);
}

void Stack::StackPrint()
{
	assert(size);

	int i = 0;
	for (i = size - 1; i >= 0; i--)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

void Stack::StackPush(TypeElement x)
{
	assert(arr);

	CheckCapacityFull();
	arr[size] = x;
	size++;
}

void Stack::CheckCapacityFull()
{
	assert(arr);
	if (size == capacity)
	{
		size_t newcapacity = capacity * 2;
		TypeElement* tmp = new TypeElement[newcapacity];
		memcpy(tmp, arr, sizeof(TypeElement) * capacity);
		delete[]arr;
		arr = tmp;
	}
}

void Stack::StackPop()
{
	assert(size);

	arr[size - 1] = -1;
	size--;
}

TypeElement Stack::StackTop()
{
	return arr[size - 1];
}

bool Stack::StackEmpty()
{
	if (size == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Stack::StackSize()
{
	return size;
}


