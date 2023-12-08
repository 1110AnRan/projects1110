#include"ThreadCache.h"


//�����Ļ����ȡ����
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//����ʼ���������㷨
	//1���ʼ����һ����central cacheһ������Ҫ̫�࣬��ΪҪ̫���˿����ò���
	//2������㲻����size��С���ڴ�������ôbatchNum�ͻ᲻��������ֱ������
	size_t batchNum = std::min(_freeLists[index].MaxSize(), SizeClass::NumMoveSize(size));
	if (batchNum == _freeLists[index].MaxSize())
	{
		_freeLists[index].MaxSize() += 1;
	}
	void* start = nullptr;
	void* end = nullptr;
	size_t actualNum = CentralCache::GetInstance()->FetchRangeObj(start, end, batchNum, size);

	assert(actualNum > 1);

	if (actualNum == 1)
	{
		assert(start == end);
		return start;
	}
	else
	{
		_freeLists[index].PushRange(NextObj(start), end);
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
}
