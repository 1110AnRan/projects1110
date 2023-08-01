#pragma once
#include<vector>
#include<iostream>

class UnionFindSet
{
public:
	UnionFindSet(int size)
		:_set(size,-1)
	{}
	

	//�����±�Ϊx�ĸ�
	size_t FindRoot(int x)
	{
		while (_set[x] >= 0)
		{
			x = _set[x];
		}
		return x;
	}

	//���±�Ϊx1��x2������Ԫ�ز�����
	void Union(int x1,int x2)
	{
		int root1 = FindRoot(x1);
		int root2 = FindRoot(x2);

		//���x1��x2�Ѿ���ͬһ�����ϣ�ֱ��return
		//��ÿ��Ԫ�س�ʼ����Ϊ-1
		//�����������䣬���޷�������
		//if (root1 == root2)
		//	return;

		//����������һ��Ϊ�µĸ�
		if (root1 != root2)
		{
			_set[root2] += _set[root1];
			_set[root1] = root2;
		}
	}

	//�м�������
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
