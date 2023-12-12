#include "CentralCache.h"
#include "PageCache.h"
CentralCache CentralCache::_sInst;


Span* CentralCache::GetOneSpan(SpanList& list, size_t size)
{
	//������ǰ��SpanList�� �Ƿ��ж���δ����
	Span* it = list.Begin();
	while (it != list.End())
	{
		//����У�ֱ�ӷ��ص�ǰ��freelist
		if (it->_freeList != nullptr)
		{
			return it;
		}
		else
		{
			it = it->_next;
		}
	}
	//û�еĻ���Ҫ��pagecache����

	//�Ȱ�central cache��Ͱ�����
	list._mtx.unlock();


	//��page cache�ϴ��� 
	PageCache::GetInstance()->_pageMtx.lock();
	Span* span = PageCache::GetInstance()->NewSpan(SizeClass::NumMovePage(size));
	//��������֮�����
	PageCache::GetInstance()->_pageMtx.unlock();

	// �Ի�ȡspan�����з֣�����Ҫ��������Ϊ��������̷߳��ʲ������span

	//�������ڴ����ʼ��ַ�ʹ���ڴ�Ĵ�С(Byte)
	char* start = (char*)(span->_pageId << PAGE_SHIFT);
	size_t bytes = span->_n << PAGE_SHIFT;
	char* end = start + bytes;
	
	// �Ѵ���ڴ��г���������
	// ��sizeΪ��λ��

	//��������һ�飬����β��
	span->_freeList = start;
	start += size;
	void* tail = span->_freeList;
	int i = 1;
	while (start < end)
	{
		++i;
		NextObj(tail) = start;
		tail = NextObj(tail);
		start += size;
	}

	//�ǵ�����
	list._mtx.lock();
	//���кõ�spanͷ�嵽spanList
	list.Pushfront(span);

	return span;

}

size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size)
{
	size_t index = SizeClass::Index(size);
	_spanLists[index]._mtx.lock();

	Span* span = GetOneSpan(_spanLists[index], size);
	assert(span);
	assert(span->_freeList);

	start = span->_freeList;
	end = start;
	// ��span�л�ȡbatchNum������
	// �������batchNum�����ж����ö���
	size_t i = 0;
	size_t acutalNum = 1;
	while (i < batchNum - 1 && NextObj(end) != nullptr)
	{
		++i;
		++acutalNum;
		end = NextObj(end);
	}

	span->_freeList = NextObj(end);
	NextObj(end) = nullptr;
	span->_useCount += acutalNum;

	_spanLists[index]._mtx.unlock();
	return acutalNum;
}