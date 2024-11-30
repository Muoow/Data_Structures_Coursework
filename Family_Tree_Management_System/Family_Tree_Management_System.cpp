#include<iostream>
#include<cstring>
#include <conio.h>

template <typename Type>
class MyTreeNode 
{
public:
	Type data;
	ChildNode* children;
	MyTreeNode(const Type& data) : data(data), parent(nullptr), children(nullptr) {}
	~MyTreeNode();
};

template <typename Type>
struct ChildNode 
{
	MyTreeNode* child;
	ChildNode* next;
	ChildNode(MyTreeNode* c) : child(c), next(nullptr) {}
};


template <typename Type>
MyTreeNode<Type>::~MyTreeNode() 
{
	ChildNode* current = children;
	while (current) {
		ChildNode* next = current->next;
		delete current;
		current = next;
	}
}

template <typename Type>
class MyTree 
{
private:

	MyTreeNode<Type>* root;

public:

	MyTree() :root(nullptr) {}
	MyTree(Type& item) { addRoot(item); }
	~MyTree() { deleteSubtree(root); }

	bool isEmpty() { return root == nullptr; }
	void deleteSubtree(MyTreeNode<Type>* node);
	MyTreeNode<Type>* findNode(MyTreeNode<Type>* node, const Type& value);
	MyTreeNode<Type>* getRoot(void) { return root; }
	MyTreeNode<Type>* getParent(MyTreeNode<Type>* current, MyTreeNode<Type>* node);
	ChildNode<Type>* getChild(MyTreeNode<Type>* node);
	bool addRoot(const Type& rootdata);
	bool addChild(const Type& parentData, const Type& childData); 
	bool modifyNode(const Type& oldData, const Type& newData);

};

template <typename Type>
void MyTree<Type>::deleteSubtree(MyTreeNode<Type>* node)
{
	if (node) {
		ChildNode<Type>* current = node->children;
		while (current) {
			deleteSubtree(current->child);
			current = current->next;
		}
		delete node;
	}

}

template <typename Type>
MyTreeNode<Type>* MyTree<Type>::findNode(MyTreeNode<Type>* node, const Type& value) 
{
	if (!node) 
		return nullptr;

	if (node->data == value) 
		return node;

	ChildNode<Type>* current = node->children;

	while (current) {
		MyTreeNode<Type>* result = findNode(current->child, value);
		if (result) 
			return result;
		current = current->next;
	}

	return nullptr;

}

template <typename Type>
MyTreeNode<Type>* MyTree<Type>::getParent(MyTreeNode<Type>* current, MyTreeNode<Type>* node) 
{
	if (!current || !node)
		return nullptr;

	ChildNode<Type>* children = current->children;

	while (children) {
		if (children->child == node) {
			return current;
		}
		MyTreeNode<Type>* result = getParent(children->child, node);
		if (result) return result;
		children = children->next;
	}

	return nullptr;
}

template <typename Type>
ChildNode<Type>* MyTree<Type>::getChild(MyTreeNode<Type>* node) 
{
	if (!node) 
		return nullptr;

	ChildNode<Type>* child = node->children;

	if (child) {
		return child;
	}

	return nullptr;

}

template <typename Type>
bool MyTree<Type>::addRoot(const Type& rootdata) 
{
	if (root) {
		return false;
	}

	root = new (std::nothrow)MyTreeNode<Type>(rootdata);

	if (root == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}

	return true;

}

template <typename Type>
bool MyTree<Type>::addChild(const Type& parentData, const Type& childData) 
{
	MyTreeNode<Type>* parentNode = findNode(root, parentData);
	if (!parentNode) 
		return false;

	MyTreeNode<Type>* childNode = new(std::nothrow) MyTreeNode<Type>(childData);
	if (childNode == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}

	ChildNode<Type>* newChild = new(std::nothrow) ChildNode<Type>*(childNode);
	if (newChild == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}

	if (!parentNode->children) {
		parentNode->children = newChild;
	}
	else {
		ChildNode<Type>* current = parentNode->children;
		while (current->next) {
			current = current->next;
		}
		current->next = newChild;
	}

	return true;

}

