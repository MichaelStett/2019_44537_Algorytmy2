// ALGO2 IS1 212A LAB09
// Michal Tymejczyk
// tm44537@zut.edu.pl

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <chrono>

using namespace std;

#define UFRanked
#define PathCompression

struct Point
{
	double x;
	double y;
};

string to_string(Point p)
{
	return to_string(p.x) + " " + to_string(p.y);
}

struct Edge
{
	int from;
	int to;
	double cost;

	friend bool operator<(const Edge &e1, const Edge &e2)
	{
		return (e1.cost < e2.cost);
	}

	friend bool operator>(const Edge &e1, const Edge &e2)
	{
		return (e1.cost > e2.cost);
	}
};

void swap(Edge &e1, Edge &e2)
{
	swap(e1.from, e2.from);
	swap(e1.to, e2.to);
	swap(e1.cost, e2.cost);
}

string to_string(Edge e)
{
	return to_string(e.cost) + ":" + to_string(e.from) + "->" + to_string(e.to);
}

struct Graph
{
	vector<Edge> edges;

	int edgesCount = 0;
	int pointsCount = 0;

	Graph(){};
	Graph(int pCount, int eCount)
	{
		pointsCount = pCount;
		edgesCount = eCount;
		edges.reserve(edgesCount);
	}

	void Push(Edge e)
	{
		edges.push_back(e);
	}

	vector<Edge>::iterator Start()
	{
		return edges.begin();
	}

	vector<Edge>::iterator End()
	{
		return edges.end();
	}

	Edge &operator[](const int i)
	{
		return edges[i];
	}
};

struct Union_find
{
	vector<int> parent;
	vector<int> rank;

	int length = 0;
	int findCounter = 0;

	Union_find(int length)
	{
		this->length = length;

		parent.reserve(length);
		rank.reserve(length);

		for(int i = 0; i < length; i++)
		{
			parent[i] = i;
			rank[i] = 0;
		}
	}

	void Union(int x, int y)
	{
		if (rank[x] < rank[y])
		{
			parent[x] = parent[y];
			rank[y] += rank[x];
		}
		else
		{
			parent[y] = parent[x];
			rank[x] += rank[y];
		}
	}

	void UnionRanked(int x, int y)
	{
		x = Find(x);
		y = Find(y);

		if (rank[x] < rank[y])
			parent[x] = y;
		else
			parent[y] = x;

		if (rank[x] == rank[y])
			rank[y]++;
	}

	int Find(int key)
	{
		if (key == parent[key])
			return key;

		parent[key] = Find(parent[key]);

		findCounter++;
		return parent[key];
	}

	int FindCompressed(int i)
	{
		while (parent[i] != i)
		{
			parent[i] = parent[parent[i]];
			i = parent[i];
		}

		findCounter++;
		return i;
	}
};

void print(Graph graph)
{
	int counter = 0;
	for_each(graph.Start(), graph.End(), [&counter](const auto &edge) {
		cout << "Edge (" << ++counter << ") : " << to_string(edge) << endl;
	});
}

void sort(Graph &graph)
{
	int i, j;
	int count = graph.edgesCount;
	Edge temp;

	for (i = 1; i < count; i++)
	{
		temp = graph[i];
		j = i - 1;

		while (j >= 0 && graph[j] > temp)
		{
			swap(graph[j + 1], graph[j]);
			j--;
		}

		swap(graph[j + 1], temp);
	}
}

void Kruskal(Graph &graph)
{
	int E = graph.edgesCount;
	int V = graph.pointsCount;

	vector<Edge> result;
	result.reserve(V);

	int v = 0;
	int e = 0;

	auto s_start = chrono::system_clock::now();
	sort(graph);
	auto s_end = chrono::system_clock::now();

	Union_find uf = Union_find(V);

	auto k_start = chrono::system_clock::now();

	while (v < V - 1 && e < E)
	{
		Edge next = graph.edges[e];

		#ifdef PathCompression
			int x = uf.FindCompressed(next.from);
			int y = uf.FindCompressed(next.to);
		#else
			int x = uf.Find(next.from);
			int y = uf.Find(next.to);
		#endif

		if (x != y)
		{
			result.push_back(next);
			#ifdef UFRanked
				uf.UnionRanked(x, y);
			#else
				uf.Union(x, y);
			#endif
			v++;
		}
		e++;
	}
	auto k_end = chrono::system_clock::now();

	printf("\nKruskal results\n");

	double sum = 0;
	int counter = 0;
	for_each(result.begin(), result.end(), [&counter, &sum](const auto &edge) {
		cout << "Edge (" << ++counter << ") : " <<  to_string(edge) << endl;
		sum += edge.cost;
	});

	printf("Number of edges: %6i \n", counter);
	printf("Sum of rank: %10f \n", sum);
	printf("Sorting time: %9f ms\n", ((chrono::duration<double, milli>)(s_end - s_start)).count());
	printf("Kruskal time: %9f ms\n", ((chrono::duration<double, milli>)(k_end - k_start)).count());
	printf("Find Counter: %9i \n", uf.findCounter);
}

int main()
{
	int N = 0; // number of points
	int M = 0; // number of edges
	double X = 0.0;
	double Y = 0.0;

	int from = 0;
	int to = 0;
	double cost = 0.0;

	vector<pair<const string, Graph>> sgpvs = {
		// {"g1.txt", Graph()},
		{"g2.txt", Graph()},
		// {"g3.txt", Graph()},
	};

	for (auto &[dir, graph] : sgpvs)
	{
		cout << "File: " << dir << endl;
		fstream file{dir};
		if (file.is_open())
		{
			file >> N;

			for (int i = 0; i < N; i++)
				file >> X >> Y;

			file >> M;

			printf("Number of edges: %6i \n", M);

			graph = Graph(N, M);

			for (int i = 0; i < M; i++)
			{
				file >> from >> to >> cost;

				graph.Push({from, to, cost});
			}

			// print(graph);

			Kruskal(graph);

			file.close();

			cout << endl;
		}
	}

	return 0;
}

// g++ main.cpp -o main.exe -std=c++17
// ./main.exe
