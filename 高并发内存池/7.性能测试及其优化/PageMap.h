#pragma once
#include"Common.h"

//单层基数树
template<int BITS>
class TCMalloc_PageMap1
{
public:
	typedef uintptr_t Number;

	//explicit TCMalloc_PageMap1(void* (*allocator)(size_t))
	explicit TCMalloc_PageMap1()
	{
		//array_ = reinterpret_cast<void**>((*allocator)(sizeof(void*) << BITS));
		size_t size = sizeof(void*) << BITS;//需要开辟数组的总大小
		size_t alignSize = SizeClass::_Roundup(size, 1 << PAGE_SHIFT);//按页对齐后的大小
		array_ = (void**)SystemAlloc(alignSize >> PAGE_SHIFT);//向堆申请空间
		memset(array_, 0, size);
	}


	// Return the current value for KEY.  Returns NULL if not yet set,
	// or if k is out of range.
	void* get(Number k)const
	{
		if ((k >> BITS) > 0)
		{
			return nullptr;
		}
		return array_[k];
	}

	void set(Number k, void* v)
	{
		assert((k >> BITS) == 0);
		array_[k] = v;
	}
private:
	void** array_;//存储页号与span对应关系的映射
	static const int LENGTH = 1 << BITS;//页的数目
};


//二层基数树
template<int BITS>
class TCMalloc_PageMap2
{
public:
	typedef uintptr_t Number;
	//explicit TCMalloc_PageMap2(void* (*allocator)(size_t))
	explicit TCMalloc_PageMap2()
	{
		//allocator_ = allocator;
		memset(root_, 0, sizeof(root_));//清空第一层
		PreAllocateMoreMemory();		//为第二层申请空间
	}
	void set(Number k,void* v)
	{
		const Number i1 = k >> LEAF_BITS;
		const Number i2 = k & (LEAF_LENGTH - 1);
		assert(i1 < ROOT_LENGTH);
		root_[i1]->values[i2] = v;
	}
	void* get(Number k)const
	{
		const Number i1 = k >> LEAF_BITS;
		const Number i2 = k & (LEAF_LENGTH - 1);
		if ((k >> BITS) > 0 || root_[i1] == NULL)
		{
			return NULL;
		}
		return root_[i1]->values[i2];
	}

	bool Ensure(Number start, size_t n)
	{
		for (Number key = start; key <= start + n - 1;)
		{
			const Number i1 = key >> LEAF_BITS;
			if (i1 >= ROOT_LENGTH)
			{
				return false;//页号超出范围
			}
			if (root_[i1] == NULL)
			{
				//Leaf* leaf = reinterpret_cast<Leaf*>((*allocator_)(sizeof(Leaf)));
				//if (leaf == NULL) return false;
				//为第二层开辟空间
				static ObjectPool<Leaf> leafPool;
				Leaf* leaf = (Leaf*)leafPool._new();
				memset(leaf, 0, sizeof(*leaf));
				root_[i1] = leaf;
			}
			// Advance key past whatever is covered by this leaf node
			key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
		}
		return true;
	}
	void PreAllocateMoreMemory()
	{	
		// Allocate enough to keep track of all possible pages
		Ensure(0, 1 << BITS);			//将第二层的空间开辟好
	}
private:
	// Put 32 entries in the root and (2^BITS)/32 entries in each leaf.
	static const int ROOT_BITS - 5;					//第一层占用的比特数
	static const int ROOT_LENGTH = 1 << ROOT_BITS;	//第一层存储元素的个数
	static const int LEAF_BITS = BITS - ROOT_BITS;	//第二层占用的比特数
	static const int LEAF_LENGTH = 1 << LEAF_BITS;	//第二层存储元素的个数
	//第一层数组中存储的元素
	struct Leaf
	{
		void* values[LEAF_LENGTH];
	};
	Leaf* root_[ROOT_LENGTH];//第一层数组Pointers to 32 child nodes
	void* (*allocator_)(size_t);          // Memory allocator
};


//三层基数树	64位的平台
template<int BITS>
class TCMalloc_PageMap3 
{
public:
	typedef uintptr_t Number;
	explicit TCMalloc_PageMap3()
	{
		root_ = NewNode();
	}
	void PreAllocateMoreMemory()
	{}
	bool Ensure(Number start, size_t n)
	{
		for (Number key = start; key <= stary + n - 1;)
		{
			const Number i1 = key >> (INTERIOR_BITS + LEAF_BITS);
			const Number i2 = (key >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
			if (i1 >= INTERIOR_LENGTH || I2 >= INTERIOR_LENGTH)//第一、二层下表超出范围
			{
				return false;
			}
			if (root_->ptrs[i1] == NULL)
			{
				Node* newnode = NewNode();
				if(newnode==NULL)
				{
					return false;
				}
				root_->ptrs[i1] = newnode;
			}
			if (root_->ptrs[i1]->ptrs[i2] == NULL)
			{
				static ObjectPool<Leaf> leafPool;
				Leaf* leaf = leafPool._new();
				if (leaf == NULL)
				{
					return false;
				}
				memset(leaf, 0, sizeof(*leaf));
				root_->ptrs[i1]->ptrs[i2] = reinterpret_cast<Node*>(leaf);
			}
			key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
		}
		return true;
	}
	void* get(Number k)const
	{
		const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
		const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
		const NUmber i3 = k & (LEAF_LENGTH - 1);
		if ((k >> BITS) > 0 || root_->ptrs[i1] == NULL || root_->ptrs[i1]->ptrs[i2] == NULL)
		{
			return NULL;
		}
		return reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2])->values[i3];
	}
	void set(Number k, void* v)
	{
		assert((k >> BITS) > 0);
		const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
		const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
		const Number i3 = k & (LEAF_LENGTH - 1);
		Ensure(k, 1);
		reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2])->values[i3] = v;
	}
private:
	static const int INTERIOR_BITS = (BITS + 2) / 3;	//第一、二层对应页号的比特数
	static const int INTERIOR_LENGTH = 1 << INTERIOR_BITS;//第一、二层对应页号的长度
	static const int LEAF_BITS = BITS - 2 * INTERIOR_BITS;//第三层对应页号的比特数
	static const int LEAF_LENGTH = 1 << LEAF_BITS;		//第三层对应页数的长度
	struct Node
	{
		Node* ptrs[INTERIOR_LENGTH];
	};
	struct Leaf
	{
		void* values[LEAF_LENGTH];
	};
	Node* NewNode()
	{
		static ObjectPool<Node> nodepool;
		Node* result = nodepool._new();
		if (result != NULL)
		{
			memset(result, 0, sizeof(*result));
		}
		return result;
	}
	Node* root_;
};