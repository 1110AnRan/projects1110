#pragma once

enum Colour
{
	RED,
	BLACK,
};

template<class K,class V>
struct RBTreeNode
{
	pair<K, V> _kv;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	Colour _col;

	RBTreeNode(const pair<K,V>& kv)
		:_kv(kv)
		,_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_col(RED)
	{}
};

template<class K,class V,class Ref,class Ptr>
struct __RBTreeIterator
{
	typedef RBTreeNode<K, V> Node;
	typedef __RBTreeIterator<K, V, Ref, Ptr> Self;
	typedef __RBTreeIterator<K, V, K&, K*> iterator;
	Node* _node;

	__RBTreeIterator(Node* node)
		:_node(node)
	{}


	__RBTreeIterator(const iterator& s)
		:_node(s._node)
	{}

	V& operator*()
	{
		return _node->_kv.second;
	}

	Ptr& operator->()
	{
		return &_node->_kv.second;
	}

	Self& operator++()
	{
		if (_node->_right)
		{
			Node* min = _node->_right;
			while (min->_left)
			{
				min = min->_left;
			}

			_node = min;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_right)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}

			_node = parent;
		}

		return *this;
	}

	Self& operator--()
	{
		if (_node->_left)
		{
			Node* min = _node->_left;
			while (min->_right)
			{
				min = min->_right;
			}

			_node = min;
		}
		else
		{
			Node* cur = _node;
			Node* parent = _node->_parent;
			while (parent && cur == parent->_left)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}

			_node = parent;
		}
		return *this;
	}

	bool operator!=(const Self& s) const
	{
		return _node != s._node;
	}

	bool operator==(const Self& s) const
	{
		return _node == s._node;
	}
};


//
template<class K,class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	typedef __RBTreeIterator<K, V, K&, K*> iterator;
	typedef __RBTreeIterator<K, const V, const K&, const K*> const_iterator;

	iterator begin()
	{
		Node* left = _root;
		while (left && left->_left)
		{
			left = left->_left;
		}

		return iterator(left);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	const_iterator begin() const
	{
		Node* left = _root;
		while (left && left->_left)
		{
			left = left->_left;
		}

		return const_iterator(left);
	}

	const_iterator end() const
	{
		return const_iterator(nullptr);
	}

	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			_root->_col = BLACK;
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				cout << "该树中已存在该数据!无需再进行插入!" << endl;
				return false;
			}
		}

		cur = new Node(kv);
		cur->_col = RED;
		if (parent->_kv.first > cur->_kv.first)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}



		while (parent && parent->_col == RED) 
		{
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left)
			{
				Node* uncle = grandfather->_right;
				// 情况一 uncle存在且为红
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						//情况二
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//情况三
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;

					}

					break;
				}
			}
			else//(parent=grandfather->right)
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					//   g                
					//      p
					//         c
					if (cur == parent->_left)
					{
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//   g                
						//      p
						//   c
						RotateL(grandfather);
						grandfather->_col = RED;
						parent->_col = BLACK;
					}

					break;
				}
			}
		}

		_root->_col = BLACK;

		return true;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		if (subRL)
		{
			subRL->_parent = parent;
		}

		Node* pparent = parent->_parent;
		parent->_right = subRL;
		parent->_parent = subR;
		subR->_left = parent;

		if (pparent == nullptr)
		{
			_root = subR;
			_root->_parent = nullptr;
		}
		else
		{
			if (pparent->_left == parent)
			{
				pparent->_left = subR;
			}
			else
			{
				pparent->_right = subR;
			}
			subR->_parent = pparent;
		}

	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		if (subLR)
		{
			subLR->_parent = parent;
		}

		Node* pparent = parent->_parent;
		parent->_parent = subL;
		parent->_left = subLR;
		subL->_right = parent;

		if (pparent == nullptr)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (pparent->_left == parent)
			{
				pparent->_left = subL;
			}
			else
			{
				pparent->_right = subL;
			}
			subL->_parent = pparent;
		}

	}

	V Find(const K& k)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first > k)
			{
				cur = cur->_left;
			}
			else if(cur->_kv.first<k)
			{
				cur = cur->_right;
			}
			else
			{
				return cur->_kv.second;
			}
		}
		return -1;
	}

	void Inorder()
	{
		_Inorder(_root);
	}

	void _Inorder(Node* root)
	{
		if (root == nullptr)
			return;
		
		_Inorder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_Inorder(root->_right);
	}
private:
	Node* _root = nullptr;
};

void TestRBTree1()
{
	RBTree<int, int> R;
	srand(time(nullptr));
	int begin1 = clock();
	for (int i = 0; i < 10000; i++)
	{
		int r = rand() % 100000;
		R.Insert(make_pair(r, r));
	}
	int end1 = clock();


	int begin2 = clock();
	R.Inorder();
	int end2 = clock();
	cout << end1 - begin1 << endl;
	cout << end2 - begin2 << endl;


	//int begin2 = clock();
	//int find = R.Find(32101);
	//int end2 = clock();
	//cout << end2 - begin2 << endl;



}