#include "CentralCache.h"
#include "PageCache.h"
CentralCache CentralCache::_sInst;


Span* CentralCache::GetOneSpan(SpanList& list, size_t size)
{
	//看看当前的SpanList中 是否还有对象未分配
	Span* it = list.Begin();
	while (it != list.End())
	{
		//如果有，直接返回当前的freelist
		if (it->_freeList != nullptr)
		{
			return it;
		}
		else
		{
			it = it->_next;
		}
	}
	//没有的话就要向pagecache申请

	//先把central cache的桶锁解掉
	list._mtx.unlock();


	//给page cache上大锁 
	PageCache::GetInstance()->_pageMtx.lock();
	Span* span = PageCache::GetInstance()->NewSpan(SizeClass::NumMovePage(size));
	//申请完了之后解锁
	PageCache::GetInstance()->_pageMtx.unlock();

	// 对获取span进行切分，不需要加锁，因为这会其他线程访问不到这个span

	//计算大块内存的起始地址和大块内存的大小(Byte)
	char* start = (char*)(span->_pageId << PAGE_SHIFT);
	size_t bytes = span->_n << PAGE_SHIFT;
	char* end = start + bytes;
	
	// 把大块内存切成自由链表
	// 以size为单位切

	//先切下来一块，方便尾差
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

	//记得上锁
	list._mtx.lock();
	//将切好的span头插到spanList
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
	// 从span中获取batchNum个对象
	// 如果不够batchNum个，有多少拿多少
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