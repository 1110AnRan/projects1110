#include "DList.h"

void test1()
{
	LTNode* plist = ListInit();
	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);

}

void test2()
{
	LTNode* plist = ListInit();

	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);

	ListPopBack(plist);
	ListPopBack(plist);
	ListPopBack(plist);
	//ListPopBack(plist);
	//ListPopBack(plist);
	//ListPopBack(plist);


	ListPrint(plist);
}

void test3()
{
	LTNode* plist = ListInit();
	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);

	ListPushFront(plist, 1);
	ListPushFront(plist, 2);
	ListPushFront(plist, 3);
	ListPushFront(plist, 4);

	ListPrint(plist);

}

void test4()
{
	LTNode* plist = ListInit();
	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);

	//ListPushFront(plist, 1);
	//ListPushFront(plist, 2);

	//ListPrint(plist);

	ListPopFront(plist);
	ListPopFront(plist);
	ListPopFront(plist);
	ListPopFront(plist);
	ListPopFront(plist);
	ListPopFront(plist);

	ListPrint(plist);

}

void test5()
{
	LTNode* plist = ListInit();
	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);

	LTNode* pos = ListFind(plist, 3);
	if (pos)
	{
		ListInsert(pos, 30);
	}
	ListPrint(plist);

	pos = ListFind(plist, 4);
	if (pos)
	{
		ListInsert(pos, 40);
	}
	ListPrint(plist);

	pos = ListFind(plist, 5);
	if (pos)
	{
		ListInsert(pos, 50);
	}
	ListPrint(plist);

}

void test6()
{
	LTNode* plist = ListInit();
	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);

	LTNode* pos = ListFind(plist, 4);
	if (pos)
	{
		ListErase(pos);
	}
	ListPrint(plist);

	pos = ListFind(plist, 5);
	if (pos)
	{
		ListErase(pos);
	}
	ListPrint(plist);
}

void test7()
{
	LTNode* plist = ListInit();
	ListPushBack(plist, 1);
	ListPushBack(plist, 2);
	ListPushBack(plist, 3);
	ListPushBack(plist, 4);
	ListPushBack(plist, 5);

	ListPrint(plist);
	
	ListDestroy(plist);
	free(plist);
	plist = NULL;

}
int main()
{
	//˫�������β��
	//test1();
	
	//˫�������βɾ
	//test2();
	 
	// ˫�������ͷ��
	//test3();
	
	// ˫�������ͷɾ
	//test4();

	//˫������Ĳ��������
	//test5();

	// ˫������ɾ��posλ�õĽڵ�
	//test6();

	//˫�����������
	//test7();
	return 0;
}