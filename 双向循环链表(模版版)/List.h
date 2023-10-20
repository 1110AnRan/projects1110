#pragma once
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <vector>
using std::cout;
using std::endl;
namespace bit
{
	template<class T>
	struct list_node
	{
		list_node* _next;
		list_node* _prev;
		T _data;

		list_node(const T& data)
			:_next(nullptr)
			,_prev(nullptr)
			,_data(data)
		{}

		list_node(T&& data)
			:_next(nullptr)
			,_prev(nullptr)
			,_data(std::forward<T>(data))
		{}
	};

	template<class T,class Ref,class Ptr>
	struct __list_iterator
	{
		//typedef bidirectional_iterator_tag iterator_category;//std中list容器的迭代器的类型
		//typedef T value_type;					
		//typedef Ptr pointer;
		//typedef Ref reference;
		//typedef list_node<T>* link_type;
		//typedef size_t size_type;
		//typedef ptrdiff_t difference_type;


		typedef list_node<T> node;
		typedef __list_iterator<T, Ref, Ptr> Self;
		node* _pnode;

		__list_iterator(node*p)
			:_pnode(p)
		{}

		Ptr operator->()
		{
			return &_pnode->_data;
		}
		
		Ref operator*()
		{
			return _pnode->_data;
		}

		//++it; 无需拷贝
		Self& operator++()
		{
			_pnode = _pnode->_next;
			return *this;
		}

		//it++ 为it先创建一个副本，再对it进行++，返回值为it的副本
		Self operator++(int)
		{
			Self tmp(*this);
			_pnode = _pnode->_next;
			return tmp;
		}

		//--运算符同理
		Self& operator--()
		{
			_pnode = _pnode->_next;
			return *this;
		}

		bool operator!=(const Self& it)const
		{
			return _pnode != it._pnode;
		}

		bool operator==(const Self& it)const
		{
			return _pnode == it._pnode;
		}

	};

	template<class T>
	class list
	{
		typedef list_node<T> node;
	public:
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;


		// 反向迭代器
		//typedef ReverseIterator<iterator, T&, T*> reverse_iterator;
		//typedef ReverseIterator<const_iterator, const T&, const T*> const_reverse_iterator;

		const_iterator begin() const
		{
			return const_iterator(_head->_next);
		}

		const_iterator end()const
		{
			return const_iterator(_head);
		}

		iterator begin()
		{
			return iterator(_head->_next);
		}

		iterator end()
		{
			return iterator(_head);
		}

		void empty_initialize()
		{
			_head = new node(T());
			_head->_next = _head;
			_head->_prev = _head;

			_size = 0;
		}

		// 无参构造	
		list()
		{
			empty_initialize();
		}

		list(const list<T>& lt)
		//list(const list& lt)//不建议
		{
			empty_initialize();

			list<T>tmp(lt.begin(), lt.end());
			swap(tmp);
		}

		list<T>& operator=(list<T>& lt)
		{
			swap(lt);
			return *this;
		}

		template<class InputIterator>
		list(InputIterator first, InputIterator last)
		{
			empty_initialize();

			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		//initializer_list C++11新特性
		list(std::initializer_list<T> il)
		{
			empty_initialize();

			for (auto e : il)
			{
				push_back(e);
			}
		}


		void swap(list<T>& lt)
		{
			std::swap(_head, lt._head);
			std::swap(_size, lt._size);
		}


		size_t size()const
		{
			return _size;
		}

		bool empty()const
		{
			return _size == 0;
		}
	

		~list()
		{
			clear();

			delete _head;
			_head = nullptr;
		}


		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		//删除当前pos位置的节点，并返回指向pos的下一个节点的位置
		iterator erase(iterator pos)
		{
			node* cur = pos._pnode;
			node* prev = cur->_prev;
			node* next = cur->_next;

			delete cur;
			prev->_next = next;
			next->_prev = prev;
			--_size;

			return iterator(next);
		}

		void push_back(const T& data)
		{
			insert(end(), data);
		}

		void push_front(const T& data)
		{
			insert(begin(), data);
		}

		void pop_front()
		{
			erase(begin());
		}


		void pop_back()
		{
			erase(--end());
		}


		//在pos位置前插入
		iterator insert(iterator pos, const T& data)
		{
			node* newnode = new node(data);
			node* cur = pos._pnode;
			node* prev = cur->_prev;

			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;

			++_size;
			return iterator(newnode);
		}

		iterator insert(iterator pos, T&& data)
		{
			node* newnode = new node(std::forward<T>(data));
			node* cur = pos._pnode;
			node* prev = cur->_prev;

			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;

			++_size;
			return iterator(newnode);

		}
	private:
		node* _head;
		size_t _size;
	};

