#include<iostream>
#include<limits>

struct Edge {
	bool exist;
	int weight;
};

template <typename Type>
class MyUndirectedGraph {
private:
	int maxVertices;
	int vertexCount;
	Type* vertices;
	Edge** graph;
	int findVertexIndex(const Type& vertex) const;
public:
	MyUndirectedGraph(int _maxVertices, bool _hasdirect);
	~MyUndirectedGraph();
	bool addVertex(const Type& vertex);
	bool addEdge(const Type& vertexA, const Type& vertexB, int weight);
	void MyUndirectedGraph<Type>::primMST(const Type& vertex);
};

template <typename Type>
int MyUndirectedGraph<Type>::findVertexIndex(const Type& vertex) const {
	for (int i = 0; i < vertexCount; i++) {
		if (vertices[i] == vertex) {
			return i;
		}
	}
	return -1;
}

template <typename Type>
MyUndirectedGraph<Type>::MyUndirectedGraph(int _maxVertices, bool _hasdirect)
	:maxVertices(_maxVertices), hasdirect(_hasdirect), vertexCount(0) {
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
	for (int i = 0; i < maxVertices i++) {
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
MyUndirectedGraph<Type>::~MyUndirectedGraph() {
	delete[] vertices;
	for (int i = 0; i < maxVertices; ++i) {
		delete[] graph[i];
	}
	delete[] graph;
}

template <typename Type>
bool MyUndirectedGraph<Type>::addVertex(const Type& vertex) {
	if (vertexCount >= maxVertices) {
		return false;
	}
	if (findVertexIndex(vertex) != -1) {
		return false;
	}
	vertices[vertexCount++] = vertex;
	return true;
}

template <typename Type>
bool MyUndirectedGraph<Type>::addEdge(const Type& vertexA, const Type& vertexB, int weight) {

	int indexA = findVertexIndex(vertexA);
	int indexB = findVertexIndex(vertexB);

	if (indexA == -1 || indexB == -1 || indexA == indexB) {
		return false;
	}

	graph[indexA][indexB].exist = true;
	graph[indexA][indexB].weight = weight;

	graph[indexB][indexA].exist = true; 
	graph[indexB][indexA].weight = weight;

	return true;
}

template <typename Type>
void MyUndirectedGraph<Type>::primMST(const Type& vertex) {

}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt) {
	std::cout << "请输入" << prompt << " 整数范围: " << lowerLimit << "~" << upperLimit << "]：";
	int input;
	while (true) {
		std::cin >> input;
		if (std::cin.good() && input >= lowerLimit && input <= upperLimit) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return input;
		}
		else {
			std::cerr << ">>> " << prompt << "输入不合法，请重新输入！" << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

class PowerGrid {
private:
	MyUndirectedGraph<char> gird;
public:
	void createGridVertices();
	void createGridEdges();
	void constructMinimumSpanningTree();
	void printMinimumSpanningTree();
};

void PowerGrid::createGridVertices() {

}

void PowerGrid::createGridEdges() {

}

void PowerGrid::constructMinimumSpanningTree() {

}

void PowerGrid::printMinimumSpanningTree() {

}

int main() {

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



}