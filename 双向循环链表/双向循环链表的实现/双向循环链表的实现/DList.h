#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
//��ͷ+˫��+ѭ��������ɾ���ʵ��
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


//����һ��LTNode�ռ�
LTNode* BuyDListNode(LTDataType x);

//��ʼ��˫������
LTNode* ListInit();

//˫����������
void ListDestroy(LTNode* phead);

// ˫�������ӡ
void ListPrint(LTNode* phead);

// ˫������β��
void ListPushBack(LTNode* phead, LTDataType x);

// ˫������βɾ
void ListPopBack(LTNode* phead);

// ˫������ͷ��
void ListPushFront(LTNode* phead, LTDataType x);

// ˫������ͷɾ
void ListPopFront(LTNode* phead);

// ˫���������
LTNode* ListFind(LTNode* phead, LTDataType x);

// ˫��������pos��ǰ����в���
void ListInsert(LTNode* pos, LTDataType x);

// ˫������ɾ��posλ�õĽڵ�
void ListErase(LTNode* pos);
