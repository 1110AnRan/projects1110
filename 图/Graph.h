#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<queue>
#include<functional>
#include<set>
#include"UnionFindSet.h"
using namespace std;

// �ٽӾ���(V:���������,W:��)
// Direction=true�����ͼΪ����ͼ
// Direction=false�����ͼΪ����ͼ
namespace Matrix
{
	template<class V, class W, W MAX_W = INT_MAX, bool Direction = false>
	class Graph
	{
	public:
		struct Edge
		{
			V _srci;
			V _dsti;
			W _w;

			Edge(const V& srci,const V& dsti,const W& w)
				:_srci(srci)
				,_dsti(dsti)
				,_w(w)
			{}

			bool operator>(const Edge& eg)const
			{
				return _w > eg._w;
			}

			bool operator<(const Edge& eg)const
			{
				return _w < eg._w;
			}

		};


		typedef Graph<V, W, MAX_W, Direction> Self;

		Graph() = default;
		
		Graph(const V* vertexs, size_t n)
		{
			_vertexs.reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				_vertexs.push_back(vertexs[i]);
				_vIndexMap[vertexs[i]] = i;
			}

			// MAX_W ��Ϊ�����ڱߵı�ʶֵ
			_matrix.resize(n);
			for (auto& e : _matrix)
			{
				e.resize(n, MAX_W);
			}
		}

		size_t GetVertexIndex(const V& v)
		{
			auto ret = _vIndexMap.find(v);
			if (ret != _vIndexMap.end())
			{
				return ret->second;
			}
			else
			{
				throw invalid_argument("�����ڵĶ���");
				return -1;
			}
		}

		void _AddEdge(size_t srci, size_t dsti, const W& w)
		{
			_matrix[srci][dsti] = w;
			if (Direction == false)
			{
				_matrix[dsti][srci] = w;
			}
		}

		void AddEdge(const V& src, const V& dst, const W& w)
		{
			size_t srci = GetVertexIndex(src);
			size_t dsti = GetVertexIndex(dst);

			_AddEdge(srci, dsti, w);
		}

		// �����������(�ø�������)
		void BFS(const V& src)
		{
			size_t srcIndex = GetVertexIndex(src);
			vector<bool> visited;
			visited.resize(_vertexs.size(), false);

			queue<int> q;
			q.push(srcIndex);
			visited[srcIndex] = true;
			size_t d = 1;
			size_t dSize = 1;
			while (!q.empty())
			{
				printf("%s��%d�Ⱥ���", src.c_str(), d);

				while (dSize--)
				{
					size_t front = q.front();
					q.pop();
					for (size_t i = 0; i < _vertexs.size(); i++)
					{
						if (visited[i] == false && _matrix[front][i] != MAX_W)
						{
							printf("[%d:%s]", i, _vertexs[i].c_str());
							visited[i] = true;
							q.push(i);
						}
					}
				}
				cout << endl;

				dSize = q.size();
				d++;
			}
			cout << endl;
		}

		void _DFS(size_t srcIndex, vector<bool>& visited)
		{
			printf("[%d:%s]->", srcIndex, _vertexs[srcIndex].c_str());
			visited[srcIndex] = true;

			//srcIndex->i
			for (size_t i = 0; i < _vertexs.size(); i++)
			{
				if (visited[i] == false && _matrix[srcIndex][i] != MAX_W)
				{
					_DFS(i, visited);
				}
			}
		}

		// �����������
		void DFS(const V& src)
		{
			size_t srcIndex = GetVertexIndex(src);
			vector<bool> visited;
			visited.resize(_vertexs.size(), false);

			_DFS(srcIndex, visited);
			cout << endl;
		}

