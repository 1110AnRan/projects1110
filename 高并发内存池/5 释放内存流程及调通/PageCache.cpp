#include"PageCache.h"


PageCache PageCache::_sInst;

Span* PageCache::NewSpan(size_t k)
{
	assert(k > 0 && k < NPAGES);


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
			Span* kSpan = new Span;


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
	Span* bigspan = new Span;
	void* ptr = SystemAlloc(NPAGES - 1);
	bigspan->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	bigspan->_n = NPAGES - 1;

	_spanLists[NPAGES - 1].Pushfront(bigspan);
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

		delete prevspan;
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
		delete nextspan;
	}

	_spanLists[span->_n].Pushfront(span);
	span->_isUse = false;
	_idSpanMap[span->_pageId] = span;
	_idSpanMap[span->_pageId + span->_n - 1] = span;
}
