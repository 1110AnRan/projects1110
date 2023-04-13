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

//��������

// ��̬����һ���ڵ�
SLTNode* BuySListNode(SLTDateType x);

// �������ӡ
void SListPrint(SLTNode* phead);

// ������β��
void SListPushBack(SLTNode** pphead, SLTDateType x);

// �������ͷ��
void SListPushFront(SLTNode** pphead, SLTDateType x);

// �������βɾ
void SListPopBack(SLTNode** pphead);

// ������ͷɾ
void SListPopFront(SLTNode** pphead);

// ���������pos��λ��
SLTNode* SListFind(SLTNode* phead, SLTDateType x);

// ��posλ��֮�����x
void SListInsertAfter(SLTNode* pos, SLTDateType x);

//��posλ��֮ǰ����һ���ڵ�
void SListInsert(SLTNode** pphead, SLTNode * pos ,SLTDateType x);

//ɾ���������posλ�õ�ǰһ���ڵ�
void SListEraseFront(SLTNode** pphead, SLTNode* pos);

// ������ɾ��posλ��֮���ֵ
void SListEraseAfter(SLTNode* pos);

//���ٸõ�����
void SListDestroy(SLTNode** pphead);

