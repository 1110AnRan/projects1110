#include"ThreadCache.h"


//�����Ļ����ȡ����
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//����ʼ���������㷨
	//1���ʼ����һ����central cacheһ������Ҫ̫�࣬��ΪҪ̫���˿����ò���
	//2������㲻����size��С���ڴ�������ôbatchNum�ͻ᲻��������ֱ������
	size_t batchNum = min(_freeLists[index].MaxSize(), SizeClass::NumMoveSize(size));
	if (batchNum == _freeLists[index].MaxSize())
	{
		_freeLists[index].MaxSize() += 1;
	}
	void* start = nullptr;
	void* end = nullptr;
	size_t actualNum = CentralCache::GetInstance()->FetchRangeObj(start, end, batchNum, size);

	assert(actualNum > 0);

	if (actualNum == 1)		//���ֻ���뵽һ������ֱ�ӷ��ؼ���
	{
		assert(start == end);
		return start;
	}
	else					//����ж�����򽫶���Ķ���ҵ�freeLists������
	{
		_freeLists[index].PushRange(NextObj(start), end, actualNum - 1);
		return start;
	}


}
void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAX_BYTES);

	//�ҵ���Ӧ�Ķ����ֽ����Լ�Ͱ
	size_t alignSize = SizeClass::Roundup(size);
	size_t index = SizeClass::Index(size);
	if (!_freeLists[index].Empty())
	{
		return _freeLists[index].Pop();
	}
	else
	{
		return FetchFromCentralCache(index, alignSize);
	}
}


void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(ptr);
	assert(size <= MAX_BYTES);

	//�ҵ���Ӧ��Ͱ
	size_t index = SizeClass::Index(size);
	_freeLists[index].Push(ptr);

	// ���thread cacheĳ��Ͱ������������ĳ��ȳ�����һ��������central cache����Ķ������
	// ��ô��ʱ���Ǿ�Ҫ�Ѹ����������е���Щ���󻹸�central cache��
	if (_freeLists[index].Size() >= _freeLists[index].MaxSize())
	{
		ListTooLong(_freeLists[index], size);
	}

}

void ThreadCache::ListTooLong(FreeList& list, size_t size)
{
	void* start = nullptr;
	void* end = nullptr;
	//��list��ȡ��һ�������Ķ���
	list.PopRange(start, end, list.MaxSize());

	//���õ��Ķ��󻹸�CentralCache
	CentralCache::GetInstance()->ReleaseListToSpans(start, size);
}
