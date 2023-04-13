#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef int SLTDateType;
typedef struct SListNode
{
	SLTDateType data;
	struct SListNode* next;
}SLTNode;

//声明函数

// 动态申请一个节点
SLTNode* BuySListNode(SLTDateType x);

// 单链表打印
void SListPrint(SLTNode* phead);

// 单链表尾插
void SListPushBack(SLTNode** pphead, SLTDateType x);

// 单链表的头插
void SListPushFront(SLTNode** pphead, SLTDateType x);

// 单链表的尾删
void SListPopBack(SLTNode** pphead);

// 单链表头删
void SListPopFront(SLTNode** pphead);

// 单链表查找pos的位置
SLTNode* SListFind(SLTNode* phead, SLTDateType x);

// 在pos位置之后插入x
void SListInsertAfter(SLTNode* pos, SLTDateType x);

//在pos位置之前插入一个节点
void SListInsert(SLTNode** pphead, SLTNode * pos ,SLTDateType x);

//删除单链表的pos位置的前一个节点
void SListEraseFront(SLTNode** pphead, SLTNode* pos);

// 单链表删除pos位置之后的值
void SListEraseAfter(SLTNode* pos);

//销毁该单链表
void SListDestroy(SLTNode** pphead);