		void Print()
		{
			// ��ӡ������±�ӳ���ϵ
			for (size_t i = 0; i < _vertexs.size(); i++)
			{
				cout << _vertexs[i] << "-" << i << endl;
			}
			cout << endl << endl;

			cout << "  ";
			for (size_t i = 0; i < _vertexs.size(); i++)
			{
				cout << i << " ";
			}
			cout << endl;

			// ��ӡ����
			for (size_t i = 0; i < _matrix.size(); i++)
			{
				cout << i << " ";
				for (size_t j = 0; j < _matrix[i].size(); j++)
				{
					if (_matrix[i][j] != MAX_W)
					{
						cout << _matrix[i][j] << " ";
					}
					else
					{
						cout << "# ";
					}
				}
				cout << endl;
			}
			cout << endl;


			// ��ӡ���еı�(ֻ��ӡ����������)
			for (size_t i = 0; i < _matrix.size(); i++)
			{
				for (size_t j = 0; j < _matrix[i].size(); j++)
				{
					if (i < j && _matrix[i][j] != MAX_W)
					{
						cout << i << "-" << j << ":" << _matrix[i][j] << endl;
					}
				}
			}
		}

		// ��³˹�����㷨(��С������)
		W Kruskal(Self& minTree)
		{
			minTree._vertexs = _vertexs;
			minTree._vIndexMap = _vIndexMap;
			minTree._matrix.resize(_vertexs.size());
			for (auto& e : minTree._matrix)
			{
				e.resize(_vertexs.size(), MAX_W);
			}


			// ���ȼ����У�С��(greater)�����(less)
			priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
			for (size_t i = 0; i < _matrix.size(); i++)
			{
				for (size_t j = 0; j < _matrix[i].size(); j++)
				{
					if (i < j && _matrix[i][j] != MAX_W)
					{
						pq.push(Edge(i, j, _matrix[i][j]));
					}
				}
			}

			W total = W();


			//Ĭ������һ����Ϊ��ʼ��
			size_t i = 1;
			UnionFindSet ufs(_vertexs.size());
			while (i < _vertexs.size() && !pq.empty())
			{
				Edge min = pq.top();
				pq.pop();
				
				if (ufs.FindRoot(min._srci) != ufs.FindRoot(min._dsti))
				{
					minTree._AddEdge(min._srci, min._dsti, min._w);

					ufs.Union(min._srci, min._dsti);

					total += min._w;
					i++;
				}
			}

			if (i == _vertexs.size())
			{
				return total;
			}
			else
			{
				return W();
			}
		}


		// ����ķ�㷨(��С������)
		W Prim(Self& minTree, const V& src)
		{
			minTree._vertexs = _vertexs;
			minTree._vIndexMap = _vIndexMap;
			minTree._matrix.resize(_vertexs.size());
			for (auto& e : minTree._matrix)
			{
				e.resize(_vertexs.size(), MAX_W);
			}

			size_t srci = GetVertexIndex(src);
			set<size_t>inSet;
			inSet.insert(srci);
			// ��srcΪ��ʼ��
			priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
			for (size_t i = 0; i < _matrix.size(); i++)
			{
				if (_matrix[srci][i] != MAX_W)
				{
					pq.push(Edge(srci, i, _matrix[srci][i]));
				}
			}

			W total = W();
			while (inSet.size() < _vertexs.size() && !pq.empty())
			{
				Edge min = pq.top();
				pq.pop();

				if (inSet.find(min._srci) == inSet.end() || inSet.find(min._dsti) == inSet.end())
				{
					minTree._AddEdge(min._srci, min._dsti, min._w);
					total += min._w;

					for (size_t i = 0; i < _vertexs.size(); i++)
					{
						if (_matrix[min._dsti][i] != MAX_W)
						{
							pq.push(Edge(min._dsti, i, _matrix[min._dsti][i]));
						}
					}

					inSet.insert(min._dsti);
				}
			}

			if (inSet.size() == _vertexs.size())
			{
				return total;
			}
			else
			{
				return W();
			}

		}

