#pragma once

namespace K
{
	template<class K>
	struct BSTreeNode
	{
		BSTreeNode<K>* _left;
		BSTreeNode<K>* _right;
		K _key;

		BSTreeNode(const K& key)
			:_key(key)
			,_left(nullptr)
			,_right(nullptr)
		{}
	};

	template<class K>
	class BSTree
	{
		typedef BSTreeNode<K> Node;
	public:
		BSTree()
			:_root(nullptr)
		{}



		BSTree<K>& operator=(BSTree<K> t)
		{
			swap(_root, t._root);
			return *this;
		}

		~BSTree()
		{
			Destory(_root);
			_root = nullptr;
		}

		bool Insert(const K& key)
		{
			if (_root == nullptr)
			{
				_root = new Node(key);
				return true;
			}

			Node* parent = nullptr;
			Node* cur = _root;

			while (cur)
			{
				if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else
				{
					return false;
				}
			}

			cur = new Node(key);
			if (parent->_key > key)
			{
				parent->_left = cur;
			}
			else
			{
				parent->_right = cur;
			}
			return true;
			
		}

		bool Find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (cur->_key > key)
				{
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					cur = cur->_right;
				}
				else
				{
					return true;
				}
			}

			return false;
		}

		bool Erase(const K& key)
		{
			Node* parent = nullptr;
			Node* cur = _root;
			while (cur)
			{
				if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else
				{
					//三种情况
					//1.左子树为空
					//2.右子树为空
					//3.左右子树都不为空
					if (cur->_left == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->_right;
						}
						else
						{
							if (parent->_left == cur)
							{
								parent->_right = cur->_right;
							}
							else
							{
								parent->_right = cur->_right;
							}
						}
						delete cur;
					}
					else if (cur->_right == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->_left;
						}
						else
						{
							if (parent->_left == cur)
							{
								parent->_left = cur->_left;
							}
							else
							{
								parent->_right = cur->_left;
							}
						}
						delete cur;
					}
					else
					{
						Node* parent = cur;
						Node* minRight = cur->_right;
						while (minRight->_left)
						{
							parent = minRight;
							minRight = minRight->_left;
						}

						cur->_key = minRight->_key;
						if (minRight == parent->_left)
						{
							parent->_left = minRight->_right;
						}
						else
						{
							parent->_right = minRight->_right;
						}
						delete minRight;
					}

					return true;
				}
			}

			return false;
		}

		void InOrder()
		{
			_InOrder(_root);
			cout << endl;
		}

		bool InsertR(const K& key)
		{
			return _InsertR(_root, key);
		}

		bool FindR(const K& key)
		{
			return _FindR(_root, key);
		}

		bool EraseR(const K& key)
		{
			return _EraseR(_root, key);
		}

	private:
		void Destory(Node* root)
		{
			if (root == nullptr)
				return;

			Destory(root->_left);
			Destory(root->_right);
			delete root;
		}

		Node* Copy(Node* root)
		{
			if (root == nullptr)
				return nullptr;

			Node* newroot = new Node(root->_key);
			newroot->_left = Copy(root->_left);
			newroot->_right = Copy(root->_right);

			return newroot;
		}

		bool _EraseR(Node*& root, const K& key)
		{
			if (root == nullptr)
			{
				return false;
			}

			if (root->_key > key)
			{
				return _EraseR(root->_left, key);
			}
			else if (root->_key < key)
			{
				return _EraseR(root->_right, key);
			}
			else
			{
				Node* del = root;
				if (root->_right == nullptr)
				{
					root = root->_left;
				}
				else if (root->_left == nullptr)
				{
					root = root->_right;
				}
				else
				{
					Node* minRight = root->_right;
					while (minRight->_left)
					{
						minRight = minRight->_left;
					}

					swap(root->_key, minRight->_key);

					// 转换成在子树中去删除节点
					return _EraseR(root->_right, key);
				}

				delete del;

				return true;
			}
			return false;
		}

		bool _InsertR(Node*& root, const K& key)
		{
			if (root == nullptr)
			{
				root = new Node(key);
				return true;
			}

			if (root->_key > key)
				return _InsertR(root->_left, key);
			else if (root->_key < key)
				return _InsertR(root->_right, key);
			else
				return false;
		}

		bool _FindR(Node* root, const K& key)
		{
			if (root == nullptr)
			{
				return false;
			}

			if (root->_key > key)
				return _FindR(root->_left, key);
			else if (root->_key < key)
				return _FindR(root->_right, key);
			else
				return true;
		}

		void _InOrder(Node* root)
		{
			if (root == nullptr)
				return;

			_InOrder(root->_left);
			cout << root->_key << " ";
			_InOrder(root->_right);
		}

		Node* _root = nullptr;
	};
}


