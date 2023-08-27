#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<functional>
using namespace std;

// 哈希函数
template<class K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
	}
};

//特化
template<>
struct HashFunc<string>
{
	size_t operator()(const string& key)
	{
		size_t hash = 0;
		for (auto ch : key)
		{
			hash *= 131;
			hash += ch;
		}
		return hash;
	}
};

namespace closehash
{
	enum State
	{
		EMPTY,
		EXIST,
		DELETE,
	};

	template<class K,class V>
	struct HashData
	{
		pair<K, V> _kv;
		State _state = EMPTY;
	};

	// hf 哈希函数
	// hashi 哈希表的下标
	template<class K,class V,class Hash = HashFunc<K>>
	class HashTable
	{
		typedef HashData<K, V> Data;
	public:
		HashTable()
			:_n(0)
		{
			_tables.resize(10);
		}

		bool Insert(const pair<K, V>& kv)
		{
			if (Find(kv.first))
			{
				return false;
			}

			// 负载因子为7，若_n>7
			// 则扩容
			if (_n * 10 / _tables.size() >= 7)
			{
				HashTable<K, V, Hash> newHT;
				newHT._tables.resize(_tables.size() * 2);
				for (auto& e : _tables)
				{
					if (e._state == EXIST)
					{
						newHT.Insert(e._kv);
					}
				}
				swap(_tables, newHT._tables);
			}

			Hash hf;
			size_t hashi = hf(kv.first) % _tables.size();
			while (_tables[hashi]._state == EXIST)
			{
				// 线性探测
				hashi++;
				hashi %= _tables.size();
			}

			_tables[hashi]._kv = kv;
			_tables[hashi]._state = EXIST;
			_n++;
			return true;
		}

		Data* Find(const K& key)
		{
			Hash hf;
			size_t hashi = hf(key) % _tables.size();
			while (_tables[hashi]._state != EMPTY)
			{
				if (_tables[hashi]._state == EXIST
					&& _tables[hashi]._kv.first == key)
				{
					return &_tables[hashi];
				}

				hashi++;
				hashi %= _tables.size();
			}

			return nullptr;
		}

		bool Erase(const K& key)
		{
			Data* ret = Find(key);
			if (ret)
			{
				ret->_state = DELETE;
				--_n;
				return true;
			}
			else
			{
				return false;
			}
		}
	private:
		vector<Data> _tables;	//哈希表的存储结构
		int _n;					//_table的有效数据个数
	};

	void TestHT1()
	{
		HashTable<int, int> ht;
		int a[] = { 18, 8, 7, 27, 57, 3, 38, 18 };
		for (auto e : a)
		{
			ht.Insert(make_pair(e, e));
		}

		ht.Insert(make_pair(17, 17));
		ht.Insert(make_pair(5, 5));

		cout << ht.Find(7) << endl;
		cout << ht.Find(8) << endl;

		ht.Erase(7);
		cout << ht.Find(7) << endl;
		cout << ht.Find(8) << endl;
	}

