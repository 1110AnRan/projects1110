#include "Queue.h"


void QueueInit(Queue* q)
{
	assert(q);
	q->head = q->tail = NULL;

}

void QueuePush(Queue* q, QDataType x)
{
	assert(q);

	QNode* newnode = (QNode*)malloc(sizeof(QNode));
	if (newnode == NULL)
	{
		printf("Malloc Failed\n");
		exit(-1);
	}
	else
	{
		newnode->next = NULL;
		newnode->data = x;
		if (q->head == q->tail && q->head == NULL)
		{
			q->head = q->tail = newnode;
		}
		else
		{
			q->tail->next = newnode;
			q->tail = q->tail->next;
		}
	}

}

void QueueDestroy(Queue* q)
{
	assert(q);
	
	QNode* cur = q->head;
	while (cur)
	{
		QNode* next = cur->next;
		free(cur);
		cur = next;
	}

	q->head = q->tail = NULL;
}

void QueuePop(Queue* q)
{
	assert(q);
	assert(!QueueEmpty(q));

	QNode* cur = q->head;
	if (q->head == q->tail)
	{
		q->head = q->tail = NULL;
	}
	else
	{
		q->head = q->head->next;
	}
	free(cur);

}

QDataType QueueFront(Queue* q)
{
	assert(q);
	assert(!QueueEmpty(q));

	return q->head->data;
}

QDataType QueueBack(Queue* q)
{
	assert(q);
	assert(!QueueEmpty(q));

	return q->tail->data;
}

int QueueSize(Queue* q)
{
	assert(q);
	
	int size = 0;
	QNode* cur = q->head;
	while (cur)
	{
		size++;
		cur = cur->next;
	}
	return size;

}

bool QueueEmpty(Queue* q)
{
	assert(q);
	return q->head == NULL;
}