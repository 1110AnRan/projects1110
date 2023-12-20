#pragma once
#include"Common.h"


// central cache和page cache在整个进程中只有一个
// 对于这种只能创建一个对象的类
// 我们可以将其设置为单例模式
class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_sInst;
	}


	//获得一个非空的span对象
	Span* GetOneSpan(SpanList& list, size_t size);


	//从central cache获取一定数量batchNum的对象给thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size);

	void ReleaseListToSpans(void* start, size_t size);
private:
	SpanList _spanLists[NFREELIST];

private:
	//构造函数私有化
	CentralCache()
	{}

	CentralCache(const CentralCache&) = delete;

	static CentralCache _sInst;
};