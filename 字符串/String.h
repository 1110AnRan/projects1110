#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<string.h>
#include<list>
#include<cassert>
using namespace std;

namespace bit
{
	class string
	{
		typedef char* iterator;
	public:
		friend ostream& operator<<(ostream& out, const bit::string& s);
		//迭代器
		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		//有参构造
		string(const char* str = "")
			:_size(strlen(str))
			, _capacity(_size)
		{

			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}

		// s1.swap(s2)
		void swap(string& s)
		{
			::swap(_size, s._size);
			::swap(_capacity, s._capacity);
			::swap(_str, s._str);
		}

		//拷贝构造
		string(const string& s)
		{
			string tmp(s._str);

			swap(tmp);
		}

		//赋值重载
		string& operator=(const string& s)
		{
			string tmp(s);
			swap(tmp);

			return *this;
		}

		// 移动构造
		string(string&& s)
		{
			cout << "string(const string& s) -- 移动拷贝" << endl;

			swap(s);
		}

		// 移动赋值
		string& operator=(string&& s)
		{
			cout << "string& operator=(string s) -- 移动赋值" << endl;
			swap(s);

			return *this;
		}

		//析构函数
		~string()
		{
			delete[] _str;
			_str = nullptr;
		}

		char& operator[](size_t pos)
		{
			assert(pos < _size);
			return _str[pos];
		}


		void reverse(size_t n)
		{
			if (n > _capacity)
			{
				char* tmp = new char[n + 1];
				strcpy(tmp, _str);
				delete[]_str;
				_str = tmp;

				_capacity = n;
			}
		}

		void push_back(char ch)
		{
			if (_size >= _capacity)
			{
				size_t newcapacity = _capacity == 0 ? 4 : _capacity * 2;
				reverse(newcapacity);
			}

			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
		}

		string& operator+=(char ch)
		{
			push_back(ch);
			return *this;
		}

		const char* c_str()const
		{
			return _str;
		}
	private:
		char* _str = nullptr;
		size_t _size = 0;// _str="12345\0",_size=6
		size_t _capacity = 0;// 不包含最后做标识的\0 _capacity=8
	};
	ostream& operator<<(ostream& out, const bit::string& str)
	{
		size_t i;
		for (i = 0; i < str._size; i++)
		{
			out << str._str[i];
		}
		return out;
	}


	string to_string(int value)
	{
		bool flag = true;
		if (value < 0)
		{
			flag = false;
			value = 0 - value;
		}

		bit::string s;
		while (value > 0)
		{
			int last = value % 10;
			value /= 10;

			s += ('0' + last);
		}

		if (flag == false)
		{
			s += '-';
		}

		std::reverse(s.begin(), s.end());
		return s;
	}
	void Test1()
	{
		bit::string ret;
		ret = bit::to_string(-1234);
		//bit::string ret = bit::to_string(-1234);
		cout << ret;

		//std::string s1("hello world");
		//std::string s2(s1);
		//std::string s3(move(s1));
	}

	void Test2()
	{
		list<bit::string> lt;
		bit::string s1("111111");
		lt.push_back(move(s1));

		lt.push_back(bit::string("222222"));

		lt.push_back("333333");
	}
}