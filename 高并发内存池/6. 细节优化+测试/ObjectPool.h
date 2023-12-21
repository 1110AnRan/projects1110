#pragma once
#include"Common.h"

template<class T>
class ObjectPool
{
public:
	T* _new()
	{
		T* obj = nullptr;

		//����ʹ�ñ��ͷŵĿռ�
		if (_freelist)
		{
			void* next = *((void**)_freelist);
			obj = (T*)_freelist;
			_freelist = next;
		}
		else
		{
			//ʣ���ڴ治��T�Ĵ�С���¿���һ���µĴ�ռ�
			if (_remainBytes < sizeof T)
			{
				_remainBytes = 1024 * 128;
				_memory = (char*)(SystemAlloc(_remainBytes >> 13));
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
		//������������
		obj->~T();

		*(void**)obj = _freelist;
		_freelist = obj;
	}
private:
	char* _memory = nullptr;	//ָ�����ڴ�ռ��ָ��
	size_t _remainBytes = 0;	//ʣ��ռ���ֽ���
	void* _freelist = nullptr;	//ָ���ͷŵ��ڴ�ռ�
};
//
//
//struct TreeNode
//{
//	int _val;
//	TreeNode* _left;
//	TreeNode* _right;
//
//	TreeNode()
//		:_val(0)
//		, _left(nullptr)
//		, _right(nullptr)
//	{}
//};
//
//void TestObjectPool()
//{
//	// �����ͷŵ��ִ�
//	const size_t Rounds = 3;
//
//	// ÿ�������ͷŶ��ٴ�
//	const size_t N = 100000;
//
//	std::vector<TreeNode*> v1;
//	v1.reserve(N);
//
//	size_t begin1 = clock();
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v1.push_back(new TreeNode);
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			delete v1[i];
//		}
//		v1.clear();
//	}
//
//	size_t end1 = clock();
//
//	std::vector<TreeNode*> v2;
//	v2.reserve(N);
//
//	ObjectPool<TreeNode> TNPool;
//	size_t begin2 = clock();
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v2.push_back(TNPool._new());
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			TNPool.Delete(v2[i]);
//		}
//		v2.clear();
//	}
//	size_t end2 = clock();
//
//	cout << "new cost time:" << end1 - begin1 << endl;
//	cout << "object pool cost time:" << end2 - begin2 << endl;
//}