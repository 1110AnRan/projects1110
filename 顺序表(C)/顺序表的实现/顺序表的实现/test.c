#include "SList.h"

void test1()
{
	SeqList plist;
	SeqListInit(&plist);

	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);

	SeqListPrint(&plist);

	SeqListDestory(&plist);
}

void test2()
{
	SeqList plist;
	SeqListInit(&plist);
	
	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);

	SeqListPopBack(&plist);
	SeqListPopBack(&plist);
	SeqListPopBack(&plist);
	SeqListPopBack(&plist);
	SeqListPopBack(&plist);

	SeqListPrint(&plist);

	SeqListDestory(&plist);
}

void test3()
{
	SeqList plist;
	SeqListInit(&plist);

	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);

	SeqListPrint(&plist);

	SeqListPushFront(&plist, 10);
	SeqListPushFront(&plist, 20);
	SeqListPushFront(&plist, 30);
	SeqListPushFront(&plist, 40);

	SeqListPrint(&plist);

	SeqListDestory(&plist);
}

void test4()
{
	SeqList plist;
	SeqListInit(&plist);

	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);

	SeqListPrint(&plist);

	SeqListPopFront(&plist);
	SeqListPopFront(&plist);
	SeqListPopFront(&plist);
	SeqListPopFront(&plist);

	SeqListPrint(&plist);

	SeqListDestory(&plist);

}

void test5()
{
	SeqList plist;
	SeqListInit(&plist);

	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);

	SeqListPrint(&plist);


	int find = SeqListFind(&plist, 3);
	printf("%d", find);

	SeqListDestory(&plist);
}

void test6()
{
	SeqList plist;
	SeqListInit(&plist);

	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);

	SeqListPrint(&plist);

	SeqListInsert(&plist, 3, 30);
	SeqListInsert(&plist, 3, 300);

	SeqListPrint(&plist);

	SeqListDestory(&plist);
}

void test7()
{
	SeqList plist;
	SeqListInit(&plist);

	SeqListPushBack(&plist, 1);
	SeqListPushBack(&plist, 2);
	SeqListPushBack(&plist, 3);
	SeqListPushBack(&plist, 4);
	SeqListPushBack(&plist, 5);

	SeqListPrint(&plist);

	int pos = SeqListFind(&plist, 1);
	if (pos != -1)
	{
		SeqListErase(&plist, pos);
	}
	//SeqListErase(&plist, 2);
	//SeqListErase(&plist, 1);

	SeqListPrint(&plist);

	SeqListDestory(&plist);
}

int main()
{
	//test1();
	//test2();
	//test3();
	test4();
	//test5();
	//test6();
	//test7();
	return 0;
}