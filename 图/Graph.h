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

// 临接矩阵(V:顶点的类型,W:边)
// Direction=true代表该图为有向图
// Direction=false代表该图为无向图
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

			// MAX_W 作为不存在边的标识值
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
				throw invalid_argument("不存在的顶点");
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

		// 广度优先搜索(用辅助队列)
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
				printf("%s的%d度好友", src.c_str(), d);

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

		// 深度优先搜索
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
			// 打印顶点和下标映射关系
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

			// 打印矩阵
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


			// 打印所有的边(只打印右上三角形)
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

		// 库鲁斯卡尔算法(最小生成树)
		W Kruskal(Self& minTree)
		{
			minTree._vertexs = _vertexs;
			minTree._vIndexMap = _vIndexMap;
			minTree._matrix.resize(_vertexs.size());
			for (auto& e : minTree._matrix)
			{
				e.resize(_vertexs.size(), MAX_W);
			}


			// 优先级队列，小堆(greater)，大堆(less)
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


			//默认任意一个点为起始点
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


		// 普里姆算法(最小生成树)
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
			// 以src为初始点
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
					// 最后一个数字即为路径长度
					cout << _vertexs[pos] << "->";
				}
				cout << dist[i] << endl;

			}

		}

		// 迪杰斯特拉算法(无法处理带负权值的图)
		// 单源点到其他点的最短路径
		void Dijkstra(const V& src, vector<W>& dist, vector<int>& parentPath)
		{
			size_t N = _vertexs.size();
			size_t srci = GetVertexIndex(src);

			// vector<W> dist,记录srci-其他顶点最短路径权值数组
			dist.resize(N, MAX_W);
			// vector<int> parentPath 记录srci-其他顶点最短路径父顶点数组
			parentPath.resize(N, -1);

			// vector<bool> S记录S[i]是否连接
			vector<bool> S;
			S.resize(N, false);

			// 起始节点src到src的权值
			dist[srci] = W();

			for (size_t i = 0; i < N; i++)
			{

				//贪心算法 srci到不在S中路径最短的那个顶点u
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

				// 松弛操作 更新一遍u连接的所有边，看是否能更新出更短连接路径
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

		// 贝尔曼福特算法，能解决权值为负的有向图
		// 但是时间复杂度比迪杰斯特拉算法大
		bool BellmanFord(const V& src, vector<W>& dist, vector<int>& parentPath)
		{
			size_t N = _vertexs.size();
			size_t srci = GetVertexIndex(src);

			// vector<W> dist,记录srci-其他顶点最短路径权值数组
			dist.resize(N, MAX_W);
			// vector<int> parentPath 记录srci-其他顶点最短路径父顶点数组
			parentPath.resize(N, -1);

			//起始节点到自身的权值
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

			// 若还能更新dist，则说明该图中存在负权回路
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

		// 弗洛伊德算法
		void FloydWarShall(vector<vector<W>>& vvDist, vector<vector<int>>& vvParentPath)
		{
			size_t N = _vertexs.size();
			vvDist.resize(N);
			vvParentPath.resize(N);

			// 初始化权值和路径矩阵
			for (size_t i = 0; i < N; i++)
			{
				vvDist[i].resize(N, MAX_W);
				vvParentPath[i].resize(N, -1);
			}

			// 将直接相连的路径初始化
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

			// 依次用顶点k作为中转点更新最短路径
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
		vector<V> _vertexs;			// 顶点集合
		vector<vector<W>> _matrix;	// 存储边的集合
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
		string a[] = { "张三", "李四", "王五", "赵六", "周七" };
		Graph<string, int> g1(a, sizeof(a) / sizeof(string));
		g1.AddEdge("张三", "李四", 100);
		g1.AddEdge("张三", "王五", 200);
		g1.AddEdge("王五", "赵六", 30);
		g1.AddEdge("王五", "周七", 30);

		g1.Print();
		g1.BFS("张三");
		g1.DFS("张三");
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
		cout << "最后一个数字即为路径长度\n";
		g.PrinrtShotPath('s', dist, parentPath);

		// 图中带有负权路径时，贪心策略则失效了。
		// 测试结果可以看到s->t->y之间的最短路径没更新出来
		//const char* str = "sytx";
		//Graph<char, int, INT_MAX, true> g(str, strlen(str));
		//g.AddEdge('s', 't', 10);
		//g.AddEdge('s', 'y', 5);
		//g.AddEdge('t', 'y', -7);
		//g.AddEdge('y', 'x', 3);

		//vector<int> dist;
		//vector<int> parentPath;
		//g.Dijkstra('s', dist, parentPath);
		//cout << "最后一个数字即为路径长度\n";
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
		//  cout << "最后一个数字即为路径长度\n";
		//	g.PrinrtShotPath('s', dist, parentPath);
		//}
		//else
		//{
		//	cout << "存在负权回路" << endl;
		//}

		// 微调图结构，带有负权回路的测试
		//const char* str = "syztx";
		//Graph<char, int, INT_MAX, true> g(str, strlen(str));
		//g.AddEdge('s', 't', 6);
		//g.AddEdge('s', 'y', 7);
		//g.AddEdge('y', 'x', -3);
		//g.AddEdge('y', 'z', 9);
		//g.AddEdge('y', 'x', -3);
		//g.AddEdge('y', 's', 1);  // 新增
		//g.AddEdge('z', 's', 2);
		//g.AddEdge('z', 'x', 7);
		//g.AddEdge('t', 'x', 5);
		//g.AddEdge('t', 'y', -8); // 更改
		//g.AddEdge('t', 'z', -4);
		//g.AddEdge('x', 't', -2);

		//vector<int> dist;
		//vector<int> parentPath;
		//if (g.BellmanFord('s', dist, parentPath))
		//{
		//  cout << "最后一个数字即为路径长度\n";
		//	g.PrinrtShotPath('s', dist, parentPath);
		//}
		//else
		//{
		//	cout << "存在负权回路" << endl;
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

		// 打印任意两点之间的最短路径
		cout << "最后一个数字即为路径长度\n";
		for (size_t i = 0; i < strlen(str); ++i)
		{
			g.PrinrtShotPath(str[i], vvDist[i], vvParentPath[i]);
			cout << endl;
		}
	}

}


// 邻接表
namespace LinkTable
{
	template<class W>
	struct Edge
	{
		int _dsti;		//目标点的下标
		W _w;			//权值
		Edge<W>* _next;
		
		Edge(int dsti,W w)
			:_dsti(dsti)
			,_w(w)
			,_next(nullptr)
		{}
	};

	// Direction的意义同上
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
				throw invalid_argument("顶点不存在");

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
			// 打印数据和数据下标之间的对应关系
			for (size_t i = 0; i < _vertexs.size(); i++)
			{
				cout << "[" << i << "]-" << _vertexs[i] << endl;
			}
			cout << endl;

			// 打印邻接表
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
		vector<V> _vertexs;			//顶点的集合
		map<V, size_t> _vIndexMap;	//点和下标的映射
		vector<Edge*> _tables;		//邻接表
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

		//string a[] = { "张三", "李四", "王五", "赵六" };
		//Graph<string, int, true> g1(a, 4);
		//g1.AddEdge("张三", "李四", 100);
		//g1.AddEdge("张三", "王五", 200);
		//g1.AddEdge("王五", "赵六", 30);
		//g1.Print();
	}
}