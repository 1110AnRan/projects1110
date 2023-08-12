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

	int _bf;//balance factor(�������߶�-�������߶�)

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

		//1.����ƽ������
		while (parent) // parentΪ�գ�Ҳ�͸��µ���
		{
			// �������ң�parent->bf++;
			// ��������parent->bf--;
			if (parent->_left == cur)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}

			// �Ƿ�����������ݣ������ĸ߶��Ƿ�仯
			// 1��parent->_bf == 0˵��֮ǰparent->_bf�� 1 ���� -1
			// ˵��֮ǰparentһ�߸�һ�ߵͣ���β������ϰ����Ǳߣ�parent���������߶Ȳ��䣬����Ҫ�������ϸ���
			// 2��parent->_bf == 1 �� -1 ˵��֮ǰ��parent->_bf == 0������һ���ߣ����ڲ���һ�߸����ˣ�
			// parent���������߶ȱ��ˣ��������ϸ���
			// 3��parent->_bf == 2 �� -2��˵��֮ǰparent->_bf == 1 ���� -1�����ڲ������ز�ƽ�⣬Υ������
			// �͵ش���--��ת

			// ��ת��
			// 1��������������Ҹ߶Ȳ�����1
			// 2����ת�����м�����������������
			// 3�����µ������ӽڵ��ƽ������
			// 4������������ĸ߶ȸ�����ǰ����һ��
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
				// ��ʼ��ת��ת
				// ���ݸ�����ת���ص�ѡ����ת�ķ�ʽ
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


	//��������
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

	//��������
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

	//����������������
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		//������⣡����
		if (bf == -1) // subLR����������
		{
			subL->_bf = 0;
			parent->_bf = 1;
			subLR->_bf = 0;
		}
		else if (bf == 1) // subLR����������
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;	
		}
		else if (bf == 0) // subLR�Լ���������
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

	//������������
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);


		//������⣡����
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
			cout << root->_kv.first << "ƽ�������쳣" << endl;
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
