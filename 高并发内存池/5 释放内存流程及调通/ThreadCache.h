#pragma once

#include"Common.h"
#include"CentralCache.h"

class ThreadCache
{
public:
	//�����ڴ�
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	//�����Ļ����ȡ����
	void* FetchFromCentralCache(size_t index, size_t size);

	//����̫������freelist��size�����󷵻ظ�centralcache
	void ListTooLong(FreeList& list, size_t size);
private:
	FreeList _freeLists[NFREELIST];
};


// TLS thread local storage
static __declspec(thread) ThreadCache* pTLSThreadCache = nullptr;
