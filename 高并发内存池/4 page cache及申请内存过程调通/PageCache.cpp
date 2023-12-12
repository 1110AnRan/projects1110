#include"PageCache.h"


PageCache PageCache::_sInst;

Span* PageCache::NewSpan(size_t k)
{
	assert(k > 0 && k < NPAGES);


	//首先看pagecache中是否有k页的span
	if (!_spanLists[k].Empty())
	{
		return _spanLists[k].Popfront();

	}

	//如果第k页没有的话，继续往后找是否存在span，若有则对其进行切割
	//分成k页和n-k页
	for (size_t i = k + 1; i < NPAGES; ++i)
	{
		if (!_spanLists[i].Empty())
		{
			//进行切割
			Span* nSpan = _spanLists[i].Popfront();
			Span* kSpan = new Span;


			kSpan->_pageId = nSpan->_pageId;
			kSpan->_n = k;

			nSpan->_pageId += k;
			nSpan->_n -= k;
			//把nSpan插入倒第n-k页后面去
			_spanLists[nSpan->_n].Pushfront(nSpan);
			return kSpan;
		}
	}

	//到这说明第k页以及第k页以后都没有了span
	//这时就需要向堆申请一个128页的span
	Span* bigspan = new Span;
	void* ptr = SystemAlloc(NPAGES - 1);
	bigspan->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	bigspan->_n = NPAGES - 1;

	_spanLists[NPAGES - 1].Pushfront(bigspan);
	return NewSpan(k);
}