		void PrinrtShotPath(const V& src, vector<W>& dist, vector<int>& parentPath)
		{
			size_t N = _vertexs.size();
			size_t srci = GetVertexIndex(src);

			for (size_t i = 0; i < N; i++)
			{
				if (i == srci)
				{
					continue;
				}

				vector<int> path;
				int parenti = i;
				while (parenti != srci)
				{
					path.push_back(parenti);
					parenti = parentPath[parenti];
				}
				path.push_back(srci);

				reverse(path.begin(), path.end());
				for (auto pos : path)
				{
					// ���һ�����ּ�Ϊ·������
					cout << _vertexs[pos] << "->";
				}
				cout << dist[i] << endl;

			}

		}

		// �Ͻ�˹�����㷨(�޷��������Ȩֵ��ͼ)
		// ��Դ�㵽����������·��
		void Dijkstra(const V& src, vector<W>& dist, vector<int>& parentPath)
		{
			size_t N = _vertexs.size();
			size_t srci = GetVertexIndex(src);

			// vector<W> dist,��¼srci-�����������·��Ȩֵ����
			dist.resize(N, MAX_W);
			// vector<int> parentPath ��¼srci-�����������·������������
			parentPath.resize(N, -1);

			// vector<bool> S��¼S[i]�Ƿ�����
			vector<bool> S;
			S.resize(N, false);

			// ��ʼ�ڵ�src��src��Ȩֵ
			dist[srci] = W();

			for (size_t i = 0; i < N; i++)
			{

				//̰���㷨 srci������S��·����̵��Ǹ�����u
				W min = MAX_W;
				size_t u = srci;
				for (size_t j = 0; j < N; j++)
				{
					if (S[j] == false && dist[j] < min)
					{
						min = dist[j];
						u = j;
					}
				}
				S[u] = true;

				// �ɳڲ��� ����һ��u���ӵ����бߣ����Ƿ��ܸ��³���������·��
				for (size_t k = 0; k < N; k++)
				{
					if (S[k] == false && _matrix[u][k] != MAX_W
						&& dist[u] + _matrix[u][k] < dist[k])
					{
						dist[k] = dist[u] + _matrix[u][k];
						parentPath[k] = u;
					}
				}
			}
		}

		// �����������㷨���ܽ��ȨֵΪ��������ͼ
		// ����ʱ�临�ӶȱȵϽ�˹�����㷨��
		bool BellmanFord(const V& src, vector<W>& dist, vector<int>& parentPath)
		{
			size_t N = _vertexs.size();
			size_t srci = GetVertexIndex(src);

			// vector<W> dist,��¼srci-�����������·��Ȩֵ����
			dist.resize(N, MAX_W);
			// vector<int> parentPath ��¼srci-�����������·������������
			parentPath.resize(N, -1);

			//��ʼ�ڵ㵽�����Ȩֵ
			dist[srci] = W();
			for (size_t k = 0; k < N - 1; k++)
			{
				bool exchange = false;
				for (size_t i = 0; i < N; i++)
				{
					for (size_t j = 0; j < N; j++)
					{
						if (_matrix[i][j] != MAX_W
							&& dist[i] + _matrix[i][j] < dist[j])
						{
							dist[j] = dist[i] + _matrix[i][j];
							parentPath[j] = i;

							exchange = true;
						}

					}
				}

				if (exchange == false)
					break;
				
			}

			// �����ܸ���dist����˵����ͼ�д��ڸ�Ȩ��·
			for (size_t i = 0; i < N; i++)
			{
				for (size_t j = 0; j < N; j++)
				{
					if (_matrix[i][j] != MAX_W
						&& dist[i] + _matrix[i][j] < dist[j])
					{
						return false;
					}
				}
			}
			return true;

		}

