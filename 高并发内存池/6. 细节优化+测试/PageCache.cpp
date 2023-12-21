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
		//����ҳ����span֮���ӳ���ϵ
		_idSpanMap[span->_pageId] = span;
		return span;
	}


	//���ȿ�pagecache���Ƿ���kҳ��span
	if (!_spanLists[k].Empty())
	{
		Span* kSpan = _spanLists[k].Popfront();

		//����ҳ����span��ӳ���ϵ������centralcache�����ڴ�ʱ�ҵ���Ӧ��span
		for (size_t i = 0; i < kSpan->_n; ++i)
		{
			_idSpanMap[kSpan->_pageId + i] = kSpan;
		}

		return kSpan;


	}

	//�����kҳû�еĻ��������������Ƿ����span���������������и�
	//�ֳ�kҳ��n-kҳ
	for (size_t i = k + 1; i < NPAGES; ++i)
	{
		if (!_spanLists[i].Empty())
		{
			//�����и�
			Span* nSpan = _spanLists[i].Popfront();
			//Span* kSpan = new Span;
			Span* kSpan = _spanPool._new();


			kSpan->_pageId = nSpan->_pageId;
			kSpan->_n = k;

			nSpan->_pageId += k;
			nSpan->_n -= k;
			//��nSpan���뵹��n-kҳ����ȥ
			_spanLists[nSpan->_n].Pushfront(nSpan);
			//�洢nSpan��βҳ����nSpan��ӳ���ϵ������page cache�����ڴ�
			_idSpanMap[nSpan->_pageId] = nSpan;
			_idSpanMap[nSpan->_pageId + nSpan->_n - 1] = nSpan;

			//����ҳ����span��ӳ���ϵ������centralcache�����ڴ�ʱ�ҵ���Ӧ��span
			for (size_t i = 0; i < kSpan->_n; ++i)
			{
				_idSpanMap[kSpan->_pageId + i] = kSpan;
			}

			return kSpan;
		}
	}

	//����˵����kҳ�Լ���kҳ�Ժ�û����span
	//��ʱ����Ҫ�������һ��128ҳ��span
	//Span* bigspan = new Span;
	Span* bigspan = _spanPool._new();
	void* ptr = SystemAlloc(NPAGES - 1);
	bigspan->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	bigspan->_n = NPAGES - 1;

	_spanLists[bigspan->_n].Pushfront(bigspan);
	return NewSpan(k);
}


// ������Ҫͨ��ĳ��ҳ�Ų������Ӧ��spanʱ
// ��ҳ������span֮���ӳ��һ���ǽ�������
// �����ʱ����û����unordered_map�����ҵ�
// ��˵������֮ǰ�Ĵ����߼�������
// �˵�û���ҵ���Ӧ��spanʱ����ֱ���ö��Խ�������
// �Ա��������߼�����
Span* PageCache::MapObjectToSpan(void* obj)
{
	PAGE_ID id = ((PAGE_ID)obj >> PAGE_SHIFT);//�Ѷ�Ӧ�ĵ�ַת����ҳ��

	std::unique_lock<std::mutex> lock(_pageMtx);	//����ʱ����������ʱ�Զ�����
	auto ret = _idSpanMap.find(id);
	if (ret != _idSpanMap.end())
	{
		return ret->second;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}


//�ͷſ��е�span��pagecache�����ϲ����ڵ�span
void PageCache::ReleaseSpanToPageCache(Span* span)
{
	//����128ҳ�ͷŸ���
	if (span->_n > NPAGES - 1)
	{
		void* ptr = (void*)(span->_pageId << PAGE_SHIFT);
		SystemFree(ptr);
		//delete span;
		_spanPool.Delete(span);
		return;
	}

	//��ǰ�ϲ�
	while (1)
	{
		PAGE_ID prevId = span->_pageId - 1;
		auto ret = _idSpanMap.find(prevId);
		//span->_pageId-1ҳδ���룬ֱ��break,ֹͣ��ǰ�ϲ�
		if (ret == _idSpanMap.end())
		{
			break;
		}
		Span* prevspan = ret->second;
		//��span���ڱ�ʹ�ã�ֱ��break��ֹͣ��ǰ�ϲ�
		if (prevspan->_isUse == true)
		{
			break;
		}
		//�ϲ����ҳ������128���޷�����ֱ��break��ֹͣ��ǰ�ϲ�
		if (prevspan->_n + span->_n > NPAGES - 1)
		{
			break;
		}
		//�ߵ�����ſ�ʼ��ǰ�ϲ�
		span->_n += prevspan->_n;
		span->_pageId = prevspan->_pageId;

		//��prevspan��_spanlists��ɾ��
		_spanLists[prevspan->_n].Erase(prevspan);

		//delete prevspan;
		_spanPool.Delete(prevspan);
	}
	
	//���ϲ�
	while (1)
	{
		PAGE_ID nextId = span->_pageId + span->_n;
		auto ret = _idSpanMap.find(nextId);
		//span�����ҳû�б����룬ֱ��break��ֹͣ���ϲ�
		if (ret == _idSpanMap.end())
		{
			break;
		}
		Span* nextspan = ret->second;
		//span�����ҳ���ڱ�ʹ�ã�ֱ��break��ֹͣ���ϲ�
		if (nextspan->_isUse == true)
		{
			break;
		}
		//span�ͺ�������ҳ֮�ʹ���128��ֱ��break��ֹͣ���ϲ�
		if (span->_n + nextspan->_n > NPAGES - 1)
		{
			break;
		}
		//���кϲ�
		span->_n += nextspan->_n;

		_spanLists[nextspan->_n].Erase(nextspan);
		//delete nextspan;
		_spanPool.Delete(nextspan);
	}

	_spanLists[span->_n].Pushfront(span);
	span->_isUse = false;
	_idSpanMap[span->_pageId] = span;
	_idSpanMap[span->_pageId + span->_n - 1] = span;
}
