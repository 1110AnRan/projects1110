#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
//带头+双向+循环链表增删查改实现
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef int LTDataType;
typedef struct ListNode
{
	LTDataType data;
	struct ListNode* next;
	struct ListNode* prev;
}LTNode;


//开辟一块LTNode空间
LTNode* BuyDListNode(LTDataType x);

//初始化双向链表
LTNode* ListInit();

//双向链表销毁
void ListDestroy(LTNode* phead);

// 双向链表打印
void ListPrint(LTNode* phead);

// 双向链表尾插
void ListPushBack(LTNode* phead, LTDataType x);

// 双向链表尾删
void ListPopBack(LTNode* phead);

// 双向链表头插
void ListPushFront(LTNode* phead, LTDataType x);

// 双向链表头删
void ListPopFront(LTNode* phead);

// 双向链表查找
LTNode* ListFind(LTNode* phead, LTDataType x);

// 双向链表在pos的前面进行插入
void ListInsert(LTNode* pos, LTDataType x);

// 双向链表删除pos位置的节点
void ListErase(LTNode* pos);
