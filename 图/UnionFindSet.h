#pragma once
#include<vector>
#include<iostream>

class UnionFindSet
{
public:
	UnionFindSet(int size)
		:_set(size,-1)
	{}
	

	//查找下标为x的根
	size_t FindRoot(int x)
	{
		while (_set[x] >= 0)
		{
			x = _set[x];
		}
		return x;
	}

	//把下标为x1和x2的两个元素并起来
	void Union(int x1,int x2)
	{
		int root1 = FindRoot(x1);
		int root2 = FindRoot(x2);

		//如果x1和x2已经是同一个集合，直接return
		//但每个元素初始化均为-1
		//如果用这条语句，则无法并起来
		//if (root1 == root2)
		//	return;

		//否则以任意一个为新的根
		if (root1 != root2)
		{
			_set[root2] += _set[root1];
			_set[root1] = root2;
		}
	}

	//有几个集合
	size_t SetCount()
	{
		size_t Count = 0;
		for (auto index : _set)
		{
			if (index < 0)
			{
				Count++;
			}
		}
		return Count;
	}
private:
	std::vector<int> _set;
};
