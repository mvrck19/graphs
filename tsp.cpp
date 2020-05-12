#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <cstring>
#include <climits>
#include <chrono>
using namespace std;

// N is number of total nodes on the graph or the cities in the map
#define N 10

// Sentinal value for representing infinity
#define INF INT_MAX

class Node
{
public:
    vector<pair<int, int>> path;
    int reducedMatrix[N][N];
    int cost;
    int vertex;
    int level;
    Node(int parentMatrix[N][N], vector<pair<int, int>> const &path,int level, int i, int j)
    {
        this->path = path;
        if (level != 0)
            this->path.push_back(make_pair(i, j));
        memcpy(this->reducedMatrix, parentMatrix,
               sizeof this->reducedMatrix);
        for (int k = 0; level != 0 && k < N; k++)
        {
            this->reducedMatrix[i][k] = INF;
            this->reducedMatrix[k][j] = INF;
        }
        this->reducedMatrix[j][0] = INF;
        this->level = level;
        this->vertex = j;
        
    }
};


// State Space Tree nodes

// Function to reduce each row in such a way that
// there must be at least one zero in each row
void rowReduction(int reducedMatrix[N][N], int row[N])
{
	// initialize row array to INF
	fill_n(row, N, INF);

	// row[i] contains minimum in row i
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] < row[i])
				row[i] = reducedMatrix[i][j];

	// reduce the minimum value from each element in each row
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] != INF && row[i] != INF)
				reducedMatrix[i][j] -= row[i];
}

// Function to reduce each column in such a way that
// there must be at least one zero in each column
void columnReduction(int reducedMatrix[N][N], int col[N])
{
	// initialize col array to INF
	fill_n(col, N, INF);

	// col[j] contains minimum in col j
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] < col[j])
				col[j] = reducedMatrix[i][j];

	// reduce the minimum value from each element in each column
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] != INF && col[j] != INF)
				reducedMatrix[i][j] -= col[j];
}

// Function to get the lower bound on
// on the path starting at current min node
int calculateCost(int reducedMatrix[N][N])
{
	// initialize cost to 0
	int cost = 0;

	// Row Reduction
	int row[N];
	rowReduction(reducedMatrix, row);

	// Column Reduction
	int col[N];
	columnReduction(reducedMatrix, col);

	// the total expected cost
	// is the sum of all reductions
	for (int i = 0; i < N; i++)
		cost += (row[i] != INT_MAX) ? row[i] : 0,
			cost += (col[i] != INT_MAX) ? col[i] : 0;

	return cost;
}

// print list of cities visited following least cost
void printPath(vector<pair<int, int>> const &list)
{
	for (int i = 0; i < list.size(); i++)
		cout << list[i].first + 1 << " -> "
			 << list[i].second + 1 << endl;
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(const Node lhs, const Node rhs) const
	{
		return lhs.cost > rhs.cost;
	}
};

// Function to solve Traveling Salesman Problem using Branch and Bound
int solve(int costMatrix[N][N])
{
	// Create a priority queue to store live nodes of search tree;
	priority_queue<Node , std::vector<Node >, comp> pq;

	vector<pair<int, int>> v;

	// create a root node and calculate its cost
	// The TSP starts from first city i.e. node 0
	Node root = Node(costMatrix, v, 0, -1, 0);

	// get the lower bound of the path starting at node 0
	root.cost = calculateCost(root.reducedMatrix);

	// Add root to list of live nodes;
	pq.push(root);

	// Finds a live node with least cost, add its children to list of
	// live nodes and finally deletes it from the list
	while (!pq.empty())
	{
		// Find a live node with least estimated cost
		Node min = pq.top();

		// The found node is deleted from the list of live nodes
		pq.pop();

		// i stores current city number
		int i = min.vertex;

		// if all cities are visited
		if (min.level == N - 1)
		{
			// return to starting city
			min.path.push_back(make_pair(i, 0));

			// print list of cities visited;
			printPath(min.path);

			// return optimal cost
			return min.cost;
		}

		// do for each child of min
		// (i, j) forms an edge in space tree
		for (int j = 0; j < N; j++)
		{
			if (min.reducedMatrix[i][j] != INF)
			{
				// create a child node and calculate its cost
				Node child = Node(min.reducedMatrix, min.path,
									  min.level + 1, i, j);

				/* Cost of the child =
					cost of parent node +
					cost of the edge(i, j) +
					lower bound of the path starting at node j
				*/
				child.cost = min.cost + min.reducedMatrix[i][j] + calculateCost(child.reducedMatrix);

				// Add child to list of live nodes
				pq.push(child);
			}
		}
	}
}

// main function
int main()
{
	/*
	int costMatrix[N][N] =
	{
		{INF, 5,	INF, 6,	5,	4},
		{5,	INF, 2,	4,	3,	INF},
		{INF, 2,	INF, 1,	INF, INF},
		{6,	4,	1,	INF, 7,	INF},
		{5,	3,	INF, 7,	INF, 3},
		{4,	INF, INF, INF, 3,	INF}
	};
	*/
	// cost 34
	// Example matrix
	int costMatrix[N][N] =
		{
			{INF, 20, 30, 10, 11},
			{15, INF, 16, 4, 2},
			{3, 5, INF, 2, 4},
			{19, 6, 18, INF, 3},
			{16, 4, 7, 16, INF}};

	/*
	// cost 16
	int costMatrix[N][N] =
	{
		{INF, 3,	1,	5,	8},
		{3,	INF, 6,	7,	9},
		{1,	6,	INF, 4,	2},
		{5,	7,	4,	INF, 3},
		{8,	9,	2,	3,	INF}
	};
	*/

	/*
	// cost 8
	int costMatrix[N][N] =
	{
		{INF, 2,	1,	INF},
		{2,	INF, 4,	3},
		{1,	4,	INF, 2},
		{INF, 3,	2,	INF}
	};
	*/

	/*
	// cost 12
	int costMatrix[N][N] =
	{
		{INF, 5,	4,	3},
		{3,	INF, 8,	2},
		{5,	3,	INF, 9},
		{6,	4,	3,	INF}
	};
	*/
	int random[N][N];
	for (int i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			if (i == j)
			{
				random[i][j] = INF;
			}
			else
			{
				random[i][j] = rand() % 100;
			}
		}
	}

	auto start = std::chrono::steady_clock::now();
	cout << "\n\nTotal Cost is " << solve(random);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	cout << "\n\nTotal Time is " << elapsed_seconds.count() << "s\n";

	return 0;
}