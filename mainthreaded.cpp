#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <cstring>
#include <climits>
#include <thread>
#include <unistd.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
// boost::mutex mtx;
using namespace std;
#define N 10
#define INF INT_MAX
struct Node
{
	vector<pair<int, int>> path;
	int reducedMatrix[N][N];
	int cost;
	int vertex;
	int level;
};
Node *newNode(int parentMatrix[N][N], vector<pair<int, int>> const &path,
			  int level, int i, int j)
{
	Node *node = new Node;
	node->path = path;
	if (level != 0)
		node->path.push_back(make_pair(i, j));
	memcpy(node->reducedMatrix, parentMatrix,
		   sizeof node->reducedMatrix);
	for (int k = 0; level != 0 && k < N; k++)
	{
		node->reducedMatrix[i][k] = INF;
		node->reducedMatrix[k][j] = INF;
	}
	node->reducedMatrix[j][0] = INF;
	node->level = level;
	node->vertex = j;
	return node;
}
int rowReduction(int reducedMatrix[N][N], int row[N])
{
	fill_n(row, N, INF);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] < row[i])
				row[i] = reducedMatrix[i][j];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] != INF && row[i] != INF)
				reducedMatrix[i][j] -= row[i];
}
int columnReduction(int reducedMatrix[N][N], int col[N])
{
	fill_n(col, N, INF);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] < col[j])
				col[j] = reducedMatrix[i][j];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] != INF && col[j] != INF)
				reducedMatrix[i][j] -= col[j];
}
int calculateCost(int reducedMatrix[N][N])
{
	int cost = 0;
	int row[N];
	rowReduction(reducedMatrix, row);
	int col[N];
	columnReduction(reducedMatrix, col);
	for (int i = 0; i < N; i++)
		cost += (row[i] != INT_MAX) ? row[i] : 0,
			cost += (col[i] != INT_MAX) ? col[i] : 0;
	return cost;
}
void printPath(vector<pair<int, int>> const &list)
{
	for (int i = 0; i < list.size(); i++)
		cout << list[i].first + 1 << " -> "
			 << list[i].second + 1 << endl;
}
struct comp
{
	bool operator()(const Node *lhs, const Node *rhs) const
	{
		return lhs->cost > rhs->cost;
	}
};

void foo(Node *min, priority_queue<Node *, std::vector<Node *>, comp> &pq, int i, int j)
{
	try
	{
		Node *child = newNode(min->reducedMatrix, min->path, min->level + 1, i, j);
		child->cost = min->cost + min->reducedMatrix[i][j] + calculateCost(child->reducedMatrix);
		// mutex lock used to be here
		pq.push(child);
		// mutex unlock used to be here
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}


int solve(int costMatrix[N][N])
{
	boost::asio::thread_pool pool(1);
	priority_queue<Node *, std::vector<Node *>, comp> pq;
	vector<pair<int, int>> v;
	Node *root = newNode(costMatrix, v, 0, -1, 0);
	root->cost = calculateCost(root->reducedMatrix);
	pq.push(root);
	while (!pq.empty())
	{
		Node *min = pq.top();
		pq.pop();
		int i = min->vertex;
		if (min->level == N - 1)
		{
			min->path.push_back(make_pair(i, 0));
			printPath(min->path);
			return min->cost;
		}
		for (int j = 0; j < N; j++)
		{
			if (min->reducedMatrix[i][j] != INF)
			{		
				boost::asio::post(pool, boost::bind(foo, min, std::ref(pq), i, j));
			}
		}
		delete min;
		pool.join();
	}
}

int main()
{
	// int costMatrix[N][N] =
	// 	{
	// 		{INF, 20, 30, 10, 11},
	// 		{15, INF, 16, 4, 2},
	// 		{3, 5, INF, 2, 4},
	// 		{19, 6, 18, INF, 3},
	// 		{16, 4, 7, 16, INF}};
	int bigmofo[N][N];
	for (int i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			if (i == j)
			{
				bigmofo[i][j] = INF;
			}
			else
			{
				bigmofo[i][j] = rand() % 100;
			}
		}
	}
	time_t pre, after;
	time(&pre);
	cout << "\n\nTotal Cost is " << solve(bigmofo);
	time(&after);
	cout << "\n\nTotal Time is " << after - pre;
	return 0;
}