#pragma once
#include<iostream>
#include<iostream>
#include<vector>
using std::cout;
using std::endl;

#ifdef _WIN32
	#include<windows.h>
#else

#endif




//在堆上按页申请空间
inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, kpage * (1 << 12), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);	//Windows API函数
#else 
	//Linux下的brx或mmap等

#endif
	
	if (ptr == nullptr)
		throw std::bad_alloc();

	return ptr;
}


template<class T>
class ObjectPool
{
public:
	T* _new()
	{
		T* obj = nullptr;

		//优先使用被释放的空间
		if (_freelist)
		{
			void* next = *((void**)_freelist);
			obj = (T*)_freelist;
			_freelist = next;
		}
		else
		{
			//剩余内存不够T的大小重新开辟一块新的大空间
			if (_remainBytes < sizeof T)
			{
				_remainBytes = 1024 * 128;
				_memory = (char*)SystemAlloc(_remainBytes);
				if (_memory == nullptr)
				{
					throw std::bad_alloc();
				}
			}
			

			obj = (T*)_memory;
			size_t ObjectSize = sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);
			_memory += ObjectSize;
			_remainBytes -= ObjectSize;
		}


		new(obj)T;
		
		return obj;
	}

	void Delete(T* obj)
	{
		//调用析构函数
		obj->~T();

		*(void**)obj = _freelist;
		_freelist = obj;
	}
private:
	char* _memory = nullptr;	//指向大块内存空间的指针
	size_t _remainBytes = 0;	//剩余空间的字节数
	void* _freelist = nullptr;	//指向被释放的内存空间
};


struct TreeNode
{
	int _val;
	TreeNode* _left;
	TreeNode* _right;

	TreeNode()
		:_val(0)
		, _left(nullptr)
		, _right(nullptr)
	{}
};

void TestObjectPool()
{
	// 申请释放的轮次
	const size_t Rounds = 5;

	// 每轮申请释放多少次
	const size_t N = 100000;

	std::vector<TreeNode*> v1;
	v1.reserve(N);

	size_t begin1 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v1.push_back(new TreeNode);
		}
		for (int i = 0; i < N; ++i)
		{
			delete v1[i];
		}
		v1.clear();
	}

	size_t end1 = clock();

	std::vector<TreeNode*> v2;
	v2.reserve(N);

	ObjectPool<TreeNode> TNPool;
	size_t begin2 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v2.push_back(TNPool._new());
		}
		for (int i = 0; i < N; ++i)
		{
			TNPool.Delete(v2[i]);
			if (i >= 5)
			{
				v2.push_back(TNPool._new());
			}
		}
		v2.clear();
	}
	size_t end2 = clock();

	cout << "new cost time:" << end1 - begin1 << endl;
	cout << "object pool cost time:" << end2 - begin2 << endl;
}