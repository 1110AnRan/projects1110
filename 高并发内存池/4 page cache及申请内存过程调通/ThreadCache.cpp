#include"ThreadCache.h"


//从中心缓存获取对象
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//慢开始反馈调节算法
	//1、最开始不会一次向central cache一次批量要太多，因为要太多了可能用不完
	//2、如果你不断有size大小的内存需求，那么batchNum就会不断增长，直到上限
	size_t batchNum = min(_freeLists[index].MaxSize(), SizeClass::NumMoveSize(size));
	if (batchNum == _freeLists[index].MaxSize())
	{
		_freeLists[index].MaxSize() += 1;
	}
	void* start = nullptr;
	void* end = nullptr;
	size_t actualNum = CentralCache::GetInstance()->FetchRangeObj(start, end, batchNum, size);

	assert(actualNum > 0);

	if (actualNum == 1)		//如果只申请到一个对象，直接返回即可
	{
		assert(start == end);
		return start;
	}
	else					//如果有多个，则将多余的对象挂到freeLists链表中
	{
		_freeLists[index].PushRange(NextObj(start), end);
		return start;
	}


}
void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAX_BYTES);

	//找到对应的对齐字节数以及桶
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

	//找到对应的桶
	size_t index = SizeClass::Index(size);
	_freeLists[index].Push(ptr);


}
