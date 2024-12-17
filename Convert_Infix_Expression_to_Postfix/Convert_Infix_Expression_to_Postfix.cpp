/****************************************************************
 * Project Name:  Convert_Infix_Expression_to_Postfix
 * File Name:     Convert_Infix_Expression_to_Postfix.cpp
 * File Function: 表达式转换的实现
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <new>
#include <conio.h>
#include <sstream>

// 常变量的定义
const int MAX_LENGTH = 256;
const int MAX_TOKEN = 10;

// 链表节点的定义
template <typename Type>
struct MyLinkNode
{
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

// 判断当前队列是否为空
template <typename Type>
bool MyQueue<Type>::isEmpty() const 
{
	return front == nullptr;
}

// 将队列重置为空
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

// 获取队列大小
template <typename Type>
int MyQueue<Type>::Size() const 
{
	return count;
}

// 向队列中插入
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

// 从队列中弹出
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

// 栈的定义
template <typename Type>
class MyStack 
{
private:
	MyLinkNode<Type>* topNode;
	int count;
	int max_size;
public:
	MyStack() : topNode(nullptr), count(0), max_size(100) {}
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
	count = 0;
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
	topNode = new(std::nothrow) MyLinkNode<Type>(item, topNode);
	if (topNode == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
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

// 判断当前字符是否是运算符
bool isOperator(const char& c) 
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

// 运算符优先级判断
int precedence(const char* op)
{
	if (std::strcmp(op, "+") == 0 || std::strcmp(op, "-") == 0) {
		return 1;
	}
	if (std::strcmp(op, "*") == 0 || std::strcmp(op, "/") == 0) {
		return 2;
	}
	return 0;
}

// 应用运算符进行计算
int applyOp(int a, int b, char op) 
{
	switch (op) {
		case '+': 
			return a + b;
		case '-': 
			return a - b;
		case '*': 
			return a * b;
		case '/': 
			return a / b;
	}
	return 0;
}

// 运算式中元素结构体的定义
struct Token 
{
	char value[MAX_TOKEN];
	bool isNumber;
};

// 算术表达式的定义
class MyExpression 
{
private:
	char expression[MAX_LENGTH];
	MyStack<Token> s1;
	MyQueue<Token> q1;
public:
	MyExpression();
	bool inputExpression();
	void infixToPostfix();
	void outputExpression();
};

// 构造函数，实现输入表达式
MyExpression::MyExpression() 
{
	while (!inputExpression())
		std::cout << std::endl << ">>> 表达式输入的格式有误！" << std::endl << std::endl;
}

// 输出中缀表达式
void MyExpression::outputExpression()
{
	std::cout << std::endl << ">>> 中缀表达式为: ";
	std::cout << expression << std::endl;
}

// 输入表达式并对表达式的格式进行检查
bool MyExpression::inputExpression() 
{
	std::cout << ">>> 请输入一个算术表达式 (在每个运算数/运算符之间用空格隔开)" << std::endl << std::endl;
	std::cin.getline(expression, sizeof(expression));
	size_t len = std::strlen(expression);
	// 对表达式的开头和结尾进行检查
	if ((expression[0] == '*' || expression[0] == '/' || expression[0] == ')') ||
		(isOperator(expression[len - 1]) || expression[len - 1] == '(')) {
		return false;
	}
	// 检查整个算术表达式括号的平衡性
	int openParentheses = 0;
	for (size_t i = 0; i < len;) {
		char element = expression[i];
		if (element == ' ') {
			i++;
			continue;
		}
		size_t j = 1;
		while (expression[i + j] == ' ') {
			j++;
		}
		// 对于运算符
		if (isOperator(element)) {
			if (isOperator(expression[i + j]) || expression[i + j] == ')') {
				return false;
			}
		}
		// 对于数字
		else if (std::isdigit(element) || (element == '-' && std::isdigit(expression[i + 1]))) {
			// 标记是否已经遇到小数点
			bool hasDecimalPoint = false;  
			while (std::isdigit(expression[i + j]) || expression[i + j] == '.') {
				if (expression[i + j] == '.') {
					// 如果已经有一个小数点，非法
					if (hasDecimalPoint) {  
						return false;
					}
					hasDecimalPoint = true;
				}
				j++;
			}
			if (expression[i + j] == '(') {
				return false;
			}
		}
		// 对于前括号
		else if (element == '(') {
			openParentheses++;
			if (expression[i + j] == ')') {
				return false;
			}
		}
		// 对于后括号
		else if (element == ')') {
			openParentheses--;
			if (expression[i + j] == '(') {
				return false;
			}
		}
		else {
			return false;
		}
		i += j;
	}
	return openParentheses == 0;
}

// 中缀表达式转后缀
void MyExpression::infixToPostfix()
{
	// 将中缀表达式做成输入流
	std::istringstream stream(expression);
	char token[MAX_TOKEN];

	while (stream >> token) {
		Token t;
		/* 数字处理 */
		if (std::isdigit(token[0]) || 
			((token[0] == '-' || token[0] == '+') && std::isdigit(token[1]))) {
			std::strcpy(t.value, token);
			t.isNumber = true;
			q1.enqueue(t);
			continue;
		}
		/* 将运算符或括号拷贝到 Token */ 
		std::strcpy(t.value, token);
		t.isNumber = false;

		/* 运算符处理 */
		if (isOperator(t.value[0])) {
			while (!s1.isEmpty()) {
				Token top;
				s1.getTop(top);
				/* 比较优先级并处理栈顶元素 */ 
				if (!top.isNumber && precedence(top.value) >= precedence(t.value)) {
					Token temp;
					s1.Pop(temp);
					q1.enqueue(temp);
				}
				else {
					break;
				}
			}
			s1.Push(t);
		}
		/* 左括号处理 */  
		else if (std::strcmp(t.value, "(") == 0) {
			s1.Push(t);
		}
		/* 右括号处理 */
		else if (std::strcmp(t.value, ")") == 0) {
			while (!s1.isEmpty()) {
				Token top;
				s1.getTop(top);

				if (std::strcmp(top.value, "(") == 0) {
					s1.Pop(top); 
					break;
				}
				Token temp;
				s1.Pop(temp);
				q1.enqueue(temp);
			}
		}
		/* 未知符号处理 */
		else {
			std::cout << "Unrecognized token: " << token << std::endl;
		}
	}

	/* 将栈中剩余的元素全部弹出到队列 */ 
	while (!s1.isEmpty()) {
		Token t;
		s1.Pop(t);
		q1.enqueue(t);
	}

	std::cout << std::endl << ">>> 后缀表达式为: ";
	/* 输出后缀表达式 */ 
	while (!q1.isEmpty()) {
		Token t;
		q1.dequeue(t);
		std::cout << t.value;
		if (!q1.isEmpty()) {
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}

int main() 
{
	// 初始化算术表达式
	MyExpression infix;

	// 输出读入的中缀表达式
	infix.outputExpression();

	// 算术表达式中缀转后缀
	infix.infixToPostfix();

	// 等待回车键再退出
	std::cout << std::endl << "Press Enter to Quit" << std::endl;
	while (_getch() != '\r')
		continue;

	// 程序结束
	return 0;
}