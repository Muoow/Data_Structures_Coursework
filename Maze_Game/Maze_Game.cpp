/****************************************************************
 * Project Name:  Maze_Game
 * File Name:     Maze_Game.cpp
 * File Function: 勇闯迷宫游戏
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <conio.h>

using namespace std;

// 常变量的定义
const int MIN_ROWCOL = 1;
const int MAX_ROWCOL = 99;
const int START_ROW = 1;
const int START_COL = 1;
enum Direction { Up, Down, Left, Right, None };

template <typename Type>
struct MyLinkNode {
	Type data;
	MyLinkNode<Type>* link;
	MyLinkNode(MyLinkNode<Type>* ptr = nullptr) : link(ptr) {}
	MyLinkNode(const Type& item, MyLinkNode<Type>* ptr = nullptr) : data(item), link(ptr) {}
};

// 队列的定义
template <typename Type>
class MyQueue
{
private:
	MyLinkNode<Type>* front;
	MyLinkNode<Type>* rear;
	int count;
public:
	MyQueue() : front(nullptr), rear(nullptr), count(0) {}
	~MyQueue() { makeEmpty(); }
	bool isEmpty() const;
	void makeEmpty();
	int Size()const;
	void enqueue(const Type& item);
	bool dequeue(Type& item);
	bool getHead(Type& item);
};

template <typename Type>
bool MyQueue<Type>::isEmpty() const
{
	return front == nullptr;
}

template <typename Type>
void MyQueue<Type>::makeEmpty()
{
	MyLinkNode<Type>* current;
	while (front != nullptr) {
		current = front;
		front = front->link;
		delete current;
	}
	rear = nullptr;
	count = 0;
}

template <typename Type>
int MyQueue<Type>::Size() const
{
	return count;
}

template <typename Type>
void MyQueue<Type>::enqueue(const Type& item)
{
	MyLinkNode<Type>* newNode = new(std::nothrow)MyLinkNode<Type>(item, nullptr);
	if (newNode == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	if (isEmpty()) {
		front = rear = newNode;
	}
	else {
		rear->link = newNode;
		rear = newNode;
	}
	count++;
}

template <typename Type>
bool MyQueue<Type>::dequeue(Type& item)
{
	if (isEmpty()) {
		return false;
	}
	MyLinkNode<Type>* temp = front;
	item = front->data;
	front = front->link;
	delete temp;
	if (front == nullptr) {
		rear = nullptr;
	}
	count--;
	return true;
}

template <typename Type>
bool MyQueue<Type>::getHead(Type& item)
{
	if (isEmpty()) {
		return false;
	}
	item = front->data;
	return true;
}

template <typename Type>
class MyStack 
{
private:
	MyLinkNode<Type>* topNode;
	int count;
	int max_size;
public:
	MyStack() : topNode(nullptr), count(0), max_size(INT_MAX) {}
	MyStack(int size) : topNode(nullptr), count(0), max_size(size) {}
	~MyStack() { makeEmpty(); }
	bool isEmpty() const;
	void makeEmpty();
	int Size() const;
	bool Push(Type& item);
	bool Pop(Type& item);
	bool getTop(Type& item);
};

template <typename Type>
bool MyStack<Type>::isEmpty() const 
{
	return topNode == nullptr;
}

template <typename Type>
void MyStack<Type>::makeEmpty()
{
	MyLinkNode<Type>* current;
	while (topNode != nullptr) {
		current = topNode;
		topNode = topNode->link;
		delete current;
	}
}

template <typename Type>
int MyStack<Type>::Size() const 
{
	return count;
}

template <typename Type>
bool MyStack<Type>::Push(Type& item)
{
	if (count >= max_size) {
		return false;
	}
	topNode = new(nothrow) MyLinkNode<Type>(item, topNode);
	if (topNode == nullptr) {
		cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	count++;
	return true;
}

template <typename Type>
bool MyStack<Type>::Pop(Type& item) 
{
	if (isEmpty()) {
		return false;
	}
	MyLinkNode<Type>* current = topNode;
	topNode = topNode->link;
	item = current->data;
	delete current;
	count--;
	return true;
}

template <typename Type>
bool MyStack<Type>::getTop(Type& item) 
{
	if (isEmpty()) {
		return false;
	}
	item = topNode->data;
	return true;
}

// 输入一个指定范围内的奇数
int inputOdd(int lowerLimit, int upperLimit, const char* prompt) 
{
	cout << ">>> 请输入" << prompt << " [奇数范围: " << lowerLimit << "~" << upperLimit << "]: ";
	int input;
	while (true) {
		cin >> input;
		if (cin.good() && input >= lowerLimit && input <= upperLimit && input % 2 == 1) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			return input;
		}
		else {
			cerr << ">>> " << prompt << "输入不合法，请重新输入！" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}
	std::cout << std::endl;
}

// 迷宫类的定义
class Maze 
{
private:
	int rows;
	int cols;
	int startRow;
	int startCol;
	int targetRow;
	int targetCol;
	int currRow;
	int currCol;
	struct MazePoint;
	MazePoint** maze;
	MazePoint* mazePointList;
	int mazePointListCount;
	bool pushList(const MazePoint point);
	bool popList(int index);
public:
	Maze(int _rows, int _cols, int _startRow, int _startCol, int _targetRow, int _targetCol);
	~Maze();
	bool isValid(int row, int col) const;
	void generateMaze();
	void generateMaze(int row, int col);
	void findAdjacentWalls();
	void ouputMaze();
	void clearPaths();
	bool DFS();
	bool BFS();
};

struct Maze::MazePoint 
{
	int row = 0;
	int col = 0;
	bool isWall = true;
	bool isPath = false;
};

bool Maze::pushList(const MazePoint point) 
{
	if (mazePointListCount >= rows * cols) {
		return false;
	}
	mazePointList[mazePointListCount] = point;
	mazePointListCount++;
	return true;
}

bool Maze::popList(int index)
{
	if (mazePointListCount == 0) {
		return false;
	}
	if (index < 0 || index >= mazePointListCount) {
		return false;
	}
	for (int i = index; i < mazePointListCount - 1; i++)
		mazePointList[i] = mazePointList[i + 1];
	mazePointListCount--;
	return true;
}

// 构造函数
Maze::Maze(int _rows, int _cols, int _startRow, int _startCol, int _targetRow, int _targetCol): 
	rows(_rows), cols(_cols), startRow(_startRow), startCol(_startCol), targetRow(_targetRow), targetCol(_targetCol), maze(nullptr)
{
	currRow = startRow;
	currCol = startCol;
	maze = new (nothrow)MazePoint * [rows];
	if (maze == nullptr) {
		cerr << "Error: Memory allocation failed." << endl;
		exit(-1);
	}
	for (int i = 0; i < rows; i++) {
		maze[i] = new(nothrow) MazePoint[cols];
		if (maze[i] == nullptr) {
			cerr << "Error: Memory allocation failed." << endl;
			exit(-1);
		}
		for (int j = 0; j < cols; j++) {
			maze[i][j].row = i;
			maze[i][j].col = j;
		}
	}
	mazePointListCount = 0;
	mazePointList = new(nothrow) MazePoint[rows * cols];
	if (mazePointList == NULL) {
		cerr << "Error: Memory allocation failed." << endl;
		exit(-1);
	}
}

// 析构函数
Maze::~Maze() 
{
	for (int i = 0; i < rows; i++) 
		delete[] maze[i];
	delete[] maze;
	delete[] mazePointList;
}

bool Maze::isValid(int row, int col) const
{
	return row >= 0 && col >= 0 && row < rows && col < cols;
}

void Maze::generateMaze()
{
	maze[currRow][currCol].isWall = false;
	findAdjacentWalls();
	while (mazePointListCount != 0) {
		int index = rand() % mazePointListCount;
		MazePoint currPoint = mazePointList[index];
		currRow = currPoint.row;
		currCol = currPoint.col;
		int count = 0;
		if (!maze[currRow - 1][currCol].isWall)
			count++;
		if (!maze[currRow + 1][currCol].isWall)
			count++;
		if (!maze[currRow][currCol - 1].isWall)
			count++;
		if (!maze[currRow][currCol + 1].isWall)
			count++;
		if (count == 1) {
			maze[currRow][currCol].isWall = false;
			findAdjacentWalls();
		}
		popList(index);
	}
}

void Maze::findAdjacentWalls() 
{
	if (currRow > 1 && maze[currRow - 1][currCol].isWall)
		pushList(maze[currRow - 1][currCol]);
	if (currRow < rows - 2 && maze[currRow + 1][currCol].isWall)
		pushList(maze[currRow + 1][currCol]);
	if (currCol > 1 && maze[currRow][currCol - 1].isWall)
		pushList(maze[currRow][currCol - 1]);
	if (currCol < cols - 2 && maze[currRow][currCol + 1].isWall)
		pushList(maze[currRow][currCol + 1]);
}

// 生成迷宫算法
void Maze::generateMaze(int row, int col) 
{
	Direction directions[4] = { Up,Down,Left,Right };
	for (int i = 0; i < 4; i++) {
		int r = rand() % 4;
		Direction temp = directions[i];
		directions[i] = directions[r];
		directions[r] = temp;
	}
	for (int i = 0; i < 4; i++) {
		int newRow = row;
		int newCol = col;
		switch (directions[i]) {
			case Up:
				newRow -= 2;
				break;
			case Down:
				newRow += 2;
				break;
			case Left:
				newCol -= 2; 
				break;
			case Right:
				newCol += 2; 
				break;
		}
		if (isValid(newRow, newCol) && maze[newRow][newCol].isWall) {
			maze[(row + newRow) / 2][(col + newCol) / 2].isWall = false;
			maze[newRow][newCol].isWall = false;
			generateMaze(newRow, newCol);
		}
	}
}

void Maze::ouputMaze()
{
	cout << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (maze[i][j].isWall)
				cout << "墙";
			else if (maze[i][j].isPath)
				cout << "×";
			else
				cout << "  ";
		}
		cout << endl;
	}
}

void Maze::clearPaths()
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			maze[i][j].isPath = false;
		}
	}
}

// 坐标结构体定义
typedef struct 
{
	int row;
	int col;
} Coordinate;

// 深度优先算法
bool Maze::DFS() 
{
	bool** visit = new (nothrow)bool* [rows];
	if (visit == nullptr) {
		cerr << "Error: Memory allocation failed." << endl;
		exit(-1);
	}
	for (int i = 0; i < rows; i++) {
		visit[i] = new(nothrow) bool[cols];
		if (visit[i] == nullptr) {
			cerr << "Error: Memory allocation failed." << endl;
			exit(-1);
		}
		for (int j = 0; j < cols; j++) {
			visit[i][j] = false;
		}
	}

	MyStack<Coordinate> DFS_Path(rows * cols);
	Coordinate start{ startRow ,startCol };
	DFS_Path.Push(start);

	while (!DFS_Path.isEmpty()) {
		Coordinate current;
		DFS_Path.getTop(current);
		currRow = current.row;
		currCol = current.col;
		visit[currRow][currCol] = true;

		if (currRow == targetRow && currCol == targetCol) {
			for (int i = 0; i < rows; i++)
				delete[] visit[i];
			delete[] visit;
			while (!DFS_Path.isEmpty()) {
				Coordinate pathPoint;
				DFS_Path.Pop(pathPoint);
				maze[pathPoint.row][pathPoint.col].isPath = true;
			}
			ouputMaze();
			return true;
		}
		
		Coordinate temp;
		if ((visit[currRow - 1][currCol] || maze[currRow - 1][currCol].isWall) &&
			(visit[currRow + 1][currCol] || maze[currRow + 1][currCol].isWall) &&
			(visit[currRow][currCol - 1] || maze[currRow][currCol - 1].isWall) &&
			(visit[currRow][currCol + 1] || maze[currRow][currCol + 1].isWall) ) {
			DFS_Path.Pop(temp);
		}
		else {
			if (!visit[currRow - 1][currCol] && !maze[currRow - 1][currCol].isWall) {
				temp = { currRow - 1 ,currCol };
				DFS_Path.Push(temp);
			}
			else if (!visit[currRow + 1][currCol] && !maze[currRow + 1][currCol].isWall) {
				temp = { currRow + 1 ,currCol };
				DFS_Path.Push(temp);
			}
			else if (!visit[currRow][currCol - 1] && !maze[currRow][currCol - 1].isWall) {
				temp = { currRow  ,currCol - 1 };
				DFS_Path.Push(temp);
			}
			else if (!visit[currRow][currCol + 1] && !maze[currRow][currCol + 1].isWall) {
				temp = { currRow ,currCol + 1 };
				DFS_Path.Push(temp);
			}
		}
	}

	for (int i = 0; i < rows; i++)
		delete[] visit[i];
	delete[] visit;
	return false;
}

// 广度优先算法
bool Maze::BFS()
{
	// 初始化访问矩阵和父节点矩阵
	bool** visit = new (std::nothrow) bool* [rows];
	Coordinate** parent = new (std::nothrow) Coordinate * [rows];
	if (visit == nullptr || parent == nullptr) {
		cerr << "Error: Memory allocation failed." << endl;
		exit(-1);
	}
	for (int i = 0; i < rows; i++) {
		visit[i] = new (std::nothrow) bool[cols];
		parent[i] = new (std::nothrow) Coordinate[cols];
		if (visit[i] == nullptr || parent[i] == nullptr) {
			cerr << "Error: Memory allocation failed." << endl;
			exit(-1);
		}
		for (int j = 0; j < cols; j++) {
			visit[i][j] = false;
			parent[i][j] = { -1, -1 }; // 初始化父节点为无效坐标
		}
	}

	// 使用自定义队列初始化和路径记录
	MyQueue<Coordinate> BFS_Queue;
	Coordinate start{ startRow, startCol };
	BFS_Queue.enqueue(start);
	visit[startRow][startCol] = true;

	while (!BFS_Queue.isEmpty()) {
		Coordinate current;
		BFS_Queue.dequeue(current);

		// 判断是否到达目标位置
		if (current.row == targetRow && current.col == targetCol) {
			// 重建路径
			Coordinate pathPoint = current;
			while (pathPoint.row != startRow || pathPoint.col != startCol) {
				maze[pathPoint.row][pathPoint.col].isPath = true;
				pathPoint = parent[pathPoint.row][pathPoint.col];
			}
			maze[startRow][startCol].isPath = true;

			// 释放内存
			for (int i = 0; i < rows; i++) {
				delete[] visit[i];
				delete[] parent[i];
			}
			delete[] visit;
			delete[] parent;

			ouputMaze();
			return true;
		}

		// 遍历当前节点的邻接节点
		static const int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
		for (const auto& dir : directions) {
			int newRow = current.row + dir[0];
			int newCol = current.col + dir[1];

			// 确保新位置在范围内且未访问且不是墙
			if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
				!visit[newRow][newCol] && !maze[newRow][newCol].isWall) {
				Coordinate next{ newRow, newCol };
				BFS_Queue.enqueue(next);
				visit[newRow][newCol] = true;
				parent[newRow][newCol] = current; // 记录父节点
			}
		}
	}

	// 如果队列为空且未找到目标，释放内存
	for (int i = 0; i < rows; i++) {
		delete[] visit[i];
		delete[] parent[i];
	}
	delete[] visit;
	delete[] parent;

	return false; // 未找到目标
}

void mazeGame() 
{
	std::cout << "=== 勇闯迷宫游戏 ===" << std::endl << std::endl;

	int _rows = inputOdd(MIN_ROWCOL, MAX_ROWCOL, "行数");
	int _cols = inputOdd(MIN_ROWCOL, MAX_ROWCOL, "列数");

	Maze maze(_rows, _cols, START_ROW, START_COL, _rows - 2, _cols - 2);
	maze.generateMaze(START_ROW, START_COL);

	// 输出生成的迷宫
	std::cout << std::endl << ">>> 生成的迷宫如下: " << std::endl;
	maze.ouputMaze();

	// DFS算法生成路径
	std::cout << std::endl << ">>> DFS算法生成的路径为: " << std::endl;
	maze.DFS();

	// 清除上个算法产生的路径
	maze.clearPaths();

	// BFS算法生成路径
	std::cout << std::endl << ">>> BFS算法生成的路径为: " << std::endl;
	maze.BFS();
}

int main() 
{
	// 设置随机数种子
	srand((unsigned int)(time(0)));

	// 进入迷宫游戏
	mazeGame();

	// 等待回车键再退出
	std::cout << std::endl << "Press Enter to Quit" << std::endl;
	while (_getch() != '\r')
		continue;

	// 退出程序
	return 0;
}