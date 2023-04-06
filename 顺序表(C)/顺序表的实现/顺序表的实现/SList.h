#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef int	SLDataType;
typedef struct SeqList
{
	SLDataType* arr; //ָ��̬���ٵ�����
	size_t size;     //��Ч���ݸ���
	size_t capacity;//�����ռ�Ĵ�С
}SeqList;

//��ɾ���

// ˳����ʼ��
void SeqListInit(SeqList* ps);

//˳�������
void SeqListDestory(SeqList* ps);

//˳����ӡ
void SeqListPrint(SeqList* ps);

//���ռ䣬������ˣ���������
void SeqListCheckCapacity(SeqList* ps);

// ˳���β��
void SeqListPushBack(SeqList* ps, SLDataType x);

// ˳���βɾ
void SeqListPopBack(SeqList* ps);

// ˳���ͷ��
void SeqListPushFront(SeqList* ps, SLDataType x);

// ˳���ͷɾ
void SeqListPopFront(SeqList* ps);

// ˳������,����xֵ��Ӧ���±�
int SeqListFind(SeqList* ps, SLDataType x);

// ˳�����posǰ��λ�ò���x
void SeqListInsert(SeqList* ps, size_t pos, SLDataType x);

// ˳���ɾ��posλ�õ�ֵ
void SeqListErase(SeqList* ps, size_t pos);