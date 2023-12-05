#include"ThreadCache.h"

void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//...待写

	return nullptr;
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