		// ���������㷨
		void FloydWarShall(vector<vector<W>>& vvDist, vector<vector<int>>& vvParentPath)
		{
			size_t N = _vertexs.size();
			vvDist.resize(N);
			vvParentPath.resize(N);

			// ��ʼ��Ȩֵ��·������
			for (size_t i = 0; i < N; i++)
			{
				vvDist[i].resize(N, MAX_W);
				vvParentPath[i].resize(N, -1);
			}

			// ��ֱ��������·����ʼ��
			for (size_t i = 0; i < N; i++)
			{
				for (size_t j = 0; j < N; j++)
				{
					if (_matrix[i][j] != MAX_W)
					{
						vvDist[i][j] = _matrix[i][j];
						vvParentPath[i][j] = i;
					}
					else
					{
						vvParentPath[i][j] = -1;
					}

					if (i == j)
					{
						vvDist[i][j] = 0;
						vvParentPath[i][j] = -1;
					}
				}
			}

			// �����ö���k��Ϊ��ת��������·��
			for (size_t k = 0; k < N; k++)
			{
				for (size_t i = 0; i < N; i++)
				{
					for (size_t j = 0; j < N; j++)
					{
						if (vvDist[i][k] != MAX_W && vvDist[k][j] != MAX_W
							&& vvDist[i][k] + vvDist[k][j] < vvDist[i][j])
						{
							vvDist[i][j] = vvDist[i][k] + vvDist[k][j];
							vvParentPath[i][j] = vvParentPath[k][j];
						}
					}
				}
			}


		}

