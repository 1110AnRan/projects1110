#include "CentralCache.h"

CentralCache CentralCache::_sInst;


Span* CentralCache::GetOneSpan(SpanList& list, size_t size)
{
	//...
	return nullptr;
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

	_spanLists[index]._mtx.unlock();
	return acutalNum;

	
}