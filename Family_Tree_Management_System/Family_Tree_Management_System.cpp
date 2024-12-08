/****************************************************************
 * Project Name:  Family_Tree_Management_System
 * File Name:     Family_Tree_Management_System.cpp
 * File Function: 家谱管理系统的实现
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>
#include<conio.h>

template <typename Type>
struct MyTreeNode {
	Type data;
	MyTreeNode<Type>* left;
	MyTreeNode<Type>* right;
	MyTreeNode(const Type& value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename Type>
class MyBinaryTree {
private:
	MyTreeNode<Type>* root;
public:
	MyBinaryTree() : root(nullptr) {}
	MyBinaryTree(Type& item);
	~MyBinaryTree() { destroy(root); }
	void destroy(MyTreeNode<Type>* subTree);
	bool isEmpty(void) { return root == nullptr; }
	bool setLeftChild(MyTreeNode<Type>* parent, Type& item);
	bool setRightChild(MyTreeNode<Type>* parent, Type& item);
	bool modifyNode(MyTreeNode<Type>* current, Type& item);
	int getSize(MyTreeNode<Type>* current) { return (current == nullptr) ? 0 : (getSize(current->left) + getSize(current->right) + 1); }
	MyTreeNode<Type>* getRoot(void) { return root; }
	MyTreeNode<Type>* getParent(MyTreeNode<Type>* current, MyTreeNode<Type>* subTree);
	MyTreeNode<Type>* getLeftChild(MyTreeNode<Type>* current) { return current == nullptr ? nullptr : current->leftChild; }
	MyTreeNode<Type>* getRightChild(MyTreeNode<Type>* current) { return current == nullptr ? nullptr : current->rightChild; }
	MyTreeNode<Type>* findNode(const Type& item, MyTreeNode<Type>* subTree);
	void preorder(MyTreeNode<Type>* node);
	void inorder(MyTreeNode<Type>* node);
	void postorder(MyTreeNode<Type>* node);
	MyBinaryTree<Type>& operator=(const MyBinaryTree<Type>& other);
	MyTreeNode<Type>* copyTree(MyTreeNode<Type>* subTree);
};

template <typename Type>
MyBinaryTree<Type>::MyBinaryTree(Type& item) 
{
	root = new(std::nothrow)MyTreeNode<Type>(item);
	if (root == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
}

template <typename Type>
void MyBinaryTree<Type>::destroy(MyTreeNode<Type>* subTree)
{
	if (subTree) {
		destroy(subTree->left);
		destroy(subTree->right);
		delete subTree;
		subTree = nullptr;
	}
}

template <typename Type>
bool MyBinaryTree<Type>::setLeftChild(MyTreeNode<Type>* parent, Type& item) 
{
	if (parent == nullptr) {
		std::cerr << "Error: Parent node is null." << std::endl;
		return false;
	}
	parent->left = new MyTreeNode<Type>(item);
	if (parent->left == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	return true;
}

template <typename Type>
bool MyBinaryTree<Type>::setRightChild(MyTreeNode<Type>* parent, Type& item) 
{
	if (parent == nullptr) {
		std::cerr << "Error: Parent node is null." << std::endl;
		return false;
	}
	parent->right = new(std::nothrow) MyTreeNode<Type>(item);
	if (parent->right == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	return true;
}

template <typename Type>
bool MyBinaryTree<Type>::modifyNode(MyTreeNode<Type>* current, Type& item)
{
	if (current == nullptr)
		return false;
	current->data = item;
	return true;
}

template <typename Type>
MyTreeNode<Type>* MyBinaryTree<Type>::getParent(MyTreeNode<Type>* current, MyTreeNode<Type>* subTree) 
{
	if (current == nullptr || subTree == nullptr) {
		return nullptr;
	}
	if ( subTree->left == current || subTree->right == current) {
		return subTree;
	}
	MyTreeNode<Type>* parent = getParent(current, subTree->left);
	if (parent == nullptr) {
		parent = getParent(current, subTree->right);
	}
	return parent;
}

template <typename Type>
MyTreeNode<Type>* MyBinaryTree<Type>::findNode(const Type& item, MyTreeNode<Type>* subTree)
{
	if (subTree == nullptr) {
		return nullptr;
	}
	if (subTree->data == item) {
		return subTree;
	}
	MyTreeNode<Type>* found = findNode(item, subTree->left);
	if (found != nullptr) {
		return found;
	}
	return findNode(item, subTree->right);
}

template <typename Type>
void MyBinaryTree<Type>::preorder(MyTreeNode<Type>* node) 
{
	if (node) {
		std::cout << node->data;
		preorder(node->left);
		preorder(node->right);
	}
}

template <typename Type>
void MyBinaryTree<Type>::inorder(MyTreeNode<Type>* node)
{
	if (node) {
		inorder(node->left);
		std::cout << node->data;
		inorder(node->right);
	}
}

template <typename Type>
void MyBinaryTree<Type>::postorder(MyTreeNode<Type>* node) 
{
	if (node) {
		postorder(node->left);
		postorder(node->right);
		std::cout << node->data;
	}
}

template <typename Type>
MyBinaryTree<Type>& MyBinaryTree<Type>::operator=(const MyBinaryTree<Type>& other) {
	// 自我赋值检查
	if (this == &other) {
		return *this;
	}

	// 释放当前对象持有的资源
	destroy(root);  // 假设 destroy(root) 会释放树的内存

	// 进行深拷贝
	if (other.root == nullptr) {
		root = nullptr;
	}
	else {
		root = copyTree(other.root);  // 假设有一个方法来拷贝树
	}

	return *this;
}

// 用于深拷贝树的辅助方法
template <typename Type>
MyTreeNode<Type>* MyBinaryTree<Type>::copyTree(MyTreeNode<Type>* subTree) {
	if (subTree == nullptr) {
		return nullptr;
	}
	// 创建新节点并递归拷贝子树
	MyTreeNode<Type>* newNode = new(std::nothrow) MyTreeNode<Type>(subTree->data);
	if (newNode == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	newNode->left = copyTree(subTree->left);   // 递归拷贝左子树
	newNode->right = copyTree(subTree->right); // 递归拷贝右子树
	return newNode;
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

struct PersonInfo 
{
	char name[NameMaxLength + 1];
	PersonInfo() { strcpy(name, ""); }
	PersonInfo& operator=(const PersonInfo& other) { if (this != &other) strcpy(name, other.name); return *this; }
	bool operator==(const PersonInfo& other) { return strcmp(name, other.name) == 0; }
	friend std::istream& operator>>(std::istream& in, PersonInfo& info);
	friend std::ostream& operator<<(std::ostream& out, PersonInfo& info);
};

std::istream& operator>>(std::istream& in, PersonInfo& info)
{
	in.getline(info.name, NameMaxLength + 1);
	return in;
}

std::ostream& operator<<(std::ostream& out, PersonInfo& info)
{
	out << info.name;
	return out;
}

class family 
{
private:
	MyBinaryTree<PersonInfo> myfamily;
public:
	family(PersonInfo& ancestor);
	bool selectOptn();
	void completeFamilyMembers();
	void addFamilyMembers();
	void removeFamilyMembers();
	void changeFamilyMembers();
	void countFamilyMembers();
};

family::family(PersonInfo& ancestor)
{
	myfamily = MyBinaryTree<PersonInfo>(ancestor);
}

bool family::selectOptn() 
{
	char option;
	while (true) {
		option = _getch();
		if (option == 0 || option == -32) {
			option = _getch();
		}
		else if (option >= '1' && option <= '6') {
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
			countFamilyMembers();
		case '6':
			return false;
	}
	return true;
}

void family::completeFamilyMembers() 
{
	std::cout << "请输入要添加后代的长辈的姓名: ";
	PersonInfo parentInfo;
	std::cin >> parentInfo;
	std::cout << std::endl;
	MyTreeNode<PersonInfo>* root = myfamily.getRoot();
	MyTreeNode<PersonInfo>* parent = myfamily.findNode(parentInfo, root);
	if (parent == nullptr) {
		std::cout << ">>> 未在家谱中找到" << parentInfo << "，请确认姓名输入是否正确！" << std::endl;
		return;
	}
	if (parent->left != nullptr) {
		std::cout << ">>> " << parentInfo << "已建立家庭" << std::endl;
		return;
	}
	int num = inputInteger(1, 10, "该长辈的儿女数量");
	std::cout << std::endl << ">>> 请依次输入" << parentInfo << "的儿女的姓名" << std::endl << std::endl;
	PersonInfo* descendants = new(std::nothrow) PersonInfo[num];
	if (descendants == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < num; i++) {
		std::cout << "请输入" << parentInfo << "的第" << i + 1 << "个儿女的姓名: ";
		std::cin >> descendants[i];
		if (myfamily.findNode(descendants[i], myfamily.getRoot())) {
			std::cout << std::endl << ">>> " << descendants[i--] << "在家谱中已存在" << std::endl << std::endl;
			continue;
		}
		if (i == 0) {
			myfamily.setLeftChild(myfamily.findNode(parentInfo, myfamily.getRoot()), descendants[i]);
		}
		else {
			myfamily.setRightChild(myfamily.findNode(descendants[i - 1], myfamily.getRoot()), descendants[i]);
		}
	}
	std::cout << std::endl << ">>> " << parentInfo << "的下一代子孙是: ";
	for (int i = 0; i < num; i++)
		std::cout << descendants[i] << (i == num - 1 ? "" : " ");
	delete[]descendants;
	std::cout << std::endl;
}

void family::addFamilyMembers() 
{
	std::cout << "请输入要添加后代的长辈的姓名: ";
	PersonInfo parentInfo,childInfo;
	std::cin >> parentInfo;
	std::cout << std::endl;
	MyTreeNode<PersonInfo>* parent = myfamily.findNode(parentInfo, myfamily.getRoot());
	if (parent == nullptr) {
		std::cout << ">>> 未在家谱中找到" << parentInfo << "，请确认姓名输入是否正确！" << std::endl;
		return;
	}
	while (true) {
		std::cout << std::endl << "请输入要添加的后代的姓名: ";
		std::cin >> childInfo;
		if (myfamily.findNode(childInfo, myfamily.getRoot())) {
			std::cout << std::endl << ">>> " << childInfo << "在家谱中已存在" << std::endl;
			continue;
		}
		break;
	}
	std::cout << ">>> 添加成功 " << std::endl << std::endl;
	std::cout << ">>> 添加后" << parentInfo << "的下一代儿女是: ";
	if (parent->left == nullptr) {
		myfamily.setLeftChild(parent, childInfo);
	}
	else {
		MyTreeNode<PersonInfo>* current = parent->left;
		std::cout << current->data << " ";
		while (current->right != nullptr) {
			current = current->right;
			std::cout << current->data << " ";
		}
		myfamily.setRightChild(current, childInfo);
	}
	std::cout << childInfo << std::endl;
}

void family::removeFamilyMembers() 
{
	std::cout << "请输入要解散家庭成员的人的名字: ";
	PersonInfo targetInfo;
	std::cin >> targetInfo;
	std::cout << std::endl;
	MyTreeNode<PersonInfo>* target = myfamily.findNode(targetInfo, myfamily.getRoot());
	if (target == nullptr) {
		std::cout  << ">>> 未在家谱中找到" << targetInfo << "，请确认姓名输入是否正确！" << std::endl;
		return;
	}
	if (target->left == nullptr) {
		std::cout << ">>> " << targetInfo << "无家庭成员" << std::endl;
		return;
	}
	myfamily.destroy(target->left);
	std::cout << std::endl << std::endl << ">>> " << targetInfo << "的家庭成员已解散" << std::endl;
}

void family::changeFamilyMembers() 
{
	std::cout << "请输入需要修改名字的成员的更改前姓名: ";
	PersonInfo targetInfo,infoAfterChange;
	std::cin >> targetInfo;
	std::cout << std::endl;
	MyTreeNode<PersonInfo>* target = myfamily.findNode(targetInfo, myfamily.getRoot());
	if (target == nullptr) {
		std::cout << ">>> 未在家谱中找到" << targetInfo << "，请确认姓名输入是否正确！" << std::endl;
		return;
	}
	while (true) {
		std::cout << std::endl << "请输入要更改姓名的人的更改后姓名: ";
		std::cin >> infoAfterChange;
		if (myfamily.findNode(infoAfterChange, myfamily.getRoot())) {
			std::cout << std::endl << ">>> " << infoAfterChange << "在家谱中已存在" << std::endl;
			continue;
		}
		break;
	}
	myfamily.modifyNode(target, infoAfterChange);
	std::cout << std::endl << ">>> " << targetInfo << "已更名为" << infoAfterChange << std::endl;
}

void family::countFamilyMembers() 
{
	std::cout << "请输入要统计的家庭成员的人的姓名: ";
	PersonInfo targetInfo;
	std::cin >> targetInfo;
	std::cout << std::endl;
	MyTreeNode<PersonInfo>* target = myfamily.findNode(targetInfo, myfamily.getRoot());
	if (target == nullptr) {
		std::cout << ">>> 未在家谱中找到" << targetInfo << "，请确认姓名输入是否正确！" << std::endl;
		return;
	}
	int num = myfamily.getSize(target->left);
	std::cout << std::endl << ">>> " << targetInfo << "共有" << num << "个儿女";
	if (num != 0) {
		std::cout << "，分别是: ";
		myfamily.inorder(target->left);
	}
	std::cout << std::endl;
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
	std::cout << "|       [5] --- 统计家庭成员      |" << std::endl;
	std::cout << "|       [6] --- 退出程序          |" << std::endl;
	std::cout << "+---------------------------------+" << std::endl << std::endl;
	/* 系统开始提示 */
	std::cout << ">>> 请建立家谱管理系统" << std::endl << std::endl; 
	std::cout << "请输入祖先姓名: ";
	PersonInfo ancestor;
	std::cin >> ancestor;
	family myFamily(ancestor);
	std::cout << std::endl << ">>> 家谱已建立，祖先为: " << ancestor.name << std::endl;
	/* 循环处理用户输入 */
	do {
		std::cout << std::endl << ">>> 请选择要使用的功能: ";
	} while (myFamily.selectOptn());
	/* 系统退出提示 */
	std::cout << ">>> 已成功退出家谱管理系统"  << std::endl;
	return 0;
}
