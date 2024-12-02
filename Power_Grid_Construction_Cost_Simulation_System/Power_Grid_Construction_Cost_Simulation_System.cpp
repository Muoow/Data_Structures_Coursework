#include<iostream>
#include<limits>
#include<conio.h>

const int MAX_VERTICES = 26;
const int MAX_EDGES = MAX_VERTICES * (MAX_VERTICES - 1);

struct Edge 
{
	bool exist;
	int weight;
};

template <typename Type>
class MyUndirectedGraph 
{
private:
	int maxVertices;
	int vertexCount;
	int edgeCount;
	Type* vertices;
	Edge** graph;
	int findVertexIndex(const Type& vertex) const;
	/* MST storage */ 
	int* mstParent;
	bool mstComputed;
public:
	MyUndirectedGraph(int _maxVertices);
	~MyUndirectedGraph();
	int getVertexCount() { return vertexCount; }
	int getEdgeCount() { return edgeCount; }
	bool addVertex(const Type& vertex);
	bool addEdge(const Type& vertexA, const Type& vertexB, int weight);
	bool findEdge(const Type& vertexA, const Type& vertexB);
	bool isComputed() { return mstComputed; }
	void primMST(const Type& vertex);
	void printMST();
};

template <typename Type>
int MyUndirectedGraph<Type>::findVertexIndex(const Type& vertex) const 
{
	for (int i = 0; i < vertexCount; i++) {
		if (vertices[i] == vertex) {
			return i;
		}
	}
	return -1;
}

