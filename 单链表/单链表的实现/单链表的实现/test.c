#include "SList.h"

void test1()
{
	SLTNode* plist = BuySListNode(1);
	SListPrint(plist);
}

void test2()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);
	SListPushBack(&plist, 5);
	SListPushBack(&plist, 6);

	SListPrint(plist);
}

void test3()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);

	SListPushFront(&plist, 1);
	SListPushFront(&plist, 2);
	SListPushFront(&plist, 3);
	SListPushFront(&plist, 4);

	SListPrint(plist);
}

void test4()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);

	SListPopBack(&plist);
	SListPopBack(&plist);
	SListPopBack(&plist);
	SListPopBack(&plist);

	SListPrint(plist);
}

void test5()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);

	SListPopFront(&plist);
	SListPopFront(&plist);
	SListPopFront(&plist);
	SListPopFront(&plist);

	SListPrint(plist);
}

void test6()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 4);

	SListPrint(plist);

	SLTNode* pos1 = SListFind(plist,2);
	SLTNode* pos2 = NULL;
	SLTNode* pos = SListFind(plist, 2);
	int i = 1;

	//下一个data为2的节点
	if(pos2)
	{
		pos2 = SListFind(pos1->next,2);
	}
	while (pos)
	{
		printf("第%d个pos节点:%p->%d\n", i++, pos, pos->data);
		pos = SListFind(pos->next, 2);
	}

	//修改
	pos = SListFind(plist, 3);
	if (pos)
	{
		pos->data = 30;
	}

	SListPrint(plist);
}

void test7()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);

	SListPrint(plist);

	SLTNode* pos = SListFind(plist, 1);
	if (pos)
	{
		SListInsertAfter(pos, 30);
	}

	SListPrint(plist);
}

void test8()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);

	SListPrint(plist);

	SLTNode* pos = SListFind(plist, 3);
	if (pos)
	{
		SListInsert(&plist, pos, 30);
	}

	pos = SListFind(plist, 1);
	if (pos)
	{
		SListInsert(&plist, pos, 10);
	}
	SListPrint(plist);
}

void test10()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);
	SListPushBack(&plist, 5);
	SListPushBack(&plist, 6);

	SListPrint(plist);

	SLTNode* pos = SListFind(plist, 4);
	if (pos)
	{
		SListEraseFront(&plist, pos);
	}
	SListPrint(plist);

	pos = SListFind(plist, 6);
	if (pos)
	{
		SListEraseFront(&plist, pos);
	}
	SListPrint(plist);

	pos = SListFind(plist, 1);
	if (pos)
	{
		SListEraseFront(&plist, pos);
	}
	SListPrint(plist);
}

void test11()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);
	SListPushBack(&plist, 5);
	SListPushBack(&plist, 6);

	SListPrint(plist);

	SLTNode* pos = SListFind(plist, 1);
	if (pos)
	{
		SListEraseAfter(pos);
	}
	SListPrint(plist);

	pos = SListFind(plist, 5);
	if (pos)
	{
		SListEraseAfter(pos);
	}
	SListPrint(plist);

}

void test12()
{
	SLTNode* plist = NULL;
	SListPushBack(&plist, 1);
	SListPushBack(&plist, 2);
	SListPushBack(&plist, 3);
	SListPushBack(&plist, 4);
	SListPushBack(&plist, 5);
	SListPushBack(&plist, 6);

	SListPrint(plist);

	SListDestroy(&plist);
	SListPrint(plist);
}
int main()
{
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	//test10();
	//test11();
	test12();
	return 0;
}