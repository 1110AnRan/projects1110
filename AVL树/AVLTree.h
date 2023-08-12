#pragma once
#include<assert.h>
#include<time.h>

template<class K, class V>
struct AVLTreeNode
{
	pair<K, V> _kv;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	int _bf;//balance factor(右子树高度-左子树高度)

	AVLTreeNode(const pair<K,V>& kv)
		:_kv(kv)
		,_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_bf(0)
	{}
};

template<class K,class V>
struct AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
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
				return false;
			}
		}

		cur = new Node(kv);
		if (parent->_kv.first > kv.first)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}

		//1.更新平衡因子
		while (parent) // parent为空，也就更新到根
		{
			// 新增在右，parent->bf++;
			// 新增在左，parent->bf--;
			if (parent->_left == cur)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}

			// 是否继续更新依据：子树的高度是否变化
			// 1、parent->_bf == 0说明之前parent->_bf是 1 或者 -1
			// 说明之前parent一边高一边低，这次插入填上矮的那边，parent所在子树高度不变，不需要继续往上更新
			// 2、parent->_bf == 1 或 -1 说明之前是parent->_bf == 0，两边一样高，现在插入一边更高了，
			// parent所在子树高度变了，继续往上更新
			// 3、parent->_bf == 2 或 -2，说明之前parent->_bf == 1 或者 -1，现在插入严重不平衡，违反规则
			// 就地处理--旋转

			// 旋转：
			// 1、让这颗子树左右高度不超过1
			// 2、旋转过程中继续保持他是搜索树
			// 3、更新调整孩子节点的平衡因子
			// 4、让这颗子树的高度跟插入前保持一致
			if (parent->_bf == 0)
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				// 开始旋转旋转
				// 根据各个旋转的特点选定旋转的方式
				if (parent->_bf == 2 && cur->_bf == 1)
				{
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == -1)
				{
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					RotateRL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateLR(parent);
				}
				else
				{
					assert(false);
				}

				break;
			}
			
			else
			{
				assert(false);
			}


		}

		return true;
	}


	//左旋操作
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		Node* pparent = parent->_parent;
		subR->_left = parent;
		parent->_parent = subR;

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
			else if (pparent->_right == parent)
			{
				pparent->_right = subR;
			}
			subR->_parent = pparent;
		}

		subR->_bf = parent->_bf = 0;

	}

	//右旋操作
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node* pparent = parent->_parent;
		subL->_right = parent;
		parent->_parent = subL;


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
		subL->_bf = parent->_bf = 0;
	}

	//先左旋再右旋操作
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		//难以理解！！！
		if (bf == -1) // subLR左子树新增
		{
			subL->_bf = 0;
			parent->_bf = 1;
			subLR->_bf = 0;
		}
		else if (bf == 1) // subLR右子树新增
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;	
		}
		else if (bf == 0) // subLR自己就是新增
		{
			parent->_bf = 0;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	//先右旋再左旋
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);


		//难以理解！！！
		if (bf == 1)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else if (bf == 0)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else
		{
			assert(false);
		}

	}

	void InOrder()
	{
		_InOrder(_root);
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
			return;

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

	int Height(Node* root)
	{
		if (root == nullptr)
			return 0;

		int leftheight = Height(root->_left);
		int rightheight = Height(root->_right);

		return leftheight > rightheight ? leftheight + 1 : rightheight + 1;
	}

	bool IsBalance()
	{
		return IsBalance(_root);
	}
	
	bool IsBalance(Node* root)
	{
		if (root == nullptr)
			return true;

		int leftheight = Height(root->_left);
		int rightheight = Height(root->_right);

		if (rightheight - leftheight != root->_bf)
		{
			cout << root->_kv.first << "平衡因子异常" << endl;
			return false;
		}

		return abs(leftheight - rightheight) < 2
			&& IsBalance(root->_left)
			&& IsBalance(root->_right);
	}


private:
	Node* _root = nullptr;
};

void TestAVLTree1()
{
	//int a[] = { 8, 3, 1, 10, 6, 4, 7, 14, 13 };
	//int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (auto e : a)
	{
		t.Insert(make_pair(e, e));
	}

	t.InOrder();

	cout << t.IsBalance() << endl;
}

void TestAVLTree2()
{
	srand(time(0));
	const size_t N = 100000;
	AVLTree<int, int> t;
	for (size_t i = 0; i < N; ++i)
	{
		size_t x = rand();
		t.Insert(make_pair(x, x));
		//cout << t.IsBalance() << endl;
	}

	t.InOrder();

	cout << t.IsBalance() << endl;
}