template <typename Type>
MyUndirectedGraph<Type>::MyUndirectedGraph(int _maxVertices)
	:maxVertices(_maxVertices), vertexCount(0),edgeCount(0), mstComputed(false)
{
	vertices = new (std::nothrow) Type[maxVertices];
	if (vertices == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	graph = new (std::nothrow) Edge * [maxVertices];
	if (graph == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < maxVertices; i++) {
		graph[i] = new(std::nothrow) Edge[maxVertices];
		if (graph[i] == nullptr) {
			std::cerr << "Error: Memory allocation failed." << std::endl;
			exit(-1);
		}
		for (int j = 0; j < maxVertices; j++) {
			graph[i][j].exist = false;
			graph[i][j].weight = INT_MAX;
		}
	}
	mstParent = new(std::nothrow) int[maxVertices];
	if (mstParent == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < maxVertices; i++) {
		mstParent[i] = -1;
	}
}

template <typename Type>
MyUndirectedGraph<Type>::~MyUndirectedGraph() 
{
	delete[] vertices;
	for (int i = 0; i < maxVertices; ++i) {
		delete[] graph[i];
	}
	delete[] graph;
	delete[] mstParent;
}

template <typename Type>
bool MyUndirectedGraph<Type>::addVertex(const Type& vertex) 
{
	if (vertexCount >= maxVertices) {
		return false;
	}
	if (findVertexIndex(vertex) != -1) {
		return false;
	}
	vertices[vertexCount] = vertex;
	vertexCount++;
	return true;
}

template <typename Type>
bool MyUndirectedGraph<Type>::addEdge(const Type& vertexA, const Type& vertexB, int weight)
{
	int indexA = findVertexIndex(vertexA);
	int indexB = findVertexIndex(vertexB);

	if (indexA == -1 || indexB == -1 || indexA == indexB) {
		return false;
	}

	if (!graph[indexA][indexB].exist|| !graph[indexB][indexA].exist) {
		graph[indexA][indexB].exist = true;
		graph[indexB][indexA].exist = true;
		edgeCount++;
	}
	
	graph[indexA][indexB].weight = weight;

	graph[indexB][indexA].weight = weight;

	return true;
}

template <typename Type>
bool MyUndirectedGraph<Type>::findEdge(const Type& vertexA, const Type& vertexB) 
{
	int indexA = findVertexIndex(vertexA);
	int indexB = findVertexIndex(vertexB);

	if (indexA == -1 || indexB == -1 || indexA == indexB) {
		return false;
	}

	if (!graph[indexA][indexB].exist || !graph[indexB][indexA].exist) {
		return false;
	}

	return true;
}

template <typename Type>
void MyUndirectedGraph<Type>::primMST(const Type& startVertex)
{
	/* whether the vertex has been added to the MST */
	bool* visited = new(std::nothrow) bool[vertexCount];
	if (visited == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < vertexCount; i++)
		visited[i] = false;
	/* The minimum weight edge from each vertex to the MST */
	int* minWeight = new (std::nothrow)int[vertexCount];
	if (minWeight == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}

	/* Initialize all vertices */
	for (int i = 0; i < vertexCount; i++) {
		minWeight[i] = INT_MAX; 
	}

	/* Find the starting node */
	int startIndex = findVertexIndex(startVertex);
	if (startIndex == -1) {
		std::cerr << "Error: Starting vertex not found in the graph." << std::endl;
		/* Release the dynamic array and exit */ 
		delete[] visited;
		delete[] minWeight;
		return;
	}
	minWeight[startIndex] = 0;

	for (int i = 0; i < vertexCount; i++) {
		int u = -1;
		/* Find the vertex with the smallest weight that is not currently visited */
		for (int j = 0; j < vertexCount; j++) {
			if (!visited[j] && (u == -1 || minWeight[j] < minWeight[u])) {
				u = j;
			}
		}

		if (u == -1) {
			std::cerr << "Graph is disconnected; no MST exists." << std::endl;
			delete[] visited;
			delete[] minWeight;
			return;
		}

		visited[u] = true;

		for (int v = 0; v < vertexCount; v++) {
			if (graph[u][v].exist && !visited[v] && graph[u][v].weight < minWeight[v]) {
				minWeight[v] = graph[u][v].weight;
				mstParent[v] = u;
			}
		}
	}

	mstComputed = true;
	delete[] visited;
	delete[] minWeight;
}

template <typename Type>
void MyUndirectedGraph<Type>::printMST()
{
	if (!mstComputed) {
		std::cerr << "Error: MST has not been computed. Call primMST first." << std::endl;
		return;
	}

	for (int i = 0; i < vertexCount; i++) {
		if (mstParent[i] != -1) {
			std::cout << "Edge: " << vertices[mstParent[i]] << " --> " << vertices[i];
			std::cout << "  Weight: " << graph[mstParent[i]][i].weight << std::endl;
		}
	}
}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt) 
{
	std::cout << ">>> " << "请输入" << prompt << " 整数范围: [" << lowerLimit << "~" << upperLimit << "]: ";
	int input;
	while (true) {
		std::cin >> input;
		if (std::cin.good() && input >= lowerLimit && input <= upperLimit) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cout << std::endl;
			return input;
		}
		else {
			std::cerr << ">>> " << prompt << "输入不合法，请重新输入！" << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

class PowerGrid 
{
private:
	MyUndirectedGraph<char> grid;
public:
	PowerGrid(int _num) : grid(_num) {}
	~PowerGrid() {}
	bool selectOption();
	void createGridVertices();
	void createGridEdges();
	void constructMinimumSpanningTree();
	void printMinimumSpanningTree();
};

bool PowerGrid::selectOption() 
{
	std::cout << ">>> 请选择要使用的功能: ";
	char option;
	while (true) {
		option = _getch();
		if (option == 0 || option == -32) {
			option = _getch();
		}
		else if (option >= '1' || option <= '5') {
			std::cout << "[" << option << "]" << std::endl << std::endl;
			break;
		}
	}
	switch (option) {
		case '1':
			createGridVertices();
			break;
		case '2':
			createGridEdges();
			break;
		case '3':
			constructMinimumSpanningTree();
			break;
		case '4':
			printMinimumSpanningTree();
			break;
		case '5':
			return false;
	}
	return true;
}

void PowerGrid::createGridVertices() 
{
	int numVertices = inputInteger(2, MAX_VERTICES, "要添加的节点数量");

	for (int i = 0; i < numVertices; i++) {
		char vertex;
		std::cout << "请输入节点" << (i + 1) << ": ";
		std::cin >> vertex;
		if (!grid.addVertex(vertex)) {
			std::cout << "节点 " << vertex << " 已存在或无法添加！" << std::endl;
			i--;
		}
		else {
			std::cout << "节点 " << vertex << " 添加成功！" << std::endl;
		}
	}
	std::cout << std::endl;
}

void PowerGrid::createGridEdges() 
{
	int numVertices = grid.getVertexCount();
	int numEdges = inputInteger(1, numVertices * (numVertices - 1) / 2, "要添加的边数量");

	for (int i = 0; i < numEdges; i++) {
		char vertexA, vertexB;
		int weight;
		std::cout << "请输入边的起点、终点和权重 (格式: A B 5): ";
		std::cin >> vertexA >> vertexB >> weight;

		if (!grid.addEdge(vertexA, vertexB, weight)) {
			std::cout << "边 " << vertexA << " - " << vertexB << " 添加失败！" << std::endl;
			i--;
		}
		else {
			std::cout << "边 " << vertexA << " - " << vertexB << " 权重: " << weight << " 添加成功！" << std::endl;
		}
	}
	std::cout << std::endl;
}

void PowerGrid::constructMinimumSpanningTree() 
{
	char startVertex;
	std::cout << "请输入构造最小生成树的起始节点: ";
	std::cin >> startVertex;

	grid.primMST(startVertex);

	if (grid.isComputed())
		std::cout << "最小生成树构造完成！" << std::endl << std::endl;
	else
		std::cout << "最小生成树生成失败！" << std::endl << std::endl;
}

void PowerGrid::printMinimumSpanningTree() 
{	
	grid.printMST();
	std::cout << std::endl;
}

int main() 
{
	std::cout << "+--------------------------------------+" << std::endl;
	std::cout << "|           电网造价模拟系统           |" << std::endl;
	std::cout << "|  Power Grid Cost Simulation System   |" << std::endl;
	std::cout << "+--------------------------------------+" << std::endl;
	std::cout << "|        [1] --- 创建电网节点          |" << std::endl;
	std::cout << "|        [2] --- 添加电网的边          |" << std::endl;
	std::cout << "|        [3] --- 构造最小生成树        |" << std::endl;
	std::cout << "|        [4] --- 显示最小生成树        |" << std::endl;
	std::cout << "|        [5] --- 退出系统              |" << std::endl;
	std::cout << "+--------------------------------------+" << std::endl << std::endl;
	PowerGrid powergrid(MAX_VERTICES);

	while (powergrid.selectOption());

	std::cout << ">>> 已成功退出电网造价模拟系统" << std::endl;
	return 0;
}