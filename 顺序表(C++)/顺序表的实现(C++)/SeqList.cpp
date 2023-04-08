#define _CRT_SECURE_NO_WARNINGS 1
#include "SeqList.h"

SeqList::SeqList()
{
	size = 0;
	capacity = 4;
	arr = new TypeElement[capacity];
}

SeqList::SeqList(size_t capacity)
{
	this->capacity = capacity;
	size = 0;
	arr = new TypeElement[capacity];
}

SeqList::SeqList(const SeqList& plist)
{
	capacity = plist.capacity;
	size = plist.size;

	arr = new TypeElement[capacity];

}

SeqList::~SeqList()
{
	if (arr != NULL)
	{
		delete[]arr;
		arr = NULL;
	}
}



void SeqList::SeqListPrint()
{
	assert(size);

	int i = 0;
	for (i = 0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void SeqList::SeqListCheckCapacity()
{
	assert(arr);

	if (size == capacity)
	{
		size_t newcapacity = capacity * 2;
		TypeElement* temp = new TypeElement[newcapacity];
		memcpy(temp, arr, sizeof(TypeElement) * size);
		delete[]arr;
		arr = temp;
		capacity = newcapacity;
	}
}

void SeqList::SeqListPushBack(TypeElement x)
{
	assert(arr);

	SeqListCheckCapacity();

	arr[size] = x;
	size++;
}

void SeqList::SeqListPopBack()
{
	assert(size);

	arr[size-1] = 0;
	size--;
}

void SeqList::SeqListPushFront(TypeElement x)
{
	assert(arr);

	SeqListCheckCapacity();

	size_t i = 0;
	for (i = size; i > 0; i--)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = x;
	size++;

}

void SeqList::SeqListPopFront()
{
	assert(size);

	int i = 0;
	for (i = 0; i < size - 1; i++)
	{
		arr[i] = arr[i + 1];
	}
	arr[size - 1] = 0;
	size--;

}

int SeqList::SeqListFind(TypeElement begin,TypeElement x)
{
	assert(size);
	assert(arr);

	int i = 0;
	for (i = begin; i < size; i++)
	{
		if (arr[i] == x)
		{
			return i;
		}
	}
	return -1;
}

void SeqList::SeqListInsert(size_t pos, TypeElement x)
{
	assert(size);

	SeqListCheckCapacity();
	size_t i = 0;
	for (i = size ; i > pos; i--)
	{
		arr[i] = arr[i - 1];
	}
	arr[pos] = x;
	size++;
	
}

void SeqList::SeqListErase(size_t pos)
{
	assert(size);

	size_t i = 0;
	for (i = pos; i < size; i++)
	{
		arr[pos] = arr[pos + 1];
	}

	arr[size] = 0;
	size--;
}

