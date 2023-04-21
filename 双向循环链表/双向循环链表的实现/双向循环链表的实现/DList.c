#include "DList.h"

LTNode* ListInit()
{
	LTNode* phead = (LTNode*)malloc(sizeof(LTNode));
	if (phead == NULL)
	{
		printf("ListInit Failed\n");
		exit(-1);
	}
	else
	{
		phead->prev = phead;
		phead->next = phead;
	}
	
	return phead;
}

LTNode* BuyDListNode(LTDataType x)
{
	LTNode* newnode = (LTNode*)malloc(sizeof(LTNode));
	if (newnode == NULL)
	{
		printf("Malloc Failed\n");
		exit(-1);
	}
	newnode->data = x;
	newnode->next = NULL;
	newnode->prev = NULL;

	return newnode;
}
void ListPrint(LTNode* phead)
{
	assert(phead);

	LTNode* tail = phead->next;
	while (tail != phead)
	{
		printf("%d->", tail->data);
		tail = tail->next;
	}

	printf("The List is restart\n");
}

void ListPushBack(LTNode* phead, LTDataType x)
{
	assert(phead);

	//LTNode* newnode = BuyDListNode(x);
	//LTNode* tail = phead->prev;
	//tail->next = newnode;
	//newnode->prev = tail;
	//newnode->next = phead;
	//phead->prev = newnode;
	ListInsert(phead, x);

}

void ListPopBack(LTNode* phead)
{
	assert(phead);
	assert(phead != phead->next);

	//LTNode* tail = phead->prev;
	//tail->prev->next = phead;
	//phead->prev = tail->prev;
	//free(tail);
	ListErase(phead->prev);

}

void ListPushFront(LTNode* phead, LTDataType x)
{
	assert(phead);

	//LTNode* newnode = BuyDListNode(x);
	//newnode->next = phead->next;
	//newnode->prev = phead;
	//phead->next->prev = newnode;
	//phead->next = newnode;

	ListInsert(phead->next, x);

}

void ListPopFront(LTNode* phead)
{
	assert(phead);
	assert(phead->next != phead);

	//LTNode* prev = phead->next;
	//phead->next = prev->next;
	//prev->next->prev = phead;
	//free(prev);
	ListErase(phead->next);

}

LTNode* ListFind(LTNode* phead, LTDataType x)
{
	assert(phead);

	LTNode* cur = phead->next;
	while (cur != phead)
	{
		if (cur->data == x)
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

void ListInsert(LTNode* pos, LTDataType x)
{
	assert(pos);

	LTNode* newnode = BuyDListNode(x);
	pos->prev->next = newnode;
	newnode->prev = pos->prev;
	newnode->next = pos;
	pos->prev = newnode;

}

void ListErase(LTNode* pos)
{
	assert(pos);

	pos->next->prev = pos->prev;
	pos->prev->next = pos->next;
	free(pos);
	pos = NULL;

}

void ListDestroy(LTNode* phead)
{
	assert(phead);

	LTNode* cur = phead->next;
	while (cur != phead)
	{
		LTNode* next = cur->next;
		free(cur);
		cur = cur->next;
	}

	free(phead);
	phead = NULL;
}

