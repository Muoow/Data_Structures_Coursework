#include<iostream>
#include<limits>
#include<conio.h>
#include<new>

template <typename Type>
struct MyLinkNode 
{
	Type data;
	MyLinkNode<Type>* link;
	MyLinkNode(MyLinkNode<Type>* ptr = nullptr) : link(ptr) {}
	MyLinkNode(const Type& item, MyLinkNode<Type>* ptr = nullptr) : data(item), link(ptr) {}
};

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

template <typename Type>
bool MyQueue<Type>::getHead(Type& item) 
{
	if (isEmpty()) {
		return false;
	}
	item = front->data;
	return true;
}

const int PROCESSSPEED_A = 2;
const int PROCESSSPEED_B = 1;

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

void Bank::lineUp() 
{
	std::cout << std::endl;
	int count = inputInteger(1, 1000, "顾客人数");

	std::cout << "请依次输入顾客编号(正整数):";

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

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

}

void Bank::dealWith() 
{
	std::cout << std::endl << ">>> 开始处理顾客队列..." << std::endl;

	std::cout << std::endl << ">>> ";

	while (!queueA.isEmpty() || !queueB.isEmpty()) {

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

	std::cout << std::endl << ">>> 已处理完毕，当前没有顾客排队。" << std::endl;

}

bool Bank::receiveCommand() 
{
	char command;

	std::cout << std::endl << ">>> 是否继续服务下一批顾客？(Y/N):";

	while (true) {
		command = _getch();
		if (command == 0 || command == -32) {
			command = _getch();
		}
		else if (command == 'y' || command == 'Y' || command == 'n' || command == 'N') {
			std::cout << "[" << command << "]" << std::endl << std::endl;
			break;
		}
	}
	
	return (command == 'Y' || command == 'y');

}

int main()
{
	std::cout << std::endl << ">>> 欢迎进入银行排队系统!" << std::endl;

	Bank bank;

	do {
		bank.lineUp();
		bank.dealWith();
	} while (bank.receiveCommand());

	std::cout << std::endl << ">>> 银行排队系统结束，感谢使用!" << std::endl;

	return 0;

}