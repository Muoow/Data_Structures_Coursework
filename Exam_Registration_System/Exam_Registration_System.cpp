/****************************************************************
 * Project Name:  Exam_Registration_System
 * File Name:     Exam_Registration_System.cpp
 * File Function: 家谱管理系统的实现
 * Author:        张翔
 * Update Date:   2024/12/8
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <conio.h>
#include <limits>
#include <cstring>
#include <new>

enum Operation { Cancel, Insert, Delete, Find, Update, Count, Rule };

const int MAX_ID_LENGTH = 12;
const int MAX_NAME_LENGTH = 24;
const int MAX_TYPE_LENGTH = 36;

template <typename Type>
struct MyLinkNode 
{
    Type data;
    MyLinkNode<Type>* next;
    MyLinkNode(MyLinkNode<Type>* ptr = nullptr) { next = ptr; }
    MyLinkNode(const Type& item, MyLinkNode<Type>* ptr = nullptr) { data = item; next = ptr; }
};

template <typename Type>
class MyList 
{
private:
    MyLinkNode<Type>* head;
    MyLinkNode<Type>* tail;
public:
    MyList();
    ~MyList();
    int getLength(void) const;
    MyLinkNode<Type>* getHead(void) const;
    MyLinkNode<Type>* getTail(void) const;
    MyLinkNode<Type>* search(Type item) const;
    MyLinkNode<Type>* locate(int i) const;
    bool getData(int i, Type& item) const;
    bool setData(int i, Type& item);
    bool insert(int i, Type& item);
    bool remove(int i, Type& item);
    bool isEmpty(void) const;
};

template <typename Type>
MyList<Type>::MyList() 
{
    head = new(std::nothrow) MyLinkNode<Type>;
    if (head == nullptr) {
        std::cerr << "Error: Memory allocation failed." << std::endl;
        exit(-1);
    }
    tail = head;
}

template <typename Type>
MyList<Type>::~MyList() 
{
    MyLinkNode<Type>* current = head;
    while (current != nullptr) {
        MyLinkNode<Type>* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
}

template <typename Type>
int MyList<Type>::getLength(void) const 
{
    int len = 0;
    MyLinkNode<Type>* current = head->next;
    while (current != nullptr) {
        current = current->next;
        len++;
    }
    return len;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::getHead(void) const 
{
    return head;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::getTail(void) const 
{
    return tail;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::search(Type item) const 
{
    MyLinkNode<Type>* current = head->next;
    while (current != nullptr) {
        if (current->data == item)
            break;
        else
            current = current->next;
    }
    return current;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::locate(int i) const 
{
    if (i < 0)
        return nullptr;
    MyLinkNode<Type>* current = head;
    int k = 0;
    while (current != nullptr && k < i) {
        current = current->next;
        k++;
    }
    return current;
}

template <typename Type>
bool MyList<Type>::getData(int i, Type& item) const
{
    if (i < 0)
        return false;
    MyLinkNode<Type>* current = locate(i);
    if (current == nullptr)
        return false;
    else {
        item = current->data;
        return true;
    }
}

template <typename Type>
bool MyList<Type>::setData(int i, Type& item)
{
    if (i < 0)
        return false;
    MyLinkNode<Type>* current = locate(i);
    if (current == nullptr)
        return false;
    else {
        current->data = item;
        return true;
    }
}

template <typename Type>
bool MyList<Type>::insert(int i, Type& item) 
{
    MyLinkNode<Type>* prev = locate(i - 1);
    if (prev == nullptr)
        return false;
    MyLinkNode<Type>* newNode = new(std::nothrow)MyLinkNode<Type>(item);
    if (newNode == nullptr) {
        std::cerr << "Error: Memory allocation failed." << std::endl;
        exit(-1);
    }
    newNode->next = prev->next;
    prev->next = newNode;
    if (tail == head)
        tail = newNode;
    return true;
}

template <typename Type>
bool MyList<Type>::remove(int i, Type& item) 
{
    MyLinkNode<Type>* prev = locate(i - 1);
    MyLinkNode<Type>* current = prev->next;
    if (prev == nullptr || current == nullptr)
        return false;
    prev->next = current->next;
    item = current->data;
    delete current;
    return true;
}

template <typename Type>
bool MyList<Type>::isEmpty(void) const
{
    return head->next == nullptr;
}

bool isNumberString(const char str[])
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt) 
{
    std::cout << ">>> 请输入" << prompt << " [范围: " << lowerLimit << "~" << upperLimit << "]:";
    int input;
    while (true) {
        std::cin >> input;
        if (std::cin.good() && input >= lowerLimit && input <= upperLimit) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            return input;
        }
        else {
            std::cout << std::endl << ">>> " << prompt << "输入不合法，请重新输入！" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

struct Student
{
    char examID[MAX_ID_LENGTH + 1];
    char name[MAX_NAME_LENGTH + 1];
    char examType[MAX_TYPE_LENGTH + 1];
    bool gender = true;
    int age = 0;
};

class Manager {
private:
    MyList<Student> student;
public:
    Manager(int stuNum);
    void buildStudentList(int stuNum);
    int findPosByStuNo(const std::string id);
    int GetPosByStuNo(const char* prompt);
    Student studentInput();
    void addStudent();
    void deleteStudent();
    void findStudent();
    void updateStudent();
    void statisticsFunction();
    void outputRules(); 
};

Manager::Manager(int stuNum)
{
    buildStudentList(stuNum);
}

Student Manager::studentInput() 
{
    Student newstudent;
    while (true) {
        std::cin >> newstudent.examID;
        if (!isNumberString(newstudent.examID)) {
            std::cout << std::endl << "学号输入不合法，请重新输入！" << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        else if (findPosByStuNo(newstudent.examID)) {
            std::cout << std::endl << "已存在相同的考试号，请重新输入！" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    while (true) {
        std::cin >> newstudent.name;
        break;
    }
    while (true) {
        std::string gender;
        std::cin >> gender;
        std::cout << gender;
        if (gender == "nan") {
            newstudent.gender = true;
            break;
        }
        else if (gender == "nv") {
            newstudent.gender = false;
            break;
        }
        else {
            std::cout << std::endl << "性别输入不合法，请重新输入！" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
    while (true) {
        std::cin >> newstudent.age;
        if (newstudent.age >= 1 && newstudent.age <= 99)
            break;
        else
            std::cout << std::endl << "年龄输入不合法，请重新输入！" << std::endl << std::endl;
    }
    while (true) {
        std::cin >> newstudent.examType;
        break;
    }
    return newstudent;
}

void inputPrompt(const std::string str)
{
    std::cout << std::endl << ">>> 请输入关于" << str << "的考试号、姓名及其他信息" << std::endl << std::endl;
}

void printHeader(std::string str)
{
    std::cout << ">>> " << str << std::endl;
    std::cout << "+------------------------------------------------------------------------------+" << std::endl;
    std::cout << "|      考试号      |          姓名          | 性别 | 年龄 |       考试类型     |" << std::endl;
    std::cout << "+------------------------------------------------------------------------------+" << std::endl;
}

void printStuinfo(const Student& stu) {
    std::cout << "| ";
    std::cout << std::setw(16) << std::left << stu.examID << " | "
        << std::setw(22) << std::left << stu.name << " | "
        << std::setw(5) << std::left << (stu.gender ? "男" : "女") << " | "
        << std::setw(4) << std::left << stu.age << " | "
        << std::setw(18) << std::left << stu.examType << " |";
    std::cout << std::endl;
}

void printFooter()
{
    std::cout << "+------------------------------------------------------------------------------+" << std::endl;
}

void Manager::buildStudentList(int stuNum)
{
    inputPrompt("全部考生");
    for (int count = 1; count <= stuNum; count++) {
        Student temp = studentInput();
        student.insert(count, temp);
    }
    std::cout << std::endl << ">>> 考生系统初始化完成 (考生总数: " << stuNum << " )" << std::endl;
    printHeader("全部考生的详细信息如下");
    for (int i = 1; i <= stuNum; i++) {
        Student temp;
        student.getData(i, temp);
        printStuinfo(temp);
    }
    printFooter();
}

int Manager::findPosByStuNo(const std::string id)
{
    MyLinkNode<Student>* current = student.getHead()->next;
    int position = 1;
    while (current != nullptr) {
        if (current->data.examID == id)
            return position;
        current = current->next;
        position++;
    }
    return 0;
}

int Manager::GetPosByStuNo(const char* prompt)
{
    char id[MAX_ID_LENGTH + 1];
    std::cout << "请输入" << prompt << "的考试号:";
    while (true) {
        std::cin.getline(id, MAX_ID_LENGTH + 1);
        if (!isNumberString(id)) {
            std::cout << std::endl << "学号输入不合法，请重新输入！" << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    return findPosByStuNo(id);
}

void Manager::addStudent() 
{
    int pos = inputInteger(1, student.getLength() + 1, "插入考生的位置");
    inputPrompt("新增考生");
    Student temp = studentInput();
    student.insert(pos, temp);
}

void Manager::deleteStudent() 
{
    if (student.getLength() == 0) {
        std::cout << std::endl << ">>> 考生信息表为空，无法执行删除操作!" << std::endl;
        return;
    }
    int pos = GetPosByStuNo("需删除的考生");
    if (pos == 0) {
        std::cout << std::endl << ">>> 未查询到该考生" << std::endl;
        return;
    }
    Student temp;
    student.remove(pos, temp);
}

void Manager::findStudent() 
{
    if (student.isEmpty()) {
        std::cout << std::endl << ">>> 考生信息表为空!" << std::endl;
        return;
    }
    int pos = GetPosByStuNo("需查询的考生");
    if (pos == 0) {
        std::cout << std::endl << ">>> 未查询到该考生" << std::endl;
        return;
    }
    Student temp;
    student.getData(pos, temp);
    printHeader("该考生的详细信息如下");
    printStuinfo(temp);
    printFooter();
}

void Manager::updateStudent()
{
    if (student.isEmpty()) {
        std::cout << std::endl << ">>> 考生信息表为空!" << std::endl;
        return;
    }
    int pos = GetPosByStuNo("需修改的考生");
    if (pos == 0) {
        std::cout << std::endl << ">>> 未查询到该考生" << std::endl;
        return;
    }
    inputPrompt("需修改的考生");
    Student temp = studentInput();
    student.setData(pos, temp);
}

void Manager::statisticsFunction()
{
    if (student.isEmpty()) {
        std::cout << std::endl << ">>> 考生信息表为空!" << std::endl;
        return;
    }
    printHeader("全部考生的详细信息如下");
    for (int i = 1; i <= student.getLength(); i++) {
        Student temp;
        student.getData(i, temp);
        printStuinfo(temp);
    }
    printFooter();
    std::cout << std::endl;
}

void Manager::outputRules()
{
    std::cout << std::endl;
    std::cout << "+----------------------------------------------------------------------------------+" << std::endl;
    std::cout << "|                           [信息输入的格式请注意以下规则]                         |" << std::endl;
    std::cout << "+----------------------------------------------------------------------------------+" << std::endl;
    std::cout << "| [格式说明] 考试号 姓名 性别 年龄 考试类型 (以空格分隔各项)                       |" << std::endl;
    std::cout << "| [考试号] 由 12 位以下字符组成，超长将自动截断                                    |" << std::endl;
    std::cout << "| [姓名] 由 24 位以内的字符组成，超长将自动截断                                    |" << std::endl;
    std::cout << "| [性别] 男 / 女                                                                   |" << std::endl;
    std::cout << "| [年龄] 在 1 到 99 范围内的整数                                                   |" << std::endl;
    std::cout << "| [考试类型] 由 32 位以内的字符组成，超长将自动截断                                |" << std::endl;
    std::cout << "+----------------------------------------------------------------------------------+" << std::endl;
    std::cout << std::endl;
}

Operation selectOperation() 
{
    std::cout << std::endl << ">>> 菜单: [1]新增 [2]删除 [3]查询 [4]修改 [5]统计 [6]帮助 [0]退出" << std::endl;
    std::cout << std::endl << "请选择要使用的功能: ";
    char optn;
    while (true) {
        optn = _getch();
        if (optn == 0 || optn == -32) {
            optn = _getch();
        }
        else if (optn >= '0' && optn <= '6') {
            std::cout << "[" << optn << "]" << std::endl << std::endl;
            return Operation(optn - '0');
        }
    }
}

int main() 
{
    /* 进入考试报名系统 */
    std::cout << "+----------------------------+" << std::endl;
    std::cout << "|        考生报名系统        |" << std::endl;
    std::cout << "|  Exam Registration System  |" << std::endl;
    std::cout << "+----------------------------+" << std::endl;
    std::cout << std::endl << ">>> 正在初始化考生信息系统" << std::endl;
    int studentNum = inputInteger(1, INT_MAX, "考生人数");
    Manager manager(studentNum);
    while (true) {
        int operation = int(selectOperation());
        if (operation == Insert)
            manager.addStudent();
        else if (operation == Delete)
            manager.deleteStudent();
        else if (operation == Find)
            manager.findStudent();
        else if (operation == Update)
            manager.updateStudent();
        else if (operation == Count)
            manager.statisticsFunction();
        else if (operation == Rule)
            manager.outputRules();
        else {
            std::cout << std::endl << ">>> 考生报名系统已退出" << std::endl;
            break;
        }
    }
    /* 程序退出 */
    return 0;
}