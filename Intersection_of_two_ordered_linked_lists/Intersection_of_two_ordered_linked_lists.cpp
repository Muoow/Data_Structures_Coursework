/****************************************************************
 * Project Name:  Intersection_of_two_ordered_linked_lists
 * File Name:     Intersection_of_two_ordered_linked_lists.cpp
 * File Function: 两个有序链表序列的交集
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#include <iostream>
#include <new>

/* Define MyLinkNode structure */
template <typename Type>
struct MyLinkNode
{
    Type data;
    MyLinkNode<Type>* link;
    MyLinkNode(MyLinkNode<Type>* ptr = NULL) { link = ptr; }
    MyLinkNode(const Type& item, MyLinkNode<Type>* ptr = NULL) { data = item; link = ptr; }
};

template <typename Type>
class MyList
{
private:
    MyLinkNode<Type>* head;
public:
    MyList() : head(nullptr) {}
    ~MyList();
    void createList(Type& end);
    void createList(MyLinkNode<Type>* _head);
    void printList();
    MyLinkNode<Type>* getHead();
};

template <typename Type>
MyList<Type>::~MyList()
{
    MyLinkNode<Type>* current = head;
    while (current != nullptr) {
        MyLinkNode<Type>* next = current->link;
        delete current;
        current = next;
    }
    head = nullptr;
}

template <typename Type>
void MyList<Type>::createList(Type& end)
{
    Type num = end;
    MyLinkNode<Type>* current = head;
    while (std::cin >> num) {
        if (num == end) {
            std::cin.ignore(INT_MAX, '\n');
            break;
        }
        MyLinkNode<Type>* newNode = new(std::nothrow) MyLinkNode<Type>(num);
        if (newNode != nullptr) {
            if (head == nullptr) {
                head = newNode;
                current = head;
            }
            else {
                current->link = newNode;
                current = newNode;
            }
        }
        else {
            std::cerr << "Memory allocation failed for MyLinkNode." << std::endl;
            break;
        }
    }
}

template <typename Type>
void MyList<Type>::printList() 
{
    if (!head) {
        std::cout << "NULL";
    }
    else {
        MyLinkNode<Type>* curr = head;
        while (curr) {
            std::cout << curr->data;
            if (curr->link) {
                std::cout << " ";
            }
            curr = curr->link;
        }
    }
    std::cout << std::endl;
}

template <typename Type>
void MyList<Type>::createList(MyLinkNode<Type>* _head)
{
    head = _head;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::getHead()
{
    return head;
}

template <typename Type>
MyLinkNode<Type>* findIntersection(MyLinkNode<Type>* l1, MyLinkNode<Type>* l2)
{
    MyLinkNode<Type>* p1 = l1;
    MyLinkNode<Type>* p2 = l2;
    MyLinkNode<Type>* dummy = nullptr;
    MyLinkNode<Type>* result = nullptr;
    while (p1 && p2) {
        if (p1->data == p2->data) {
            MyLinkNode<Type>* newNode = new(std::nothrow) MyLinkNode<Type>(p1->data);
            if (newNode != nullptr) {
                if (result == nullptr) {
                    result = newNode;
                    dummy = result;
                }
                else {
                    dummy->link = newNode;
                    dummy = newNode;
                }
                p1 = p1->link;
                p2 = p2->link;
            }
            else {
                std::cerr << "Memory allocation failed for MyLinkNode." << std::endl;
            }
        }
        else if (p1->data < p2->data) {
            p1 = p1->link;
        }
        else if (p1->data > p2->data) {
            p2 = p2->link;
        }
    }
    return result;
}

int main() 
{
    // 初始化
    MyList<int> list1, list2, list3;
    int end = -1;

    // 输入提示和输入要求
    std::cout << ">>> 请输入第一个链表的值(用 -1 代表链表的结束): ";
    list1.createList(end);
    std::cout << std::endl;
    std::cout << ">>> 请输入第二个链表的值: " ;
    list2.createList(end);
    std::cout << std::endl;

    // 结合链表
    MyLinkNode<int>* head3 = findIntersection(list1.getHead(), list2.getHead());
    list3.createList(head3);

    // 输出结合之前的两个链表和结合之后的链表
    std::cout << ">>> 第一个链表的值为: ";
    list1.printList();
    std::cout << std::endl;
    std::cout << ">>> 第二个链表的值为: ";
    list2.printList();
    std::cout << std::endl;
    std::cout << ">>> 这两个链表的交集为: ";
    list3.printList();
    std::cout << std::endl;
}