template <typename Type>
bool MyTree<Type>::modifyNode(const Type& oldData, const Type& newData) 
{
	TreeNode<Type>* node = findNode(root, oldData);
	if (!node)
		return false;

	node->data = newData;
	return true;

}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt)
{
	std::cout << ">>> " << "请输入" << prompt << " 整数范围: [" << lowerLimit << "~" << upperLimit << "]: ";
	int input;
	while (true) {
		std::cin >> input;
		if (std::cin.good() && input >= lowerLimit && input <= upperLimit) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cout << std::endl;
			return input;
		}
		else {
			std::cerr << ">>> " << prompt << "输入不合法，请重新输入！" << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

const int NameMaxLength = 32;

struct familyInfo 
{
	char name[NameMaxLength + 1];
	familyInfo() { strcpy(name, ""); }
	familyInfo& operator=(const familyInfo& other) { if (this != &other) strcpy(name, other.name); return *this; }
	bool operator==(const familyInfo& other) { return strcmp(name, other.name) == 0; }
	friend std::istream& operator>>(std::istream& in, familyInfo& info);
};

std::istream& operator>>(std::istream& in, familyInfo& info) 
{
	in.getline(info.name, NameMaxLength + 1);
	return in;
}

class family 
{
private:
	MyTree<familyInfo> myfamily;
public:
	family(familyInfo& ancestor);
	void rule();
	bool selectOptn();
	void completeFamilyMembers();
	void addFamilyMembers();
	void removeFamilyMembers();
	void changeFamilyMembers();
	void countFamilyMembers();
};

family::family(familyInfo& ancestor) 
{
	myfamily.addRoot(ancestor);
}

bool family::selectOptn() 
{
	std::cout << ">>> 请选择要使用的功能: ";
	char option;
	while (true) {
		option = _getch();
		if (option == 0 || option == -32) {
			option = _getch();
		}
		else if (option >= '1' || option <= '5') {
			std::cout << "[" << option << "]" << std::endl << std::endl;
			break;
		}
	}
	switch (option) {
		case '1': 
			completeFamilyMembers(); 
			break;
		case '2': 
			addFamilyMembers(); 
			break;
		case '3': 
			removeFamilyMembers(); 
			break;
		case '4': 
			changeFamilyMembers(); 
			break;
		case '5': 
			return false;
	}
	return true;

}

void family::rule() 
{
	std::cout << ">>> " << std::endl;
}

void family::completeFamilyMembers() 
{

}

void family::addFamilyMembers() 
{

}

void family::removeFamilyMembers() 
{

}

void family::changeFamilyMembers() 
{

}

void family::countFamilyMembers() 
{

}

int main() 
{
	std::cout << "+---------------------------------+" << std::endl;
	std::cout << "|           家谱管理系统          |" << std::endl;
	std::cout << "|  Family_Tree_Management_System  |" << std::endl;
	std::cout << "+---------------------------------+" << std::endl;
	std::cout << "|       [1] --- 完善家谱          |" << std::endl;
	std::cout << "|       [2] --- 添加家庭成员      |" << std::endl;
	std::cout << "|       [3] --- 解散局部家庭      |" << std::endl;
	std::cout << "|       [4] --- 更改成员姓名      |" << std::endl;
	std::cout << "|       [5] --- 退出程序          |" << std::endl;
	std::cout << "+---------------------------------+" << std::endl << std::endl;

	std::cout << ">>> 请建立家谱管理系统" << std::endl << std::endl; 
	std::cout << "请输入祖先姓名" << std::endl;
	familyInfo ancestor;
	std::cin >> ancestor;
	family myFamily(ancestor);
	while (myFamily.selectOptn());

	std::cout << ">>> 已成功退出家谱管理系统"  << std::endl;
	return 0;
}
