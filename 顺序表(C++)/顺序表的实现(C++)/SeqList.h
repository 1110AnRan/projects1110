#pragma once


#include <iostream>
#include <stdlib.h>
#include <assert.h>
using namespace std;

typedef int TypeElement;
class SeqList
{
private:
	TypeElement* arr;	//指向动态开辟的数组
	size_t size;		//记录该数组中的有效个数
	size_t capacity;	//记录该数组中的最大容量
public:
	SeqList();
	SeqList(size_t capacity);
	SeqList(const SeqList& plist);
	~SeqList();

	//顺序表打印
	void SeqListPrint();

	//检查顺序表是否满了，如果满了，进行增容
	void SeqListCheckCapacity();

	//顺序表尾插
	void SeqListPushBack(TypeElement x);

	//顺序表尾删
	void SeqListPopBack();

	//顺序表头插
	void SeqListPushFront(TypeElement x);

	//顺序表头删
	void SeqListPopFront();

	//顺序表查找，返回x值对应的下标,begin为寻找的第一个位置的下标
	int SeqListFind(TypeElement begin,TypeElement x);

	//顺序表在pos前的位置插入x
	void SeqListInsert(size_t pos, TypeElement x);

	//顺序表删除pos位置的值
	void SeqListErase(size_t pos);
};