namespace KV
{
	template<class K,class V>
	struct BSTreeNode
	{
		BSTreeNode<K, V>* _left;
		BSTreeNode<K, V>* _right;
		K _key;
		V _value;

		BSTreeNode(const K& key , const V& value)
			:_key(key)
			,_value(value)
			,_left(nullptr)
			,_right(nullptr)
		{}
	};

	//BSTree<string, vector<string>> bookInfo;
	
	template<class K,class V>
	class BSTree
	{
		typedef BSTreeNode<K, V> Node;
	public:
		bool Insert(const K& key, const V& value)
		{
			if (root == nullptr)
			{
				root = new Node(key, value);
				return true;
			}

			Node* parent = nullptr;
			Node* cur = root;
			while (cur)
			{
				if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else
				{
					return false;
				}
			}

			cur = new Node(key, value);
			if (parent->_key > key)
			{
				parent->_left = cur;
			}
			else if (parent->_key < key)
			{
				parent->_right = cur;
			}
			return true;
		}

		Node* Find(const K& key)
		{
			Node* cur = root;
			while (cur)
			{
				if (cur->_key > key)
				{
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					cur = cur->_right;
				}
				else
				{
					return cur;
				}
			}
			return nullptr;
		}

		void InOrder()
		{
			_InOrder(root);
		}

		void _InOrder(Node* root)
		{
			if (root == nullptr)
				return;

			_InOrder(root->_left);
			cout << root->_key << ":" << root->_value << endl;
			_InOrder(root->_right);
		}
	private:
		Node* root = nullptr;
	};
}

void TestBSTree1()
{
	K::BSTree<int> t;
	int a[] = { 8, 3, 1, 10, 6, 4, 7, 14, 13 };
	for (auto i : a)
	{
		t.Insert(i);
	}

	t.InOrder();

	K::BSTree<int> copyt(t);
	copyt.InOrder();

	//t.EraseR(14);
	//t.InOrder();

	//t.EraseR(3);
	//t.InOrder();

	//t.EraseR(8);
	//t.InOrder();

	for (auto e : a)
	{
		t.EraseR(e);
		t.InOrder();
	}
}

void TestBSTree2()
{
	// 词库中单词都放进这个搜索树中
	// Key的搜索模型，判断在不在？
	// 场景：检查单词拼写是否正确/车库出入系统/...
	//K::BSTree<string> dict;

	// Key/Value的搜索模型,通过Key查找或修改Value
	KV::BSTree<string, string> dict;
	dict.Insert("sort", "排序");
	dict.Insert("string", "字符串");
	dict.Insert("left", "左边");
	dict.Insert("right", "右边");

	string str;
	while (cin >> str)
	{
		KV::BSTreeNode<string, string>* ret = dict.Find(str);
		if (ret)
		{
			cout << ret->_value << endl;
		}
		else
		{
			cout << "无此单词" << endl;
		}
	}
}

void TestBSTree3()
{
	// 统计水果出现的次数
	string arr[] = { "苹果", "西瓜", "香蕉", "草莓","苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

	KV::BSTree<string, int> countTree;
	for (auto e : arr)
	{
		auto* ret = countTree.Find(e);
		if (ret == nullptr)
		{
			countTree.Insert(e, 1);
		}
		else
		{
			ret->_value++;
		}
	}

	countTree.InOrder();
}