	private:
		map<V, size_t> _vIndexMap;
		vector<V> _vertexs;			// ���㼯��
		vector<vector<W>> _matrix;	// �洢�ߵļ���
	};

	void TestGraph()
	{
		Graph<char, int, INT_MAX, true> g("0123", 4);
		g.AddEdge('0', '1', 1);
		g.AddEdge('0', '3', 4);
		g.AddEdge('1', '3', 2);
		g.AddEdge('1', '2', 9);
		g.AddEdge('2', '3', 8);
		g.AddEdge('2', '1', 5);
		g.AddEdge('2', '0', 3);
		g.AddEdge('3', '2', 6);

		g.Print();
	}

	void TestGraphDBFS()
	{
		string a[] = { "����", "����", "����", "����", "����" };
		Graph<string, int> g1(a, sizeof(a) / sizeof(string));
		g1.AddEdge("����", "����", 100);
		g1.AddEdge("����", "����", 200);
		g1.AddEdge("����", "����", 30);
		g1.AddEdge("����", "����", 30);

		g1.Print();
		g1.BFS("����");
		g1.DFS("����");
	}

	void TestGraphMinTree()
	{
		const char* str = "abcdefghi";
		Graph<char, int> g(str, strlen(str));
		g.AddEdge('a', 'b', 4);
		g.AddEdge('a', 'h', 8);
		//g.AddEdge('a', 'h', 9);
		g.AddEdge('b', 'c', 8);
		g.AddEdge('b', 'h', 11);
		g.AddEdge('c', 'i', 2);
		g.AddEdge('c', 'f', 4);
		g.AddEdge('c', 'd', 7);
		g.AddEdge('d', 'f', 14);
		g.AddEdge('d', 'e', 9);
		g.AddEdge('e', 'f', 10);
		g.AddEdge('f', 'g', 2);
		g.AddEdge('g', 'h', 1);
		g.AddEdge('g', 'i', 6);
		g.AddEdge('h', 'i', 7);

		Graph<char, int> kminTree;
		cout << "Kruskal:" << g.Kruskal(kminTree) << endl;
		kminTree.Print();

		cout << endl;
		Graph<char, int> pminTree;
		cout << "Prim:" << g.Prim(pminTree, 'a') << endl;
		pminTree.Print();
	}

	void TestGraphDijkstra()
	{
		const char* str = "syztx";
		Graph<char, int, INT_MAX, true> g(str, strlen(str));
		g.AddEdge('s', 't', 10);
		g.AddEdge('s', 'y', 5);
		g.AddEdge('y', 't', 3);
		g.AddEdge('y', 'x', 9);
		g.AddEdge('y', 'z', 2);
		g.AddEdge('z', 's', 7);
		g.AddEdge('z', 'x', 6);
		g.AddEdge('t', 'y', 2);
		g.AddEdge('t', 'x', 1);
		g.AddEdge('x', 'z', 4);

		vector<int> dist;
		vector<int> parentPath;
		g.Dijkstra('s', dist, parentPath);
		cout << "���һ�����ּ�Ϊ·������\n";
		g.PrinrtShotPath('s', dist, parentPath);

		// ͼ�д��и�Ȩ·��ʱ��̰�Ĳ�����ʧЧ�ˡ�
		// ���Խ�����Կ���s->t->y֮������·��û���³���
		//const char* str = "sytx";
		//Graph<char, int, INT_MAX, true> g(str, strlen(str));
		//g.AddEdge('s', 't', 10);
		//g.AddEdge('s', 'y', 5);
		//g.AddEdge('t', 'y', -7);
		//g.AddEdge('y', 'x', 3);

		//vector<int> dist;
		//vector<int> parentPath;
		//g.Dijkstra('s', dist, parentPath);
		//cout << "���һ�����ּ�Ϊ·������\n";
		//g.PrinrtShotPath('s', dist, parentPath);
	}

	void TestGraphBellmanFord()
	{
		//const char* str = "syztx";
		//Graph<char, int, INT_MAX, true> g(str, strlen(str));
		//g.AddEdge('s', 't', 6);
		//g.AddEdge('s', 'y', 7);
		//g.AddEdge('y', 'z', 9);
		//g.AddEdge('y', 'x', -3);
		//g.AddEdge('z', 's', 2);
		//g.AddEdge('z', 'x', 7);
		//g.AddEdge('t', 'x', 5);
		//g.AddEdge('t', 'y', 8);
		//g.AddEdge('t', 'z', -4);
		//g.AddEdge('x', 't', -2);

		//vector<int> dist;
		//vector<int> parentPath;
		//if (g.BellmanFord('s', dist, parentPath))
		//{
		//  cout << "���һ�����ּ�Ϊ·������\n";
		//	g.PrinrtShotPath('s', dist, parentPath);
		//}
		//else
		//{
		//	cout << "���ڸ�Ȩ��·" << endl;
		//}

		// ΢��ͼ�ṹ�����и�Ȩ��·�Ĳ���
		//const char* str = "syztx";
		//Graph<char, int, INT_MAX, true> g(str, strlen(str));
		//g.AddEdge('s', 't', 6);
		//g.AddEdge('s', 'y', 7);
		//g.AddEdge('y', 'x', -3);
		//g.AddEdge('y', 'z', 9);
		//g.AddEdge('y', 'x', -3);
		//g.AddEdge('y', 's', 1);  // ����
		//g.AddEdge('z', 's', 2);
		//g.AddEdge('z', 'x', 7);
		//g.AddEdge('t', 'x', 5);
		//g.AddEdge('t', 'y', -8); // ����
		//g.AddEdge('t', 'z', -4);
		//g.AddEdge('x', 't', -2);

		//vector<int> dist;
		//vector<int> parentPath;
		//if (g.BellmanFord('s', dist, parentPath))
		//{
		//  cout << "���һ�����ּ�Ϊ·������\n";
		//	g.PrinrtShotPath('s', dist, parentPath);
		//}
		//else
		//{
		//	cout << "���ڸ�Ȩ��·" << endl;
		//}
	}


	void TestFloydWarShall()
	{
		const char* str = "12345";
		Graph<char, int, INT_MAX, true> g(str, strlen(str));
		g.AddEdge('1', '2', 3);
		g.AddEdge('1', '3', 8);
		g.AddEdge('1', '5', -4);
		g.AddEdge('2', '4', 1);
		g.AddEdge('2', '5', 7);
		g.AddEdge('3', '2', 4);
		g.AddEdge('4', '1', 2);
		g.AddEdge('4', '3', -5);
		g.AddEdge('5', '4', 6);

		vector<vector<int>> vvDist;
		vector<vector<int>> vvParentPath;
		g.FloydWarShall(vvDist, vvParentPath);

		// ��ӡ��������֮������·��
		cout << "���һ�����ּ�Ϊ·������\n";
		for (size_t i = 0; i < strlen(str); ++i)
		{
			g.PrinrtShotPath(str[i], vvDist[i], vvParentPath[i]);
			cout << endl;
		}
	}

}


