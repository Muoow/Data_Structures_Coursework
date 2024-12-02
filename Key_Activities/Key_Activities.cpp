#include<iostream>
#include<conio.h>

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
	MyDirectedGraph(int _maxVertices);
	~MyDirectedGraph();
	int getVertexCount() { return vertexCount; }
	int getEdgeCount() { return edgeCount; }
	bool addVertex(const Type& vertex);
	bool addEdge(const Type& vertexA, const Type& vertexB, int weight);
	bool findEdge(const Type& vertexA, const Type& vertexB);

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

int main()
{

}