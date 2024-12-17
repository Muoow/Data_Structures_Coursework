/****************************************************************
 * Project Name:  Repair_the_Ranch
 * File Name:     Repair_the_Ranch.cpp
 * File Function: 修复牧场
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#include <iostream>
#include <conio.h>
#include <new>

// 优先队列的定义
template<typename Type>
class MyPriorityQueue
{
private:
	Type* elements;
	int maxsize;
	int currsize;
	void heapifyUp(int index);
	void heapifyDown(int index);
public:
	MyPriorityQueue(int _maxsize);
	~MyPriorityQueue() { delete[]elements; }
	bool isEmpty(void) const { return currsize == 0; }
	bool isFull(void) const { return currsize == maxsize; }
	int getSize(void) const { return currsize; }
	bool insert(const Type& item);
	bool remove(Type& item);
	bool getFront(Type& item) const;
};

// 上浮函数
template<typename Type>
void MyPriorityQueue<Type>::heapifyUp(int index) 
{
	while (index > 0) {

		int parent = (index - 1) / 2;

		if (elements[index] < elements[parent]) {
			Type temp = elements[parent];
			elements[parent] = elements[index];
			elements[index] = temp;
			index = parent;
		}
		else {
			break;
		}

	}

}

// 下沉函数
template<typename Type>
void MyPriorityQueue<Type>::heapifyDown(int index) 
{
	while (true) {

		int least = index;

		int left = 2 * index + 1;
		int right = 2 * index + 2;

		if (left < currsize && elements[left] < elements[least]) {
			least = left;
		}

		if (right < currsize && elements[right] < elements[least]) {
			least = right;
		}

		if (least != index) {
			Type temp = elements[least];
			elements[least] = elements[index];
			elements[index] = temp;
			index = least;
		}
		else {
			break;
		}

	}
}

// 优先队列的初始化
template<typename Type>
MyPriorityQueue<Type>::MyPriorityQueue(int _maxsize)
{
	maxsize = _maxsize;
	currsize = 0;
	elements = new (std::nothrow) Type[maxsize];
	if (elements == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
}

// 从队列尾部插入新节点
template<typename Type>
bool MyPriorityQueue<Type>::insert(const Type& item) 
{
	if (isFull())
		return false;
	else {
		elements[currsize++] = item;
		heapifyUp(currsize - 1);
		return true;
	}
}

// 删去队列最前面的元素
template<typename Type>
bool MyPriorityQueue<Type>::remove(Type& item)
{
	if (isEmpty())
		return false;
	else {
		item = elements[0];
		elements[0] = elements[--currsize];
		heapifyDown(0);
		return true;
	}
}

// 获取优先队列最前面的节点
template<typename Type>
bool MyPriorityQueue<Type>::getFront(Type& item) const
{
	if (isEmpty())
		return false;
	else {
		item = elements[0];
		return true;
	}
}

// 输入一个指定范围内的整数
int inputInteger(int lowerLimit, int upperLimit, const char* prompt)
{
	std::cout << ">>> " << "请输入" << prompt << " 整数范围: " << lowerLimit << "~" << upperLimit << "]: ";
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

int main()
{
	// 输入要切成的块数
	int num = inputInteger(1, INT_MAX, "请输入要将木头切成的块数: ");
	MyPriorityQueue<int> pq(num);
	int totalCost = 0;

	// 依次输入每段木头的长度
	std::cout << std::endl << "请依次输入 " << num << " 段木头的长度: " ;
	for (int i = 0; i < num; i++) {
		int length;
		std::cin >> length;
		pq.insert(length);
	}

	// 找出最小成本 
	while (pq.getSize() > 1) {
		int first, second;
		pq.remove(first);
		pq.remove(second);
		int cost = first + second;
		pq.insert(cost);
		totalCost += cost;
	}

	// 输出最小成本
	std::cout << std::endl << ">>> 最小切割成本为: " << totalCost << std::endl;

	// 等待回车键再退出
	std::cout << std::endl << "Press Enter to Quit" << std::endl;
	while (_getch() != '\r')
		continue;

	// 退出程序
	return 0;
}
