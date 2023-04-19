#include "Queue.h"



void TestQueue()
{
	Queue q;

	QueueInit(&q);
	QueuePush(&q, 1);
	QueuePush(&q, 2);

	printf("%d ", QueueSize(&q));
	QueuePush(&q, 3);
	QueuePush(&q, 4);
	QueuePush(&q, 5);
	printf("%d ", QueueSize(&q));

	printf("%d ", QueueFront(&q));

	printf("%d ", QueueBack(&q));
	//QueuePop(&q);
	//QueuePop(&q);
	//QueuePop(&q);
	//QueuePop(&q);
	//QueuePop(&q);

	QueueDestroy(&q);

	
	return 0;
}

int main()
{
	TestQueue();
	return 0;
}