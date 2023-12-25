#include"PageCache.h"


PageCache PageCache::_sInst;

Span* PageCache::NewSpan(size_t k)
{
	assert(k > 0);
	if (k > NPAGES - 1)
	{
		void* ptr = SystemAlloc(k);
		//Span* span = new Span;
		Span* span = _spanPool._new();
		span->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
		span->_n = k;
		//建立页号与span之间的映射关系
		//_idSpanMap[span->_pageId] = span;
		_idSpanMap.set(span->_pageId, span);
		return span;
	}


	//首先看pagecache中是否有k页的span
	if (!_spanLists[k].Empty())
	{
		Span* kSpan = _spanLists[k].Popfront();

		//建立页号与span的映射关系，方便centralcache回收内存时找到对应的span
		for (size_t i = 0; i < kSpan->_n; ++i)
		{
			_idSpanMap.set(kSpan->_pageId + i, kSpan);
		}

		return kSpan;


	}

	//如果第k页没有的话，继续往后找是否存在span，若有则对其进行切割
	//分成k页和n-k页
	for (size_t i = k + 1; i < NPAGES; ++i)
	{
		if (!_spanLists[i].Empty())
		{
			//进行切割
			Span* nSpan = _spanLists[i].Popfront();
			//Span* kSpan = new Span;
			Span* kSpan = _spanPool._new();


			kSpan->_pageId = nSpan->_pageId;
			kSpan->_n = k;

			nSpan->_pageId += k;
			nSpan->_n -= k;
			//把nSpan插入倒第n-k页后面去
			_spanLists[nSpan->_n].Pushfront(nSpan);
			//存储nSpan首尾页号与nSpan的映射关系，方便page cache回收内存
			/*_idSpanMap[nSpan->_pageId] = nSpan;
			_idSpanMap[nSpan->_pageId + nSpan->_n - 1] = nSpan;*/
			_idSpanMap.set(nSpan->_pageId, nSpan);
			_idSpanMap.set(nSpan->_pageId + nSpan->_n - 1, nSpan);


			//建立页号与span的映射关系，方便centralcache回收内存时找到对应的span
			for (size_t i = 0; i < kSpan->_n; ++i)
			{
				//_idSpanMap[kSpan->_pageId + i] = kSpan;
				_idSpanMap.set(kSpan->_pageId + i, kSpan);
			}

			return kSpan;
		}
	}

	//到这说明第k页以及第k页以后都没有了span
	//这时就需要向堆申请一个128页的span
	//Span* bigspan = new Span;
	Span* bigspan = _spanPool._new();
	void* ptr = SystemAlloc(NPAGES - 1);
	bigspan->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	bigspan->_n = NPAGES - 1;

	_spanLists[bigspan->_n].Pushfront(bigspan);
	return NewSpan(k);
}


// 当我们要通过某个页号查找其对应的span时
// 该页号与其span之间的映射一定是建立过的
// 如果此时我们没有在unordered_map当中找到
// 则说明我们之前的代码逻辑有问题
// 此当没有找到对应的span时可以直接用断言结束程序
// 以表明程序逻辑出错。
Span* PageCache::MapObjectToSpan(void* obj)
{
	PAGE_ID id = ((PAGE_ID)obj >> PAGE_SHIFT);//把对应的地址转换成页号

	//std::unique_lock<std::mutex> lock(_pageMtx);	//构造时加锁，析构时自动解锁
	//auto ret = _idSpanMap.find(id);
	//if (ret != _idSpanMap.end())
	//{
	//	return ret->second;
	//}
	//else
	//{
	//	assert(false);
	//	return nullptr;
	//}
	auto ret = (Span*)_idSpanMap.get(id);
	assert(ret != nullptr);
	return ret;
}


//释放空闲的span回pagecache，并合并相邻的span
void PageCache::ReleaseSpanToPageCache(Span* span)
{
	//大于128页释放给堆
	if (span->_n > NPAGES - 1)
	{
		void* ptr = (void*)(span->_pageId << PAGE_SHIFT);
		SystemFree(ptr);
		//delete span;
		_spanPool.Delete(span);
		return;
	}

	//向前合并
	while (1)
	{
		PAGE_ID prevId = span->_pageId - 1;
		//auto ret = _idSpanMap.find(prevId);
		////span->_pageId-1页未申请，直接break,停止向前合并
		//if (ret == _idSpanMap.end())
		//{
		//	break;
		//}
		auto ret = (Span*)_idSpanMap.get(prevId);
		Span* prevspan = ret;
		//该span正在被使用，直接break，停止向前合并
		if (prevspan->_isUse == true)
		{
			break;
		}
		//合并后的页数大于128，无法管理，直接break，停止向前合并
		if (prevspan->_n + span->_n > NPAGES - 1)
		{
			break;
		}
		//走到这里才开始向前合并
		span->_n += prevspan->_n;
		span->_pageId = prevspan->_pageId;

		//将prevspan从_spanlists中删除
		_spanLists[prevspan->_n].Erase(prevspan);

		//delete prevspan;
		_spanPool.Delete(prevspan);
	}
	
	//向后合并
	while (1)
	{
		PAGE_ID nextId = span->_pageId + span->_n;
		//auto ret = _idSpanMap.find(nextId);
		////span后面的页没有被申请，直接break，停止向后合并
		//if (ret == _idSpanMap.end())
		//{
		//	break;
		//}
		auto ret = (Span*)_idSpanMap.get(nextId);
		Span* nextspan = ret;
		//span后面的页正在被使用，直接break，停止向后合并
		if (nextspan->_isUse == true)
		{
			break;
		}
		//span和后面管理的页之和大于128，直接break，停止向后合并
		if (span->_n + nextspan->_n > NPAGES - 1)
		{
			break;
		}
		//进行合并
		span->_n += nextspan->_n;

		_spanLists[nextspan->_n].Erase(nextspan);
		//delete nextspan;
		_spanPool.Delete(nextspan);
	}

	_spanLists[span->_n].Pushfront(span);
	span->_isUse = false;
	//_idSpanMap[span->_pageId] = span;
	//_idSpanMap[span->_pageId + span->_n - 1] = span;
	_idSpanMap.set(span->_pageId, span);
	_idSpanMap.set(span->_pageId + span->_n - 1, span);
}
