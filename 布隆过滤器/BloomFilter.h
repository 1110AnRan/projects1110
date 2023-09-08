#pragma once
#include<bitset>
#include<iostream>
#include<string>
#include<vector>

using namespace std;
namespace bit 
{
	struct BKDRHash
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

	struct APHash
	{
		size_t operator()(const string& key)
		{
			unsigned int hash = 0;
			int i = 0;

			for (auto ch : key)
			{
				if ((i & 1) == 0)
				{
					hash ^= ((hash << 7) ^ (ch) ^ (hash >> 3));
				}
				else
				{
					hash ^= (~((hash << 11) ^ (ch) ^ (hash >> 5)));
				}

				++i;
			}

			return hash;
		}
	};

	struct DJBHash
	{
		size_t operator()(const string& key)
		{
			unsigned int hash = 5381;

			for (auto ch : key)
			{
				hash += (hash << 5) + ch;
			}

			return hash;
		}
	};

	struct JSHash
	{
		size_t operator()(const string& s)
		{
			size_t hash = 1315423911;
			for (auto ch : s)
			{
				hash ^= ((hash << 5) + ch + (hash >> 2));
			}
			return hash;
		}
	};

	template<size_t N,
		size_t X=6,
		class K=string,
		class HashFunc1=BKDRHash,
		class HashFunc2=APHash,
		class HashFunc3=DJBHash,
		class HashFunc4=JSHash>
	class BloomFilter
	{
	public:
		void set(const K& key)
		{
			size_t hash1 = HashFunc1()(key) % (N * X);
			size_t hash2 = HashFunc2()(key) % (N * X);
			size_t hash3 = HashFunc3()(key) % (N * X);
			size_t hash4 = HashFunc4()(key) % (N * X);

			_bs.set(hash1);
			_bs.set(hash2);
			_bs.set(hash3);
			_bs.set(hash4);
		}
		bool test(const K& key)
		{
			size_t hash1 = HashFunc1()(key) % (N * X);
			if (!_bs.test(hash1))
			{
				return false;
			}

			size_t hash2 = HashFunc2()(key) % (N * X);
			if (!_bs.test(hash2))
			{
				return false;
			}

			size_t hash3 = HashFunc3()(key) % (N * X);
			if (!_bs.test(hash3))
			{
				return false;
			}

			size_t hash4 = HashFunc4()(key) % (N * X);
			if (!_bs.test(hash4))
			{
				return false;
			}

			return true;
		}
	private:
		std::bitset<N*X> _bs;
	};
	
	void test_bloomfilter1()
	{
		// 10:46¼ÌĞø
		string str[] = { "Öí°Ë½ä", "ËïÎò¿Õ", "É³Îò¾»", "ÌÆÈı²Ø", "°×ÁúÂí1","1°×ÁúÂí","°×1ÁúÂí","°×11ÁúÂí","1°×ÁúÂí1" };
		BloomFilter<10> bf;
		for (auto& str : str)
		{
			bf.set(str);
		}

		for (auto& s : str)
		{
			cout << bf.test(s) << endl;
		}
		cout << endl;

		srand(time(0));
		for (const auto& s : str)					 
		{
			cout << bf.test(s + to_string(rand())) << endl;
		}
	}

	void test_bloomfilter2()
	{
		srand(time(0));
		const size_t N = 100000;
		BloomFilter<N> bf;

		std::vector<std::string> v1;
		std::string url = "https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html";

		for (size_t i = 0; i < N; ++i)
		{
			v1.push_back(url + std::to_string(i));
		}

		for (auto& str : v1)
		{
			bf.set(str);
		}

		// v2¸úv1ÊÇÏàËÆ×Ö·û´®¼¯£¬µ«ÊÇ²»Ò»Ñù
		std::vector<std::string> v2;
		for (size_t i = 0; i < N; ++i)
		{
			std::string url = "https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html";
			url += std::to_string(999999 + i);
			v2.push_back(url);
		}

		size_t n2 = 0;
		for (auto& str : v2)
		{
			if (bf.test(str))
			{
				++n2;
			}
		}
		cout << "ÏàËÆ×Ö·û´®ÎóÅĞÂÊ:" << (double)n2 / (double)N << endl;

		// ²»ÏàËÆ×Ö·û´®¼¯
		std::vector<std::string> v3;
		for (size_t i = 0; i < N; ++i)
		{
			string url = "zhihu.com";
			url += std::to_string(i+rand());
			v3.push_back(url);
		}

		size_t n3 = 0;
		for (auto& str : v3)
		{
			if (bf.test(str))
			{
				++n3;
			}
		}
		cout << "²»ÏàËÆ×Ö·û´®ÎóÅĞÂÊ:" << (double)n3 / (double)N << endl;
	}
}