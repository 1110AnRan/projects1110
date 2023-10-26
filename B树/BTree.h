#pragma once
#include<utility>
#include<iostream>
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

template<class K,size_t M>
struct BTreeNode
{
	K _keys[M];

	//һ���ڵ�ĺ��ӽڵ��key�ĸ�����1
	BTreeNode<K, M>* _subs[M + 1];
	BTreeNode<K, M>* _parent;
	size_t _n;//ʵ��keyֵ�ĸ���
	BTreeNode()
	{
		size_t i = 0;
		for (; i < M; ++i)
		{
			_keys[i] = K();
			_subs[i] = nullptr;
		}

		_subs[M] = nullptr;
		_parent = nullptr;
		_n = 0;
	}
};


template<class K,size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	pair<Node*, int> Find(const K& key)
	{
		Node* parent = nullptr;
		Node* cur = _root;

		while (cur)
		{
			size_t i = 0;
			while (i < cur->_n)
			{
				if (cur->_keys[i] < key)
				{
					++i;
				}
				else if (cur->_keys[i] > key)
				{
					break;
				}
				else
				{
					return make_pair(cur, i);
				}
			}

			parent = cur;
			cur = cur->_subs[i];
		}

		return make_pair(parent, -1);
	}

	void Insertkey(Node* cur, K& key, Node* child)
	{
		size_t end = cur->_n - 1;
		while (end >= 0)
		{
			if (cur->_keys[end] > key)
			{
				cur->_keys[end + 1] = cur->_keys[end];
				cur->_subs[end + 2] = cur->_subs[end + 1];
				--end;
			}
			else
			{
				break;
			}
		}

		cur->_keys[end + 1] = key;
		cur->_subs[end + 2] = child;
		if (child)
		{
			child->_parent = cur;
		}

		cur->_n++;
	}
	
	bool Insert(const K& key)
	{
		if (_root == nullptr)
		{
			_root = new Node;
			_root->_keys[0] = key;
			_root->_n++;

			return true;
		}

		//���keyֵ�Ѵ��ڣ���ֱ��return
		pair<Node*, int> ret = Find(key);
		if (ret.second >= 0)
		{
			return false;
		}

		//keyֵ�����ڣ���ret�Ѿ��ҵ���ǰ�Ĳ���ڵ��λ��
		Node* cur = ret.first;
		K newkey = key;
		Node* child = nullptr;
		//����������
		while (1)
		{
			Insertkey(cur, newkey, child);
			//���˲ŷ���
			//û����ֱ��break;
			if (cur->_n < M)
			{
				return true;
			}
			//���Ѳ���
			else
			{
				size_t mid = M / 2;
				Node* brother = new Node;
				size_t i = mid + 1;			//cur�ڵ���Ұ벿�ַ��ѳ�ȥ
				size_t j = 0;				//brother�ڵ�ĸ���
				for (; i <= M - 1; ++i)
				{
					brother->_keys[j] = cur->_keys[i];
					brother->_subs[j] = cur->_subs[i];
					++j;
					if (cur->_subs[i])
					{
						cur->_subs[i]->_parent = brother;
					}

					cur->_keys[i] = K();
					cur->_subs[i] = nullptr;
				}

				brother->_subs[j] = cur->_subs[i];
				if (cur->_subs[i])
				{
					cur->_subs[i]->_parent = brother;
				}
				cur->_subs[i] = nullptr;

				brother->_n = j;
				cur->_n -= (brother->_n + 1);

				K midkey = cur->_keys[mid];
				cur->_keys[mid] = K();

				if (cur->_parent == nullptr)
				{
					_root = new Node;
					_root->_keys[0] = midkey;
					_root->_n = 1;
					_root->_subs[0] = cur;
					_root->_subs[1] = brother;

					cur->_parent = _root;
					brother->_parent = _root;
					break;
				}
				else
				{
					// ת������parent->parent ȥ����parent->[mid] �� brother
					newkey = midkey;
					child = brother;

					cur = cur->_parent;
				}
			}
		}


		return true;
	}

	void _InOrder(Node* cur)
	{
		if (cur == nullptr)
			return;

		size_t i = 0;
		for (; i < cur->_n; ++i)
		{
			_InOrder(cur->_subs[i]);
			cout << cur->_keys[i] << " ";
		}

		_InOrder(cur->_subs[i]);
	}

	void InOrder()
	{
		_InOrder(_root);
	}

private:
	Node* _root = nullptr;
};

void TestBtree()
{
	int a[] = { 53, 139, 75, 49, 145, 36, 101 };
	BTree<int, 3> t;
	//cout << t.Find(139).first << " " << t.Find(139).second << endl;
	for (auto e : a)
	{
		t.Insert(e);
	}
	t.InOrder();
}