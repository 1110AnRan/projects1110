#define _CRT_SECURE_NO_WARNINGS 1
#include "SeqList.h"

int main()
{
	SeqList plist;
	plist.SeqListPushBack(10);
	plist.SeqListPushBack(20);
	plist.SeqListPushBack(30);
	plist.SeqListPushBack(40);
	plist.SeqListPushBack(50);
	plist.SeqListPushBack(50);

	plist.SeqListPrint();

	plist.SeqListPopBack();

	plist.SeqListPrint();

	plist.SeqListPushFront(40);
	//plist.SeqListPushFront(40);
	//plist.SeqListPushFront(40);
	//plist.SeqListPushFront(40);
	//plist.SeqListPushFront(40);

	plist.SeqListPrint();

	plist.SeqListPopFront();
	//plist.SeqListPopFront();
	//plist.SeqListPopFront();
	//plist.SeqListPopFront();
	//plist.SeqListPopFront();
	//plist.SeqListPopFront();

	plist.SeqListPrint();

	int  pos = plist.SeqListFind(0, 50);
	if (pos != -1)
	{
		printf("找到了!,下标是%d\n", pos);

		plist.SeqListInsert(pos, 60);

		plist.SeqListPrint();

	}
	pos = plist.SeqListFind(0, 60);
	if (pos != -1)
	{
		plist.SeqListErase(pos);
		plist.SeqListPrint();
	}


	return 0;
}