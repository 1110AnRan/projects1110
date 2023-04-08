#pragma once


#include <iostream>
#include <stdlib.h>
#include <assert.h>
using namespace std;

typedef int TypeElement;
class SeqList
{
private:
	TypeElement* arr;	//ָ��̬���ٵ�����
	size_t size;		//��¼�������е���Ч����
	size_t capacity;	//��¼�������е��������
public:
	SeqList();
	SeqList(size_t capacity);
	SeqList(const SeqList& plist);
	~SeqList();

	//˳����ӡ
	void SeqListPrint();

	//���˳����Ƿ����ˣ�������ˣ���������
	void SeqListCheckCapacity();

	//˳���β��
	void SeqListPushBack(TypeElement x);

	//˳���βɾ
	void SeqListPopBack();

	//˳���ͷ��
	void SeqListPushFront(TypeElement x);

	//˳���ͷɾ
	void SeqListPopFront();

	//˳�����ң�����xֵ��Ӧ���±�,beginΪѰ�ҵĵ�һ��λ�õ��±�
	int SeqListFind(TypeElement begin,TypeElement x);

	//˳�����posǰ��λ�ò���x
	void SeqListInsert(size_t pos, TypeElement x);

	//˳���ɾ��posλ�õ�ֵ
	void SeqListErase(size_t pos);
};