	void TestHT2()
	{
		string arr[] = { "苹果", "西瓜", "香蕉", "草莓", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

		//HashTable<string, int, HashFuncString> countHT;
		HashTable<string, int> countHT;
		for (auto& e : arr)
		{
			HashData<string, int>* ret = countHT.Find(e);
			if (ret)
			{
				ret->_kv.second++;
			}
			else
			{
				countHT.Insert(make_pair(e, 1));
			}
		}

		HashFunc<string> hf;
		cout << hf("abc") << endl;
		cout << hf("bac") << endl;
		cout << hf("cba") << endl;
		cout << hf("aad") << endl;
	}
}

//prev
//namespace buckehash
//{
//	template<class K,class V>
//	struct HashNode
//	{
//		pair<K, V> _kv;
//		HashNode<K, V>* _next;
//
//		HashNode(const pair<K, V>& kv)
//			:_kv(kv)
//			,_next(nullptr)
//		{}
//	};
//
//	template<class K,class V,class Hash = HashFunc<K>>
//	class HashTable
//	{
//		typedef HashNode<K, V> Node;
//	public:
//		HashTable()
//			:_n(0)
//		{
//			_tables.resize(__stl_next_prime(0));
//		}
//		~HashTable()
//		{
//			for (size_t i = 0; i < _tables.size(); i++)
//			{
//				Node* cur = _tables[i];
//				while (cur)
//				{
//					Node* next = cur->_next;
//					delete cur;
//					cur = next;
//				}
//
//				_tables[i] = nullptr;
//			}
//		}
//
//		bool Insert(const pair<K, V>& kv)
//		{
//			if (Find(kv.first))
//				return false;
//
//			// 扩容
//			if (_tables.size() == _n)
//			{
//				vector<Node*> newtables;
//				newtables.resize(__stl_next_prime(_tables.size()), nullptr);
//				for (size_t i = 0; i < _tables.size(); i++)
//				{
//					Node* cur = _tables[i];
//					while (cur)
//					{
//						Node* next = cur->_next;
//
//						size_t hashi = Hash()(cur->_kv.first) % newtables.size();
//
//						cur->_next = newtables[hashi];
//						newtables[hashi] = cur;
//
//						cur = next;
//					}
//					_tables[i] = nullptr;
//				}
//
//				swap(_tables, newtables);
//			}
//
//			size_t hashi = Hash()(kv.first) % _tables.size();
//			// 头插
//			Node* newnode = new Node(kv);
//			newnode->_next = _tables[hashi];
//			_tables[hashi] = newnode;
//			_n++;
//
//			return true;
//		}
//		
//		Node* Find(const K& key)
//		{
//			size_t hashi = Hash()(key) % _tables.size();
//			Node* cur = _tables[hashi];
//			while (cur)
//			{
//				if (cur->_kv.first == key)
//				{
//					return cur;
//				}
//				else
//				{
//					cur = cur->_next;
//				}
//			}
//			return nullptr;
//		}
//
//		bool Erase(const K& key)
//		{
//			size_t hashi = Hash()(key) % _tables.size();
//			Node* prev = nullptr;
//			Node* cur = _tables[hashi];
//			while (cur)
//			{
//				if (cur->_kv.first == key)
//				{
//					// 删除操作
//					if (cur == _tables[hashi])
//					{
//						_tables[hashi] = cur->_next;
//					}
//					else
//					{
//						prev->_next = cur->_next;
//					}
//
//					delete cur;
//					_n--;
//
//					return true;
//				}
//				else
//				{
//					prev = cur;
//					cur = cur->_next;
//				}
//			}
//
//			return false;
//		}
//
//
//		inline unsigned long __stl_next_prime(unsigned long n)
//		{
//			static const int __stl_num_primes = 28;
//			static const unsigned long __stl_prime_list[__stl_num_primes] =
//			{
//				53, 97, 193, 389, 769,
//				1543, 3079, 6151, 12289, 24593,
//				49157, 98317, 196613, 393241, 786433,
//				1572869, 3145739, 6291469, 12582917, 25165843,
//				50331653, 100663319, 201326611, 402653189, 805306457,
//				1610612741, 3221225473, 4294967291
//			};
//
//			for (int i = 0; i < __stl_num_primes; ++i)
//			{
//				if (__stl_prime_list[i] > n)
//				{
//					return __stl_prime_list[i];
//				}
//			}
//
//			return __stl_prime_list[__stl_num_primes - 1];
//		}
//
//	private:
//		vector<Node*> _tables;
//		int _n;
//	};
//	
//	void TestHT1()
//	{
//		HashTable<int, int> ht;
//		int a[] = { 18, 8, 7, 27, 57, 3, 38, 18,17,88,38,28 };
//		for (auto e : a)
//		{
//			ht.Insert(make_pair(e, e));
//		}
//
//		ht.Insert(make_pair(5, 5));
//
//		ht.Erase(17);
//		ht.Erase(57);
//	}
//
//	void TestHT2()
//	{
//		string arr[] = { "苹果", "西瓜", "香蕉", "草莓", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
//
//		//HashTable<string, int, HashFuncString> countHT;
//		HashTable<string, int> countHT;
//		for (auto& e : arr)
//		{
//			auto ret = countHT.Find(e);
//			if (ret)
//			{
//				ret->_kv.second++;
//			}
//			else
//			{
//				countHT.Insert(make_pair(e, 1));
//			}
//		}
//	}
//}

//after
namespace buckehash
{
	template<class T>
	struct HashNode
	{
		T _data;
		HashNode<T>* _next;
		
		HashNode(const T& data)
			:_data(data)
			,_next(nullptr)
		{}
	};

	//前置声明
	template<class K,class T,class Hash,class KeyOfT>
	class HashTable;

	//迭代器
	template<class K,class T,class Ref,class Ptr,class Hash,class KeyOfT>
	struct __HTIterator
	{
		typedef HashNode<T> Node;
		typedef HashTable<K, T, Hash, KeyOfT> HT;
		typedef __HTIterator<K, T, Ref, Ptr, Hash, KeyOfT> Self;

		Node* _node;
		HT* _ht;

