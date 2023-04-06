#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef int	SLDataType;
typedef struct SeqList
{
	SLDataType* arr; //指向动态开辟的数组
	size_t size;     //有效数据个数
	size_t capacity;//容量空间的大小
}SeqList;

//增删查改

// 顺序表初始化
void SeqListInit(SeqList* ps);

//顺序表销毁
void SeqListDestory(SeqList* ps);

//顺序表打印
void SeqListPrint(SeqList* ps);

//检查空间，如果满了，进行增容
void SeqListCheckCapacity(SeqList* ps);

// 顺序表尾插
void SeqListPushBack(SeqList* ps, SLDataType x);

// 顺序表尾删
void SeqListPopBack(SeqList* ps);

// 顺序表头插
void SeqListPushFront(SeqList* ps, SLDataType x);

// 顺序表头删
void SeqListPopFront(SeqList* ps);

// 顺序表查找,返回x值对应的下标
int SeqListFind(SeqList* ps, SLDataType x);

// 顺序表在pos前的位置插入x
void SeqListInsert(SeqList* ps, size_t pos, SLDataType x);

// 顺序表删除pos位置的值
void SeqListErase(SeqList* ps, size_t pos);