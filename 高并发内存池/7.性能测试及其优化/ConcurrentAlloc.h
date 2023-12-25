#pragma once
#include"ThreadCache.h"
#include"Common.h"
#include"PageCache.h"
#include"ObjectPool.h"


static void* ConcurrentAlloc(size_t size)
{
	if (size > MAX_BYTES)//大于256KB的内存申请
	{
		//计算出对应需要申请的页数
		size_t alignSize = SizeClass::Roundup(size);
		size_t kPage = alignSize >> PAGE_SHIFT;

		//向pagecache申请k页的span
		PageCache::GetInstance()->_pageMtx.lock();
		Span* span = PageCache::GetInstance()->NewSpan(kPage);
		span->_objSize = size;
		PageCache::GetInstance()->_pageMtx.unlock();

		void* ptr = (void*)(span->_pageId << PAGE_SHIFT);

		return ptr;

	}
	else
	{
		// 通过TLS 每个线程无锁的获取自己的专属的ThreadCache对象

		if (pTLSThreadCache == nullptr)
		{
			static ObjectPool<ThreadCache> tcPool;
			pTLSThreadCache = tcPool._new();
		}

		//cout << std::this_thread::get_id() << ":" << pTLSThreadCache << endl;

		return pTLSThreadCache->Allocate(size);
	}
}

static void ConcurrentFree(void* ptr)
{
	Span* span = PageCache::GetInstance()->MapObjectToSpan(ptr);
	size_t size = span->_objSize;
	if (size > MAX_BYTES)
	{

		PageCache::GetInstance()->_pageMtx.lock();
		PageCache::GetInstance()->ReleaseSpanToPageCache(span);
		PageCache::GetInstance()->_pageMtx.unlock();
	}
	else
	{
		assert(pTLSThreadCache);
		pTLSThreadCache->Deallocate(ptr, size);
	}
}