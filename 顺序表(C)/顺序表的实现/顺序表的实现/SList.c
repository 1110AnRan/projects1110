#include "SList.h"


void SeqListInit(SeqList* ps)
{
	ps->arr = NULL;
	ps->size = ps->capacity = 0;
}

void SeqListPrint(SeqList* ps)
{
	int i = 0;
	for (i = 0; i < ps->size; i++)
	{
		printf("%d ", ps->arr[i]);
	}
	printf("\n");
}

void SeqListDestory(SeqList* ps)
{
	free(ps->arr);
	ps->size = ps->capacity = 0;
}

void SeqListCheckCapacity(SeqList* ps)
{
	if (ps->size == ps->capacity)
	{
		int newcapacity = ps->capacity == 0 ? 4 : ps->capacity * 2;
		SLDataType* tmp = (SLDataType*)realloc(ps->arr, newcapacity * sizeof(SLDataType));
		if (tmp == NULL)
		{
			printf("Malloc Fail\n");
			exit(-1);
		}
		ps->arr = tmp;
		ps->capacity = newcapacity;
	}
}

void SeqListPushBack(SeqList* ps, SLDataType x)
{	
	SeqListCheckCapacity(ps);
	ps->arr[ps->size] = x;
	ps->size++;
}

void SeqListPopBack(SeqList* ps)
{
	assert(ps->size > 0);
	ps->size--;
}

void SeqListPushFront(SeqList* ps, SLDataType x)
{
	SeqListCheckCapacity(ps);
	int end = ps->size;
	while (end)
	{
		ps->arr[end] = ps->arr[end - 1];
		end--;
	}
	ps->arr[end] = x;
	ps->size++;
}

void SeqListPopFront(SeqList* ps)
{
	assert(ps->size);
	int start = 1;
	while (start <= ps->size)
	{
		ps->arr[start - 1] = ps->arr[start];
		start++;
	}
	ps->size--;
}

int SeqListFind(SeqList* ps, SLDataType x)
{
	assert(ps->size);
	int i = 0;
	for (i = 0; i < ps->size; i++)
	{
		if (ps->arr[i] == x)
		{
			return i;
		}
	}
	return -1;
}

void SeqListInsert(SeqList* ps, size_t pos, SLDataType x)
{
	assert(pos >= 0 && pos < ps->size);
	SeqListCheckCapacity(ps);
	int end = ps->size;
	while (end >= pos)
	{
		ps->arr[end] = ps->arr[end - 1];
		end--;
	}
	ps->arr[end] = x;
	ps->size++;
}

void SeqListErase(SeqList* ps, size_t pos)
{
	assert(ps->size && pos >= 0 && pos < ps->size);
	int begin = pos;
	while (begin < ps->size)
	{
		ps->arr[begin] = ps->arr[begin + 1];
		begin++;
	}
	ps->size--;
}