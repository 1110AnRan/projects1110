#pragma once
#include<vector>
#include<iostream>
using namespace std;


namespace bit
{
	template<size_t N>
	class BitSet
	{
	public:
		BitSet()
		{
			_bitset.resize((N >> 3) + 1, 0);
		}

		void SetBit(size_t x)
		{
			size_t index = x >> 3;
			size_t num = x % 8;

			_bitset[index] |= (1 << num);
		}

		void RemoveBit(size_t x)
		{
			size_t index = x >> 3;
			size_t num = x % 8;

			_bitset[index] &= ~(1 << num);
		}

		bool TestBit(size_t x)
		{
			size_t index = x >> 3;
			size_t num = x % 8;

			return (_bitset[index] & (1 << num)) != 0 ? true : false;
		}
	private:
		vector<char> _bitset;
	};

	void test_bitset()
	{
		BitSet<0xffffffff> bs2;
		bs2.SetBit(10);
		bs2.SetBit(10000);
		bs2.SetBit(8888);

		cout << bs2.TestBit(10) << endl;
		cout << bs2.TestBit(10000) << endl;
		cout << bs2.TestBit(8888) << endl;
		cout << bs2.TestBit(8887) << endl;
		cout << bs2.TestBit(9999) << endl << endl;

		bs2.RemoveBit(8888);
		bs2.SetBit(8887);

		cout << bs2.TestBit(10) << endl;
		cout << bs2.TestBit(10000) << endl;
		cout << bs2.TestBit(8888) << endl;
		cout << bs2.TestBit(8887) << endl;
		cout << bs2.TestBit(9999) << endl;
	}
}