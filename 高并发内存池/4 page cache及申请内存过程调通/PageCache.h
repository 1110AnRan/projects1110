#pragma once
#include"Common.h"


//单例模式
class PageCache
{
public:
	static PageCache* GetInstance()
	{
		return &_sInst;
	}

	// 获取一个K页的span
	Span* NewSpan(size_t k);

	std::mutex _pageMtx;	//大锁
private:
	SpanList _spanLists[NPAGES];

	PageCache()
	{}

	PageCache(const PageCache&) = delete;

	static PageCache _sInst;
};