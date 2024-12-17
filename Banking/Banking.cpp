/****************************************************************
 * Project Name:  Banking
 * File Name:     Banking.cpp
 * File Function: 银行业务处理
 * Author:        张翔
 * Update Date:   2024/12/7
 ****************************************************************/

#include<iostream>
#include<limits>
#include<conio.h>
#include<new>

// 常变量的定义
const int PROCESSSPEED_A = 2;
const int PROCESSSPEED_B = 1;

// MyLinkNode结构体的定义
template <typename Type>
struct MyLinkNode 
{
	Type data;
	MyLinkNode<Type>* link;
	MyLinkNode(MyLinkNode<Type>* ptr = nullptr) : link(ptr) {}
	MyLinkNode(const Type& item, MyLinkNode<Type>* ptr = nullptr) : data(item), link(ptr) {}
};

// MyQueue类的定义
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
	void enQueue(const Type& item);
	bool deQueue(Type& item);
	bool getHead(Type& item);
};

// 判断队列是否为空
template <typename Type>
bool MyQueue<Type>::isEmpty() const 
{
	return front == nullptr;
}

// 将队列设置为空
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

// 获取当前队列内的节点数
template <typename Type>
int MyQueue<Type>::Size() const 
{
	return count;
}

// 从最后将节点加入队列内
template <typename Type>
void MyQueue<Type>::enQueue(const Type& item) 
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

// 从最前面将节点移出队列
template <typename Type>
bool MyQueue<Type>::deQueue(Type& item) 
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

// 获取队列的头节点
template <typename Type>
bool MyQueue<Type>::getHead(Type& item) 
{
	if (isEmpty()) {
		return false;
	}
	item = front->data;
	return true;
}

// Bank类的定义
class Bank 
{
private:
	MyQueue<int> queueA;
	MyQueue<int> queueB;
public:
	Bank() { ; }
	~Bank() { ; }
	void lineUp();
	void dealWith();
	bool receiveCommand();
};

// 输入一个指定范围大小的整数
int inputInteger(int lowerLimit, int upperLimit, const char* prompt) 
{
	std::cout << "请输入" << prompt << " 整数范围: " << lowerLimit << "~" << upperLimit << "]：";
	int input;
	while (true) {
		std::cin >> input;
		if (std:: cin.good() && input >= lowerLimit && input <= upperLimit) {
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

// 通过读取输入进行排队
void Bank::lineUp() 
{
	// 输入顾客人数
	std::cout << std::endl;
	int count = inputInteger(1, 1000, "顾客人数");

	// 顾客编号输入提示
	std::cout << "请依次输入顾客编号(正整数):";
	// 依次输入
	for (int i = 0; i < count; i++) {

		unsigned int customerNum;
		std::cin >> customerNum;

		while (std::cin.fail()) {
			std::cin.clear();
			std::cerr << "输入无效，请输入一个正整数:" << std::endl;
			std::cin >> customerNum;
		}
		if (customerNum % 2 == 1) {
			queueA.enQueue(customerNum);
		}
		else {
			queueB.enQueue(customerNum);
		}
	}
	// 及时清除缓冲区
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

// 根据两个窗口处理速度不同输出顾客完成业务的顺序
void Bank::dealWith() 
{
	// 处理开始提示
	std::cout << std::endl << ">>> 开始处理顾客队列..." << std::endl;
	std::cout << std::endl << ">>> ";

	while (!queueA.isEmpty() || !queueB.isEmpty()) {
		// 根据两个窗口的处理速度进行处理
		for (int i = 0; i < PROCESSSPEED_A && !queueA.isEmpty(); i++) {
			int customer;
			queueA.deQueue(customer);
			std::cout << customer;

			if (queueA.isEmpty() && queueB.isEmpty()) {
				std::cout << std::endl;
				break;
			}
			else {
				std::cout << " "; 
			}
		}

		for (int i = 0; i < PROCESSSPEED_B && !queueB.isEmpty(); i++) {
			int customer;
			queueB.deQueue(customer);
			std::cout << customer;

			if (queueA.isEmpty() && queueB.isEmpty()) {
				std::cout << std::endl;
				break;
			}
			else {
				std::cout << " "; 
			}
		}
	}
	// 业务处理结束提示
	std::cout << std::endl << ">>> 已处理完毕，当前没有顾客排队。" << std::endl;
}

// 用户输入判断是否要服务下一篇顾客
bool Bank::receiveCommand() 
{
	char command;
	// 输入提示
	std::cout << std::endl << ">>> 是否继续服务下一批顾客？(Y/N):";

	while (true) {
		command = _getch();
		if (command == 0 || command == -32) {
			command = _getch();
		}
		else if (command == 'y' || command == 'Y' || command == 'n' || command == 'N') {
			std::cout << "[" << command << "]" << std::endl;
			break;
		}
	}
	// 返回值
	return (command == 'Y' || command == 'y');
}

int main()
{
	// 程序开始提示
	std::cout << std::endl << ">>> 欢迎进入银行排队系统!" << std::endl;
	// 初始化bank实例
	Bank bank;

	// 进行排队和业务处理操作
	do {
		bank.lineUp();
		bank.dealWith();
	} while (bank.receiveCommand());

	// 程序结束提示
	std::cout << std::endl << ">>> 银行排队系统结束，感谢使用!" << std::endl;
	return 0;
}