#pragma once


#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<assert.h>
#include<time.h>
#include<unordered_map>
#include<algorithm>
#include<atomic>
#include<map>
using std::cout;
using std::endl;


#ifdef _WIN32
	#include<windows.h>
#else

#endif

#ifdef _WIN64
typedef unsigned long long PAGE_ID;
#elif _WIN32
typedef size_t PAGE_ID;
#else
//Linux
#endif


// ֱ��ȥ���ϰ�ҳ����ռ�
inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, kpage << 13, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// linux��brk mmap��
#endif

	if (ptr == nullptr)
		throw std::bad_alloc();

	return ptr;
}

//����ͷſռ�
inline static void SystemFree(void* ptr)
{
#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	//Linus�µ�sbrk��unmmap��
#endif

}



// С�ڵ���MAX_BYTES������thread cache����
// ����MAX_BYTES����ֱ����page cache����ϵͳ������
static const size_t MAX_BYTES = 256 * 1024;

// thread cache �� central cache���������ϣͰ�ı��С
static const size_t NFREELIST = 208;

//page cache�����ҳ��
static const size_t NPAGES = 129;

//ÿ��ҳ��СΪ8KB 2^13
static const size_t PAGE_SHIFT = 13;

static void*& NextObj(void* obj)
{
	return *(void**)obj;
}


// �����зֺõ�С�������������
class FreeList
{
public:

	void Push(void* obj)	//��ͬ��ObjectPool��new
	{
		assert(obj);

		//ͷ��
		NextObj(obj) = _freeList;
		_freeList = obj;
		++_size;
	}

	void PushRange(void* start, void* end, size_t n)
	{
		NextObj(end) = _freeList;
		_freeList = start;
		_size += n;
	}

	void* Pop()	//��ͬ��ObjectPool��Delete
	{
		assert(_freeList);

		//ͷɾ
		void* obj = _freeList;
		_freeList = NextObj(obj);
		--_size;
		return obj;
	}

	void PopRange(void*& start, void*& end, size_t n)
	{
		assert(n <= _size);

		start = _freeList;
		end = start;

		for (size_t i = 0; i < n - 1; i++)
		{
			end = NextObj(end);
		}

		_freeList = NextObj(end);
		NextObj(end) = nullptr;

		_size -= n;
	}

	bool Empty()
	{
		return _freeList == nullptr;
	}

	size_t& MaxSize()
	{
		return _maxSize;
	}

	size_t& Size()
	{
		return _size;
	}
private:
	void* _freeList = nullptr;
	size_t _maxSize = 1;
	size_t _size = 0;
};


// ��������С�Ķ���ӳ�����
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
			//����256KB�İ�ҳ����
			return _Roundup(bytes, 1 << PAGE_SHIFT);
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

	static size_t NumMoveSize(size_t size)
	{
		assert(size > 0);


		// [2, 512]��һ�������ƶ����ٸ������(������)����ֵ
		// С����һ���������޸�
		// С����һ���������޵�
		int num = MAX_BYTES / size;
		
		if (num < 2)
		{
			num = 2;
		}

		if (num > 512)
		{
			num = 512;
		}

		return num;
	}

	//central cacheһ����page cache������ٸ�ҳ
	static size_t NumMovePage(size_t size)
	{
		size_t num = NumMoveSize(size);	//һ��������ٸ�����
		size_t npage = num * size;		//num��size��С�Ķ�����ֽ���

		npage >>= PAGE_SHIFT;			//ת���ɶ�Ӧ��ҳ��
		if (npage == 0)					//���ٸ�һҳ
		{
			return 1;
		}
		return npage;
	}
};

// ������ҳΪ��λ�Ĵ���ڴ��
struct Span
{
	PAGE_ID _pageId = 0;	//��ʼҳ��
	size_t _n = 0;			//ҳ��

	Span* _prev = nullptr;	//˫������ṹ
	Span* _next = nullptr;	

	size_t _objSize = 0;		//������������ÿ�������С
	size_t _useCount = 0;		//��������������С���ڴ�Ŀ���
	void* _freeList = nullptr;	//�������������

	bool _isUse = false;		//����Ƿ��ڱ�ʹ��
};


// ��һ��˫��ѭ������
// ��˫����ɾ����span�ỹ����һ���page cache
// �൱��ֻ�ǰ����span��˫�������Ƴ�
// ��˲���Ҫ��ɾ����span����delete����
// spanҪ����
// ���threadcache���span����������
class SpanList
{
public:
	SpanList()
	{
		_head = new Span;
		//_head = _spanPool._new();
		_head->_prev = _head;
		_head->_next = _head;
	}

	Span* Begin()
	{
		return _head->_next;
	}

	Span* End()
	{
		return _head;
	}

	bool Empty()
	{
		return _head->_next == _head;
	}

	void Pushfront(Span* span)
	{
		Insert(Begin(), span);
	}

	Span* Popfront()
	{
		Span* front = _head->_next;
		Erase(front);
		return front;
	}

	void Insert(Span* pos, Span* newSpan)
	{
		assert(pos);
		assert(newSpan);

		Span* prev = pos->_prev;
		// prev newSpan pos
		prev->_next = newSpan;
		newSpan->_prev = prev;
		newSpan->_next = pos;
		pos->_prev = newSpan;

	}

	void Erase(Span* pos)
	{
		assert(pos);
		//assert(pos != _head);

		Span* prev = pos->_prev;
		Span* next = pos->_next;
		prev->_next = next;
		next->_prev = prev;
	}
private:
	Span* _head;
	//static ObjectPool<Span> _spanPool;
public:
	std::mutex _mtx; // Ͱ��
};