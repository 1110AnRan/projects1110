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
	span->_isUse = true;
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

void CentralCache::ReleaseListToSpans(void* start, size_t size)
{
	size_t index = SizeClass::Index(size);
	_spanLists[index]._mtx.lock();//����
	while (start)
	{
		void* next = NextObj(start);
		Span* span = PageCache::GetInstance()->MapObjectToSpan(start);
		//ͷ��
		NextObj(start) = span->_freeList;
		span->_freeList = start;

		span->_useCount--;				//���±������thread cache�ļ���
		if (span->_useCount == 0)		//��ʱ���԰�span�黹��pagecache
		{
			_spanLists[index].Erase(span);
			span->_freeList = nullptr;	//pagecache�е�span�����и�һС�����
			span->_next = nullptr;		//�������뵽pagecache�е�������
			span->_prev = nullptr;

			//���������Ҫ��pagecache���в���
			//���ù�_spanLists[index]����
			_spanLists[index]._mtx.unlock();
			PageCache::GetInstance()->_pageMtx.lock();//����
			PageCache::GetInstance()->ReleaseSpanToPageCache(span);
			PageCache::GetInstance()->_pageMtx.unlock();//����
			_spanLists[index]._mtx.lock();
		}
		start = next;
	}

	_spanLists[index]._mtx.unlock();//����
}