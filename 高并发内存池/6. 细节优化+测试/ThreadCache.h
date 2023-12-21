#pragma once

#include"Common.h"

class ThreadCache
{
public:
	//申请内存
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	//从中心缓存获取对象
	void* FetchFromCentralCache(size_t index, size_t size);

	//链表太长，将freelist中size个对象返回给centralcache
	void ListTooLong(FreeList& list, size_t size);
private:
	FreeList _freeLists[NFREELIST];
};


// TLS thread local storage
static __declspec(thread) ThreadCache* pTLSThreadCache = nullptr;
