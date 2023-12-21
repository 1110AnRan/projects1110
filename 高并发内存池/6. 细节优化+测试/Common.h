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


// 直接去堆上按页申请空间
inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, kpage << 13, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// linux下brk mmap等
#endif

	if (ptr == nullptr)
		throw std::bad_alloc();

	return ptr;
}

//向堆释放空间
inline static void SystemFree(void* ptr)
{
#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	//Linus下的sbrk，unmmap等
#endif

}



// 小于等于MAX_BYTES，就找thread cache申请
// 大于MAX_BYTES，就直接找page cache或者系统堆申请
static const size_t MAX_BYTES = 256 * 1024;

// thread cache 和 central cache自由链表哈希桶的表大小
static const size_t NFREELIST = 208;

//page cache的最大页号
static const size_t NPAGES = 129;

//每个页大小为8KB 2^13
static const size_t PAGE_SHIFT = 13;

static void*& NextObj(void* obj)
{
	return *(void**)obj;
}


// 管理切分好的小对象的自由链表
class FreeList
{
public:

	void Push(void* obj)	//等同于ObjectPool的new
	{
		assert(obj);

		//头插
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

	void* Pop()	//等同于ObjectPool的Delete
	{
		assert(_freeList);

		//头删
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


// 计算对象大小的对齐映射规则
class SizeClass
{
public:
	// 整体控制在最多10%左右的内碎片浪费
	// [1,128]					8byte对齐       freelist[0,16)		
	// [128+1,1024]				16byte对齐		freelist[16,72)
	// [1024+1,8*1024]			128byte对齐		freelist[72,128)
	// [8*1024+1,64*1024]		1024byte对齐	freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte对齐  freelist[184,208)

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
			//大于256KB的按页对齐
			return _Roundup(bytes, 1 << PAGE_SHIFT);
		}
	}


	//计算对应的自由链表桶
	static inline size_t _Index(size_t bytes, size_t align_shift)
	{
		return (((bytes)+(1 << align_shift) - 1) >> align_shift) - 1;
	}

	static inline size_t Index(size_t bytes)
	{
		assert(bytes <= MAX_BYTES);

		//每个区间有多少个自由链表
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


		// [2, 512]，一次批量移动多少个对象的(慢启动)上限值
		// 小对象一次批量上限高
		// 小对象一次批量上限低
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

	//central cache一次向page cache申请多少个页
	static size_t NumMovePage(size_t size)
	{
		size_t num = NumMoveSize(size);	//一次申请多少个对象
		size_t npage = num * size;		//num个size大小的对象的字节数

		npage >>= PAGE_SHIFT;			//转换成对应的页数
		if (npage == 0)					//至少给一页
		{
			return 1;
		}
		return npage;
	}
};

// 管理以页为单位的大块内存块
struct Span
{
	PAGE_ID _pageId = 0;	//起始页号
	size_t _n = 0;			//页数

	Span* _prev = nullptr;	//双向链表结构
	Span* _next = nullptr;	

	size_t _objSize = 0;		//自由链表管理的每个对象大小
	size_t _useCount = 0;		//分配给自由链表的小块内存的块数
	void* _freeList = nullptr;	//管理的自由链表

	bool _isUse = false;		//标记是否在被使用
};


// 是一个双向循环链表
// 从双链表删除的span会还给下一层的page cache
// 相当于只是把这个span从双链表中移除
// 因此不需要对删除的span进行delete操作
// span要上锁
// 多个threadcache会对span产生锁竞争
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
	std::mutex _mtx; // 桶锁
};