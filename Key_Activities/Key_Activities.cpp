/****************************************************************
 * Project Name:  Key_Activities
 * File Name:     Key_Activities.cpp
 * File Function: 关键活动的实现
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#include <iostream>
#include <conio.h>
#include <climits>
#include <iomanip>

// 常变量的定义
const int MAX_TASK_HANDOVER = 10;
const int MAX_TASK = MAX_TASK_HANDOVER * (MAX_TASK_HANDOVER - 1);

struct Edge
{
	bool exist;
	int weight;
};

template <typename Type>
class MyDirectedGraph
{
private:
	int maxVertices;
	int vertexCount;
	int edgeCount;
	Type* vertices;
	Edge** graph;
	int findVertexIndex(const Type& vertex) const;
public:
	// 基础功能
	MyDirectedGraph(int _maxVertices);
	~MyDirectedGraph();
	int getVertexCount() { return vertexCount; }
	int getEdgeCount() { return edgeCount; }
	bool addVertex(const Type& vertex);
	bool addEdge(const Type& vertexA, const Type& vertexB, int weight);
	bool findEdge(const Type& vertexA, const Type& vertexB);
	// 关键活动
	bool topologicalSort(); 
	void printCriticalActivities(); 
};

template <typename Type>
int MyDirectedGraph<Type>::findVertexIndex(const Type& vertex) const
{
	for (int i = 0; i < vertexCount; i++) {
		if (vertices[i] == vertex) {
			return i;
		}
	}
	return -1;
}

template <typename Type>
MyDirectedGraph<Type>::MyDirectedGraph(int _maxVertices)
	:maxVertices(_maxVertices), vertexCount(0), edgeCount(0)
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
}

template <typename Type>
MyDirectedGraph<Type>::~MyDirectedGraph()
{
	delete[] vertices;
	for (int i = 0; i < maxVertices; ++i) {
		delete[] graph[i];
	}
	delete[] graph;
}

template <typename Type>
bool MyDirectedGraph<Type>::addVertex(const Type& vertex)
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
bool MyDirectedGraph<Type>::addEdge(const Type& from, const Type& to, int weight)
{
	int indexfrom = findVertexIndex(from);
	int indexto = findVertexIndex(to);
	if (indexfrom == -1 || indexto == -1 || indexfrom == indexto) {
		return false;
	}
	if (!graph[indexfrom][indexto].exist) {
		graph[indexfrom][indexto].exist = true;
		edgeCount++;
	}
	graph[indexfrom][indexto].weight = weight;
	return true;
}

template <typename Type>
bool MyDirectedGraph<Type>::findEdge(const Type& from, const Type& to)
{
	int indexfrom = findVertexIndex(from);
	int indexto = findVertexIndex(to);
	if (indexfrom == -1 || indexto == -1 || indexfrom == indexto) {
		return false;
	}
	if (!graph[indexfrom][indexto].exist) {
		return false;
	}
	return true;
}

template <typename Type>
bool MyDirectedGraph<Type>::topologicalSort()
{
	// 存储每个节点的入度
	int* inDegree = new(std::nothrow) int[maxVertices];
	if (inDegree == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < maxVertices; i++) {
		inDegree[i] = 0;
	}

	for (int i = 0; i < maxVertices; i++) {
		for (int j = 0; j < maxVertices; j++) {
			if (graph[i][j].exist) { // 如果存在边，从i到j
				inDegree[j]++;  // 增加j的入度
			}
		}
	}

	// 使用栈保存入度为0的节点
	int* stack = new int[maxVertices];
	int top = -1;
	for (int i = 0; i < maxVertices; i++) {
		if (inDegree[i] == 0) {
			stack[++top] = i;
		}
	}

	int processedCount = 0; // 记录已处理的节点数

	while (top != -1) {
		int node = stack[top--];
		processedCount++;
		for (int i = 0; i < maxVertices; i++) {
			if (graph[node][i].exist) {  // 如果有边 node -> i
				if (--inDegree[i] == 0) {  // 入度减1，如果变为0，则可以加入栈中
					stack[++top] = i;
				}
			}
		}
	}

	// 如果处理的节点数少于图中的总节点数，则说明存在环，不能完成拓扑排序
	if (processedCount != vertexCount) {
		std::cout << "Error: The graph contains a cycle, topological sort is not possible." << std::endl;
		return false;
	}

	delete[] inDegree;
	delete[] stack;
	return true;
}

template <typename Type>
void MyDirectedGraph<Type>::printCriticalActivities()
{
	int* earliestStart = new int[maxVertices];
	int* latestStart = new int[maxVertices];
	if (earliestStart == nullptr || latestStart == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}

	// 初始化最早开始时间和最晚开始时间
	for (int i = 0; i < maxVertices; i++) {
		earliestStart[i] = 0;
		latestStart[i] = INT_MAX;
	}

	// 计算最早开始时间（从源点开始）
	for (int i = 0; i < maxVertices; i++) {
		for (int j = 0; j < maxVertices; j++) {
			if (graph[i][j].exist) {
				earliestStart[j] = std::max(earliestStart[j], earliestStart[i] + graph[i][j].weight);
			}
		}
	}

	// 计算最晚开始时间（从汇点开始）
	latestStart[maxVertices - 1] = earliestStart[maxVertices - 1];
	for (int i = maxVertices - 2; i >= 0; i--) {
		for (int j = 0; j < maxVertices; j++) {
			if (graph[i][j].exist) {
				latestStart[i] = std::min(latestStart[i], latestStart[j] - graph[i][j].weight);
			}
		}
	}

	// 输出关键活动及最小时间
	std::cout << ">>> 关键活动及最小时间: " << std::endl;
	for (int i = 0; i < maxVertices; i++) {
		for (int j = 0; j < maxVertices; j++) {
			if (graph[i][j].exist && earliestStart[i] == latestStart[j] - graph[i][j].weight) {
				std::cout << "关键活动: " << vertices[i] << " -> " << vertices[j]
					<< " | 最早开始时间: " << earliestStart[i] << std::endl;
			}
		}
	}

	delete[] earliestStart;
	delete[] latestStart;
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

int main()
{
	/* 系统进入提示语 */
	std::cout << ">>> 欢迎使用任务调度管理系统" << std::endl << std::endl;

	/* 输入任务交接点和任务的数量 */
	int N = inputInteger(1, MAX_TASK_HANDOVER, "请输入任务交接点的数量");
	int M = inputInteger(1, MAX_TASK, "请输入任务的数量");

	/* 初始化有向图 */
	MyDirectedGraph<int> graph(N);
	for (int i = 1; i <= N; i++)
		graph.addVertex(i);

	std::cout << ">>> 请输入每个任务的交接点及任务所需时长(请按照顺序输入): " << std::endl;
	std::cout << ">>> 对于每个任务，请输入两个交接点的编号（从1开始）以及该任务的时长" << std::endl;
	std::cout << ">>> 例如，输入“1 2 5”表示任务从交接点1到交接点2，任务所需时长为5单位" << std::endl << std::endl;

	for (int i = 0; i < M; i++) {
		int from, to, weight;
		std::cout << "任务" << std::setw(3) << i + 1 << ": ";
		std::cin >> from >> to >> weight;
		if (from <= 0 || from > N || to <= 0 || to > N || weight <= 0) {
			std::cout << std::endl << ">>> 输入无效，请确保交接点编号在范围内，并且任务时长为正整数" << std::endl;
			i--;
			continue;
		}
		graph.addEdge(from, to, weight);
	}

	std::cout << std::endl;
	std::cout << ">>> 正在进行拓扑排序..." << std::endl;

	if (!graph.topologicalSort()) {
		std::cout << ">>> 无法完成拓扑排序: 存在环路，无法完成任务调度 " << std::endl;
	}
	else {
		std::cout << ">>> 拓扑排序成功，正在计算关键活动..." << std::endl << std::endl;
		graph.printCriticalActivities();
	}

	// 退出程序
	return 0;
}
