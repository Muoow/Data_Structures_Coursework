#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<new>
#include<sstream>

const int MAX_LENGTH = 256;
const int MAX_TOKEN = 10;

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

bool isOperator(const char& c) 
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

bool isNum(const char& n) 
{
	return n >= '0' && n <= '9';
}

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

struct Token 
{
	char value[MAX_TOKEN];
	bool isNumber;
};

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
};

MyExpression::MyExpression() 
{
	while (!inputExpression())
		std::cout << "表达式输入的格式有误！" << std::endl;
}

bool MyExpression::inputExpression() 
{
	std::cout << "请输入一个算术表达式 (在每个运算数/运算符之间用空格隔开)" << std::endl;
	std::cin.getline(expression, sizeof(expression));
	size_t len = std::strlen(expression);
	if ((isOperator(expression[0]) || expression[0] == ')') ||
		(isOperator(expression[len - 1]) || expression[len - 1] == '(')) {
		return false;
	}
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
		if (isOperator(element)) {
			if (isOperator(expression[i + j]) || expression[i + j] == ')') {
				return false;
			}
		}
		else if (std::isdigit(element)) {
			if (expression[i + j] == '(') {
				return false;
			}
		}
		else if (element == '(') {
			openParentheses++;
			if (expression[i + j] == ')') {
				return false;
			}
		}
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

void MyExpression::infixToPostfix()
{
	std::istringstream stream(expression);
	char token[MAX_TOKEN];

	while (stream >> token) {
		Token t;
		/* 数字处理 */
		if (std::isdigit(token[0]) || (token[0] == '-' && std::isdigit(token[1]))) {
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

	MyExpression infix;
	infix.infixToPostfix();
	return 0;
}