// �ڽӱ�
namespace LinkTable
{
	template<class W>
	struct Edge
	{
		int _dsti;		//Ŀ�����±�
		W _w;			//Ȩֵ
		Edge<W>* _next;
		
		Edge(int dsti,W w)
			:_dsti(dsti)
			,_w(w)
			,_next(nullptr)
		{}
	};

	// Direction������ͬ��
	template<class V,class W,bool Direction = false>
	class Graph
	{
		typedef Edge<W> Edge;
	public:
		Graph(const V* v, size_t n)
		{
			_vertexs.reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				_vertexs.push_back(v[i]);
				_vIndexMap[v[i]] = i;
			}

			_tables.resize(n, nullptr);
		}

		size_t GetVertexIndex(const V& v)
		{
			auto it = _vIndexMap.find(v);
			if (it != _vIndexMap.end())
			{
				return it->second;
			}
			else
			{
				throw invalid_argument("���㲻����");

				return -1;
			}
		}

		void AddEdge(const V& src, const V& dst, const W& w)
		{
			size_t srci = GetVertexIndex(src);
			size_t dsti = GetVertexIndex(dst);

			//1->2
			Edge* eg = new Edge(dsti, w);
			eg->_next = _tables[srci];
			_tables[srci] = eg;

			//2->1
			if (Direction == false)
			{
				Edge* eg = new Edge(srci, w);
				eg->_next = _tables[dsti];
				_tables[dsti] = eg;
			}
		}

		void Print()
		{
			// ��ӡ���ݺ������±�֮��Ķ�Ӧ��ϵ
			for (size_t i = 0; i < _vertexs.size(); i++)
			{
				cout << "[" << i << "]-" << _vertexs[i] << endl;
			}
			cout << endl;

			// ��ӡ�ڽӱ�
			for (size_t i = 0; i < _tables.size(); i++)
			{
				cout << _vertexs[i] << ":[" << i << "]->";
				Edge* cur = _tables[i];
				while (cur)
				{
					cout << "[" << _vertexs[cur->_dsti] << ":" << cur->_dsti << ":" << cur->_w << "]->";
					cur = cur->_next;
				}
				cout << "nullptr" << endl;

			}

		}


	private:
		vector<V> _vertexs;			//����ļ���
		map<V, size_t> _vIndexMap;	//����±��ӳ��
		vector<Edge*> _tables;		//�ڽӱ�
	};

	void TestGraph1()
	{
		//Graph<char, int, true> g("0123", 4);
		//g.AddEdge('0', '1', 1);
		//g.AddEdge('0', '3', 4);
		//g.AddEdge('1', '3', 2);
		//g.AddEdge('1', '2', 9);
		//g.AddEdge('2', '3', 8);
		//g.AddEdge('2', '1', 5);
		//g.AddEdge('2', '0', 3);
		//g.AddEdge('3', '2', 6);

		//g.Print();

		//string a[] = { "����", "����", "����", "����" };
		//Graph<string, int, true> g1(a, 4);
		//g1.AddEdge("����", "����", 100);
		//g1.AddEdge("����", "����", 200);
		//g1.AddEdge("����", "����", 30);
		//g1.Print();
	}
}