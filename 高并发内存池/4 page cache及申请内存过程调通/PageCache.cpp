#include"PageCache.h"


PageCache PageCache::_sInst;

Span* PageCache::NewSpan(size_t k)
{
	assert(k > 0 && k < NPAGES);


	//���ȿ�pagecache���Ƿ���kҳ��span
	if (!_spanLists[k].Empty())
	{
		return _spanLists[k].Popfront();

	}

	//�����kҳû�еĻ��������������Ƿ����span���������������и�
	//�ֳ�kҳ��n-kҳ
	for (size_t i = k + 1; i < NPAGES; ++i)
	{
		if (!_spanLists[i].Empty())
		{
			//�����и�
			Span* nSpan = _spanLists[i].Popfront();
			Span* kSpan = new Span;


			kSpan->_pageId = nSpan->_pageId;
			kSpan->_n = k;

			nSpan->_pageId += k;
			nSpan->_n -= k;
			//��nSpan���뵹��n-kҳ����ȥ
			_spanLists[nSpan->_n].Pushfront(nSpan);
			return kSpan;
		}
	}

	//����˵����kҳ�Լ���kҳ�Ժ�û����span
	//��ʱ����Ҫ�������һ��128ҳ��span
	Span* bigspan = new Span;
	void* ptr = SystemAlloc(NPAGES - 1);
	bigspan->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	bigspan->_n = NPAGES - 1;

	_spanLists[NPAGES - 1].Pushfront(bigspan);
	return NewSpan(k);
}