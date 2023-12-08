#pragma once
#include"Common.h"


// central cache��page cache������������ֻ��һ��
// ��������ֻ�ܴ���һ���������
// ���ǿ��Խ�������Ϊ����ģʽ
class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_sInst;
	}


	//���һ���ǿյ�span����
	Span* GetOneSpan(SpanList& list, size_t size);


	//��central cache��ȡһ������batchNum�Ķ����thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size);


private:
	SpanList _spanLists[NFREELIST];

private:
	//���캯��˽�л�
	CentralCache()
	{}

	CentralCache(const CentralCache&) = delete;

	static CentralCache _sInst;
};