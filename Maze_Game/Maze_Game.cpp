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

template <typename Type>
class MyQueue {
private:
	MyLinkNode<Type>* front;
	MyLinkNode<Type>* rear;
public:
	MyQueue() : front(nullptr), rear(nullptr) {}
	~MyQueue();
	bool isEmpty() const;
	void enqueue(const Type& item);
	bool dequeue(Type& item);
	bool getHead(Type& item);
};

template <typename Type>
MyQueue<Type>::~MyQueue() {
	while (!isEmpty()) {

	}
}

template <typename Type>
bool MyQueue<Type>::isEmpty() const {
	return front == nullptr;
}

template <typename Type>
void MyQueue<Type>::enqueue(const Type& item) {
	MyLinkNode<Type>* newNode = new(nothrow)MyLinkNode<Type>(item, nullptr);
	if (newNode == nullptr) {
		cerr << "Error: Memory allocation failed." << endl;
		exit(-1);
	}
	if (isEmpty()) {
		front = rear = newNode;
	}
	else {
		rear->link = newNode;
		rear = newNode;
	}
}

template <typename Type>
bool MyQueue<Type>::dequeue(Type& item) {
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
	return true;
}

template <typename Type>
bool MyQueue<Type>::getHead(Type& item) {
	if (isEmpty()) {
		return false;
	}

}

template <typename Type>
class MyStack {
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
bool MyStack<Type>::isEmpty() const {
	return topNode == nullptr;
}

template <typename Type>
void MyStack<Type>::makeEmpty(){
	MyLinkNode<Type>* current;
	while (topNode != nullptr) {
		current = topNode;
		topNode = topNode->link;
		delete current;
	}
}

template <typename Type>
int MyStack<Type>::Size() const {
	return count;
}

template <typename Type>
bool MyStack<Type>::Push(Type& item) {
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
bool MyStack<Type>::Pop(Type& item) {
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
bool MyStack<Type>::getTop(Type& item) {
	if (isEmpty()) {
		return false;
	}
	item = topNode->data;
	return true;
}

int inputOdd(int lowerLimit, int upperLimit, const char* prompt) {
	cout << "请输入" << prompt << " [奇数范围: " << lowerLimit << "~" << upperLimit << "]：";
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
}

class Maze {
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
	bool DFS();

};

struct Maze::MazePoint {
	int row = 0;
	int col = 0;
	bool isWall = true;
	bool isPath = false;
};

bool Maze::pushList(const MazePoint point) {
	if (mazePointListCount >= rows * cols) {
		return false;
	}
	mazePointList[mazePointListCount] = point;
	mazePointListCount++;
	return true;
}

bool Maze::popList(int index) {
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

Maze::Maze(int _rows, int _cols, int _startRow, int _startCol, int _targetRow, int _targetCol): 
	rows(_rows), cols(_cols), startRow(_startRow), startCol(_startCol), targetRow(_targetRow), targetCol(_targetCol), maze(nullptr){
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

Maze::~Maze() {
	for (int i = 0; i < rows; i++) 
		delete[] maze[i];
	delete[] maze;
	delete[] mazePointList;
}

bool Maze::isValid(int row, int col) const{
	return row >= 0 && col >= 0 && row < rows && col < cols;
}

void Maze::generateMaze() {
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

void Maze::findAdjacentWalls() {
	if (currRow > 1 && maze[currRow - 1][currCol].isWall)
		pushList(maze[currRow - 1][currCol]);
	if (currRow < rows - 2 && maze[currRow + 1][currCol].isWall)
		pushList(maze[currRow + 1][currCol]);
	if (currCol > 1 && maze[currRow][currCol - 1].isWall)
		pushList(maze[currRow][currCol - 1]);
	if (currCol < cols - 2 && maze[currRow][currCol + 1].isWall)
		pushList(maze[currRow][currCol + 1]);
}

void Maze::generateMaze(int row, int col) {
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

void Maze::ouputMaze() {
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

typedef struct {
	int row;
	int col;
} Coordinate;

bool Maze::DFS() {
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

void mazeGame() {
	int _rows = inputOdd(MIN_ROWCOL, MAX_ROWCOL, "行数");
	int _cols = inputOdd(MIN_ROWCOL, MAX_ROWCOL, "列数");
	Maze maze(_rows, _cols, START_ROW, START_COL, _rows - 2, _cols - 2);
	maze.generateMaze(START_ROW, START_COL);
	maze.ouputMaze();
	maze.DFS();
}

int main() {
	srand((unsigned int)(time(0)));

	mazeGame();
	return 0;
}