		__HTIterator(Node* node,HT* ht)
			:_node(node)
			,_ht(ht)
		{}

		Ref operator*()
		{
			return _node->_data;
		}

		Ptr operator->()
		{
			return &_node->_data;
		}

		bool operator!=(const Self& s)const
		{
			return _node != s._node;
		}

		Self& operator++()
		{
			if (_node->_next)
			{
				_node = _node->_next
			}
			else
			{
				KeyOfT kot;
				Hash hash;
				size_t hashi = hash(kot(_node->_data)) % _ht->_tables.size();
				while (hashi < _ht->_tables.size())
				{
					if (_ht->_tables[hashi])
					{
						_node = _tables[hashi];
						break;
					}
					else
					{
						hashi++;
					}
				}

				if (hashi == _ht->_tables.size())
				{
					_node = nullptr;
				}
			}

			return *this;
		}
	};

	template<class K,class T,class Hash,class KetOfT>
	class HashTable
	{
		typedef HashNode<T> Node;

		template<class K,class T,class Ref,class Ptr,class Hash,class KeyOfT>
		friend struct __HTIterator;

	public:
		typedef __HTIterator<K, T, T&, T*, Hash, KetOfT> iterator;
		typedef __HTIterator<K, T, const T&, const T*, Hash, KetOfT> const_iterator;

		iterator begin()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i])
				{
					return iterator(_tables[i], this);
				}
			}

			return iterator(nullptr, this);
		}

		iterator end()
		{
			return iterator(nullptr, this);
		}

		const_iterator begin()const
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i])
				{
					return const_iterator(_tables[i], this);
				}
			}

			return const_iterator(nullptr, this);

		}

		const_iterator end()const
		{
			return const_iterator(nullptr, this);
		}

		HashTable()
			:_n(0)
		{
			_tables.resize(__stl_next_prime(0));
		}

		~HashTable()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;
					delete cur;
					cur = next;
				}

				_tables[i] = nullptr;
			}
		}

		pair<iterator, bool> Insert(const T& data)
		{
			KetOfT kot;

			iterator it = Find(data);
			if (it != end())
			{
				return make_pair<it, false>;
			}

			if (_tables.size() == _n)
			{
				vector<Node*> newtables;
				newtables.resize(__stl_next_prime(_tables.size()), nullptr);
				for (size_t i = 0; i < _tables.size(); i++)
				{
					Node* cur = _tables[i];
					while (cur)
					{
						Node* next = cur->_next;
						size_t hashi = Hash()(kot(cur->_data)) % newtables.size();

						cur->_next = newtables[hashi];
						newtables[hashi] = cur;

						cur = next;
					}

					_tables[i] = nullptr;
				}

				swap(_tables, newtables);
			}

			Node* newnode = new Node(data);
			size_t hashi = Hash()(kot(data)) % _tables.size();

			newnode->_next = _tables[hashi];
			_tables[hashi] = newnode;
			_n++;
			return make_pair<iterator(newnode, this), true>;
		}

		iterator Find(const K& key)
		{
			KetOfT kot;
			size_t hashi = Hash()(kot(key)) % _tables.size();
			Node* cur = _tables[hashi];
			while (cur)
			{
				if (kot(cur->_data) == key)
				{
					return iterator(cur, this);
				}
				else
				{
					cur = cur->_next;
				}
			}
			
			return end();

		}

		bool Erase(const K& key)
		{
			KetOfT kot;
			size_t hashi = Hash()(kot(key)) % _tables.size();
			Node* prev = nullptr;
			Node* cur = _tables[hashi];
			while (cur)
			{
				if (kot(cur->_data) == key)
				{
					if (cur == _tables[hashi])
					{
						_tables[hashi] = cur->_next;
					}
					else
					{
						prev->_next = cur->_next;
					}
					delete cur;
					_n--;
					return true;
				}
				else
				{
					prev = cur;
					cur = cur->_next;
				}
			}

			return false;
		}

		inline unsigned long __stl_next_prime(unsigned long n)
		{
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				53, 97, 193, 389, 769,
				1543, 3079, 6151, 12289, 24593,
				49157, 98317, 196613, 393241, 786433,
				1572869, 3145739, 6291469, 12582917, 25165843,
				50331653, 100663319, 201326611, 402653189, 805306457,
				1610612741, 3221225473, 4294967291
			};

			for (int i = 0; i < __stl_num_primes; ++i)
			{
				if (__stl_prime_list[i] > n)
				{
					return __stl_prime_list[i];
				}
			}

			return __stl_prime_list[__stl_num_primes - 1];
		}
	private:
		vector<Node*> _tables;
		int _n;
	};
}