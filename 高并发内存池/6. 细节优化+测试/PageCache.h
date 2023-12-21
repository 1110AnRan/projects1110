#pragma once
#include"Common.h"
#include"ObjectPool.h"

//����ģʽ
class PageCache
{
public:
	static PageCache* GetInstance()
	{
		return &_sInst;
	}

	// ��ȡһ��Kҳ��span
	Span* NewSpan(size_t k);

	//���һ���Ӷ���ӳ�䵽span��span
	Span* MapObjectToSpan(void* obj);

	//��centralcache�е�span�黹��page
	void ReleaseSpanToPageCache(Span* span);

	std::mutex _pageMtx;	//����
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