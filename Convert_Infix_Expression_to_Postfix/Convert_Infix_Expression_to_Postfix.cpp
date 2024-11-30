#include<iostream>
#include<sstream>
#include<new>
#include<windows.h>

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
bool MyQueue<Type>::isEmpty() const {
	return front == nullptr;
}

template <typename Type>
void MyQueue<Type>::makeEmpty() {
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
int MyQueue<Type>::Size() const {
	return count;
}

template <typename Type>
void MyQueue<Type>::enqueue(const Type& item) {
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
	count--;
	return true;
}

template <typename Type>
bool MyQueue<Type>::getHead(Type& item) {
	if (isEmpty()) {
		return false;
	}
	item = front->data;
	return true;
}

template <typename Type>
class MyStack {
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
bool MyStack<Type>::isEmpty() const {
	return topNode == nullptr;
}

template <typename Type>
void MyStack<Type>::makeEmpty() {
	MyLinkNode<Type>* current;
	while (topNode != nullptr) {
		current = topNode;
		topNode = topNode->link;
		delete current;
	}
	count = 0;
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
	topNode = new(std::nothrow) MyLinkNode<Type>(item, topNode);
	if (topNode == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
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

template <typename Type>
struct TreeNode {
	Type data;
	TreeNode<Type>* left;
	TreeNode<Type>* right;
	TreeNode(const Type& value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename Type>
class BinaryTree {
private:
	TreeNode<Type>* root;
	void preorder(TreeNode<Type>* node);
	void inorder(TreeNode<Type>* node);
	void postorder(TreeNode<Type>* node);
public:
	BinaryTree() : root(nullptr) {}
	void preorderTraversal() {
		preorder(root);
		std::cout << std::endl;
	}
	void inorderTraversal() {
		inorder(root);
		std::cout << std::endl;
	}
	void postorderTraversal() {
		postorder(root);
		std::cout << std::endl;
	}
};

template <typename Type>
void BinaryTree<Type>::preorder(TreeNode<Type>* node) {
	if (node) {
		std::cout << node->data; 
		preorder(node->left);
		preorder(node->right);
	}
}

template <typename Type>
void BinaryTree<Type>::inorder(TreeNode<Type>* node) {
	if (node) {
		inorder(node->left);
		std::cout << node->data;
		inorder(node->right);
	}
}

template <typename Type>
void BinaryTree<Type>::postorder(TreeNode<Type>* node) {
	if (node) {
		postorder(node->left);
		postorder(node->right);
		std::cout << node->data;
	}
}

bool isOperator(const char& c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

bool isNum(const char& n) {
	return n >= '0' && n <= '9';
}

int precedence(const std::string& op) {
	if (op == "+" || op == "-") {
		return 1;
	}
	if (op == "*" || op == "/") {
		return 2;
	}
	return 0;
}

int applyOp(int a, int b, char op) {
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

#include <string>

struct Element {
	double num;
	std::string op;
	bool isNumber; 
	Element() : num(0), op(""), isNumber(true) {}
	Element(double n) : num(n), op(""), isNumber(true) {}
	Element(const std::string& o) : num(0), op(o), isNumber(false) {}
};

class MyExpression {
private:
	std::string expression;
	MyStack<Element> s1;
	MyQueue<Element> q1;
public:
	MyExpression();
	bool inputExpression();
	void infixToPostfix();
};

MyExpression::MyExpression() {
	while (!inputExpression())
		std::cout << "The expression input format is incorrect." << std::endl;
}

bool MyExpression::inputExpression() {
	std::cout << "Please enter an arithmetic expression.(Separate expression elements with spaces)" << std::endl;
	getline(std::cin, expression);
	if ((isOperator(expression[0]) || expression[0] == ')') ||
		(isOperator(expression.back()) || expression.back() == '(')) {
		return false;
	}
	int openParentheses = 0;
	//for (int i = 0; i < expression.size();) {
	//	char element = expression[i];
	//	if (element == ' ') {
	//		i++;
	//		continue;
	//	}
	//	int j = 1;
	//	while (expression[i + j] == ' ') {
	//		j++;
	//	}
	//	if (isOperator(element)) {
	//		if (isOperator(expression[i + j]) ||
	//			expression[i + j] == ')') {
	//			return false;
	//		}
	//	}
	//	else if (isNum(element)) {
	//		if (expression[i + j] == '(') {
	//			return false;
	//		}
	//	}
	//	else if (element == '(') {
	//		openParentheses++;
	//		if (expression[i + j] == ')')
	//			return false;
	//	}
	//	else if (element == ')') {
	//		openParentheses--;
	//		if (expression[i + j] == '(')
	//			return false;
	//	}
	//	else {
	//		return false;
	//	}
	//	i += j;
	//}
	if (openParentheses != 0) {
		return false;
	}
	return true;
}

void MyExpression::infixToPostfix() {
	std::istringstream iss(expression);
	std::string token;
	while (iss >> token) { 

		if (token == "+" || token == "-" || token == "*" || token == "/") {
			Element element(token);
			if (!s1.isEmpty()) {
				Element stacktop;
				s1.getTop(stacktop);
				while (!s1.isEmpty() && precedence(stacktop.op) >= precedence(token)) {
					Element temp;
					s1.Pop(temp);
					q1.enqueue(temp);
					if (!s1.isEmpty())
						s1.getTop(stacktop);
				}
			}
			s1.Push(element);
		}
		else if (token == "(") {
			Element element(token);
			s1.Push(element);
		}
		else if (token == ")") {
			if (!s1.isEmpty()) {
				Element stacktop;
				s1.getTop(stacktop);
				while (stacktop.op != "(") {
					Element temp;
					s1.Pop(temp);
					q1.enqueue(temp);
					if (!s1.isEmpty())
						s1.getTop(stacktop);
				}
				s1.Pop(stacktop);
			}
		}
		else {
			try {
				double number = std::stod(token);
				Element element(number);
				q1.enqueue(element);
			}
			catch (const std::invalid_argument&) {
				std::cout << "Unrecognized token:" << token << std::endl;
			}
		}
	}
	while (!s1.isEmpty()) {
        Element element;
        s1.Pop(element);
        q1.enqueue(element);
    }
	while (!q1.isEmpty()) {
		Element element;
		q1.dequeue(element);
		if (!element.isNumber)
			std::cout << element.op;
		else
			std::cout << element.num;
		if (!q1.isEmpty())
			std::cout << " ";
	}
}

int main() {
	MyExpression infix;
	infix.infixToPostfix();
	return 0;
}