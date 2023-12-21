#pragma once
#include"Common.h"
#include"ObjectPool.h"

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

	//获得一个从对象映射到span的span
	Span* MapObjectToSpan(void* obj);

	//把centralcache中的span归还给page
	void ReleaseSpanToPageCache(Span* span);

	std::mutex _pageMtx;	//大锁
private:
	ObjectPool<Span> _spanPool;

	//std::unordered_map<PAGE_ID, Span*> _idSpanMap;
	std::map<PAGE_ID, Span*>_idSpanMap;
	SpanList _spanLists[NPAGES];

	PageCache()
	{}

	PageCache(const PageCache&) = delete;

	static PageCache _sInst;
};