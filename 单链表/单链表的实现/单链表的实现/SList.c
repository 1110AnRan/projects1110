#include "SList.h"

//定义函数

SLTNode* BuySListNode(SLTDateType x)
{
	SLTNode* newnode = (SLTNode*)malloc(sizeof(SLTNode));
	if (newnode == NULL)
	{
		printf("Malloc Fail\n");
		exit(-1);
	}
	newnode->data = x;
	newnode->next = NULL;

	return newnode;
}

void SListPrint(SLTNode* phead)
{
	SLTNode* cur = phead;
	
	while (cur)
	{
		printf("%d->", cur->data);
		cur = cur->next;
	}

	printf("NULL\n");
}

void SListPushBack(SLTNode** pphead, SLTDateType x)
{
	SLTNode* newnode = BuySListNode(x);
	if (*pphead == NULL)
	{
		*pphead = newnode;
	}
	
	else
	{
		SLTNode* tail = *pphead;
		while (tail->next)
		{
			tail = tail->next;
		}
		tail->next = newnode;
	}
}

void SListPushFront(SLTNode** pphead, SLTDateType x)
{
	SLTNode* newnode = BuySListNode(x);

	newnode->next = *pphead;
	*pphead = newnode;
}

void SListPopBack(SLTNode** pphead)
{

	assert(*pphead);
	if ((*pphead)->next == NULL)
	{
		free(*pphead);
		*pphead = NULL;
	}
	else
	{

		SLTNode* tail = *pphead;
		SLTNode* prev = NULL;
		//找到尾节点
		while (tail->next)
		{
			prev = tail;
			tail = tail->next;
		}
		free(tail);
		tail = NULL;
		prev->next = NULL;
	}
}

void SListPopFront(SLTNode** pphead)
{
	assert(*pphead);
	if ((*pphead)->next == NULL)
	{
		free(*pphead);
		*pphead = NULL;
	}
	else
	{
		SLTNode* head = *pphead;
		*pphead = head->next;
		free(head);
		head = NULL;
	}
}

SLTNode* SListFind(SLTNode* phead, SLTDateType x)
{
	SLTNode* cur = phead;
	while (cur)
	{
		if (cur->data == x)
		{
			return cur;
		}
		cur = cur->next;
	}
	printf("该链表已无%d值\n", x);
	return NULL;
}

void SListInsertAfter(SLTNode* pos, SLTDateType x)
{
	SLTNode* newnode = BuySListNode(x);
	newnode->next = pos->next;
	pos->next = newnode;
}

void SListInsert(SLTNode** pphead, SLTNode* pos, SLTDateType x)
{
	assert(*pphead);
	SLTNode* newnode = BuySListNode(x);
	if (*pphead == pos)
	{
		newnode->next = *pphead;
		*pphead = newnode;
	}
	SLTNode* posPrev = *pphead;
	while (posPrev)
	{
		if (posPrev->next == pos)
		{
			posPrev->next = newnode;
			newnode->next = pos;
			break;
		}
		posPrev = posPrev->next;
	}
}


void SListEraseFront(SLTNode** pphead, SLTNode* pos)
{
	//头删
	if ((*pphead) == pos)
	{
		SLTNode* head = *pphead;
		*pphead = head->next;
		free(head);
		head = NULL;
	}

	else
	{
		SLTNode* posPrev = *pphead;
		while (posPrev->next != pos)
		{
			posPrev = posPrev->next;
		}
		posPrev->next = pos->next;
		free(pos);
		pos = NULL;
	}
}

void SListEraseAfter(SLTNode* pos)
{
	assert(pos->next);

	SLTNode* next = pos->next;
	pos->next = next->next;
	free(next);
	next = NULL;
}

void SListDestroy(SLTNode** pphead)
{
	assert(*pphead);
	
	SLTNode* cur = *pphead;
	while (cur)
	{
		SLTNode* next = cur->next;
		free(cur);
		cur = next;
	}

	*pphead = NULL;
}