	void test_list1()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);

		// iterator 1、内嵌类型 2、行为像指针一样 
		list<int>::iterator it = lt.begin();
		while (it != lt.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	//void test_vector1()
	//{
	//	std::vector<int> v;
	//	v.push_back(1);
	//	v.push_back(2);
	//	v.push_back(3);
	//	v.push_back(4);

	//	std::vector<int>::iterator it = v.begin();
	//	while (it != v.end())
	//	{
	//		cout << *it << " ";
	//		++it;
	//	}
	//	cout << endl;
	//}

	void test_list2()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_front(5);
		lt.push_front(6);

		//auto pos = find(lt.begin(), lt.end(), 3);

		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;

		lt.pop_back();
		lt.pop_back();
		lt.pop_front();
		lt.pop_front();

		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_list3()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_front(5);
		lt.push_front(6);

		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
		cout << lt.size() << endl;

		list<int> lt1(lt);
		for (auto e : lt1)
		{
			cout << e << " ";
		}
		cout << endl;

		//lt.clear();
		//for (auto e : lt)
		//{
		//	cout << e << " ";
		//}
		//cout << endl;

		list<int> lt2;
		lt2.push_back(10);
		lt2.push_back(20);
		lt2.push_back(30);
		lt2.push_back(40);
		cout << lt2.size() << endl;

		lt = lt2;
		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
	}


	// const T* p1;
	// T* const p2;
	// const迭代器类似p1的行为，保护指向的对象不被修改，迭代器本身可以修改
	//void print_list(const list<int>& lt)
	//{
	//	// const list<int>::iterator cit = lt.begin();
	//	// 不符合const迭代器的行为，因为他保护迭代器本省不能修改，那么我们也就不能++迭代器
	//}

	void print_list(const list<int>& lt)
	{
		list<int>::const_iterator it = lt.begin();
		while (it != lt.end())
		{
			//it为常量
			// (*it) += 2; // 不能写
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}

	void test_list4()
	{
		list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);

		list<int>::iterator it = lt1.begin();
		while (it != lt1.end())
		{
			(*it) += 2; // 可以写
			cout << *it << " ";
			++it;
		}
		cout << endl;

		print_list(lt1);
	}

	struct Pos
	{
		int _row;
		int _col;

		Pos(int row = 0, int col = 0)
			:_row(row)
			, _col(col)
		{}
	};

	void print_list(const list<Pos>& lt)
	{
		list<Pos>::const_iterator it = lt.begin();
		while (it != lt.end())
		{
			//it->_row++;

			cout << it->_row << ":" << it->_col << endl;

			++it;
		}
		cout << endl;
	}

	void test_list5()
	{
		list<Pos> lt;
		Pos p1(1, 1);
		lt.push_back(p1);
		lt.push_back(p1);
		lt.push_back(p1);
		lt.push_back(Pos(2, 2));
		lt.push_back(Pos(3, 3));

		// int* p  -> *p
		// Pos* p  -> p->
		list<Pos>::iterator it = lt.begin();
		//list<Pos>::iterator it2 = it;
		while (it != lt.end())
		{
			it->_row++;

			//cout << (&(*it))->_row << ":" << (*it)._col << endl;
			cout << it->_row << ":" << it->_col << endl;
			//cout << it.operator->()->_row << ":" << it->_col << endl;

			++it;
		}
		cout << endl;

		print_list(lt);
	}

	void test_list6()
	{
		list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);

		list<int>::iterator it = lt1.begin();
		while (it != lt1.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		/*list<int>::reverse_iterator rit = lt1.rbegin();
		while (rit != lt1.rend())
		{
			cout << *rit << " ";
			++rit;
		}
		cout << endl;*/
	}


	void test_list7()
	{
		list<int> lt1 = { 1, 2, 3, 4, 5 };

		list<int>::iterator it = lt1.begin();
		while (it != lt1.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}
}