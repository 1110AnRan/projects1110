#pragma once

#include<iostream>
#include<iostream>
#include<vector>
#include<thread>
#include<assert.h>
#include<time.h>
using std::cout;
using std::endl;


// С�ڵ���MAX_BYTES������thread cache����
// ����MAX_BYTES����ֱ����page cache����ϵͳ������
static const size_t MAX_BYTES = 256 * 1024;

// thread cache �� central cache���������ϣͰ�ı��С
static const size_t NFREELISTS = 208;


static void*& NextObj(void* obj)
{
	return *(void**)obj;
}

//�����зֺõ�С�������������
class FreeList
{
public:

	void Push(void* obj)	//��ͬ��ObjectPool��new
	{
		assert(obj);

		//ͷ��
		NextObj(obj) = _freeList;
		_freeList = obj;
	}


	void* Pop()	//��ͬ��ObjectPool��Delete
	{
		assert(_freeList);

		//ͷɾ
		void* obj = _freeList;
		_freeList = NextObj(obj);

		return obj;

	}


	bool Empty()
	{
		return _freeList == nullptr;
	}
private:
	void* _freeList = nullptr;
};




//��������С�Ķ���ӳ�����
class SizeClass
{
public:
	// ������������10%���ҵ�����Ƭ�˷�
	// [1,128]					8byte����       freelist[0,16)		
	// [128+1,1024]				16byte����		freelist[16,72)
	// [1024+1,8*1024]			128byte����		freelist[72,128)
	// [8*1024+1,64*1024]		1024byte����	freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte����  freelist[184,208)

	static inline size_t _Roundup(size_t bytes, size_t alignNum)
	{
		return (((bytes)+alignNum - 1) & ~(alignNum - 1));
	}

	static inline size_t Roundup(size_t bytes)
	{
		if (bytes <= 128)
		{
			return _Roundup(bytes, 8);
		}
		else if (bytes <= 1024)
		{
			return _Roundup(bytes, 16);
		}
		else if (bytes <= 8 * 1024)
		{
			return _Roundup(bytes, 128);
		}
		else if (bytes <= 64 * 1024)
		{
			return _Roundup(bytes, 1024);
		}
		else if (bytes <= 256 * 1024)
		{
			return _Roundup(bytes, 8 * 1024);
		}
		else
		{
			assert(false);
			return -1;
		}
	}


	//�����Ӧ����������Ͱ
	static inline size_t _Index(size_t bytes, size_t align_shift)
	{
		return (((bytes)+(1 << align_shift) - 1) >> align_shift) - 1;
	}

	static inline size_t Index(size_t bytes)
	{
		assert(bytes <= MAX_BYTES);

		//ÿ�������ж��ٸ���������
		static size_t groups[4] = { 16,56,56,56 };
		if (bytes <= 128)
		{
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024)
		{
			return _Index(bytes - 128, 4) + groups[0];
		}
		else if (bytes <= 8 * 1024)
		{
			return _Index(bytes - 1024, 7) + groups[0] + groups[1];
		}
		else if (bytes <= 64 * 1024)
		{
			return _Index(bytes - 8 * 1024, 10) + groups[0] + groups[1] + groups[2];
		}
		else if (bytes <= 256 * 1024)
		{
			return _Index(bytes - 64 * 1024, 13) + groups[0] + groups[1] + groups[2] + groups[3];
		}
		else
		{
			assert(false);
			return -1;
		}
	}
};