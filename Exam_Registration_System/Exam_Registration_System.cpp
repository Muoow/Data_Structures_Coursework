/****************************************************************
 * Project Name:  Exam_Registration_System
 * File Name:     Exam_Registration_System.cpp
 * File Function: ���Ա���ϵͳ��ʵ��
 * Author:        ����
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <conio.h>
#include <new>
using namespace std;

enum Operation { Cancel, Insert, Delete, Find, Update, Count, Rule };

struct Student {
    string examID = "";
    string name = "";
    string gender = "";
    string examType = "";
    int age = 0;
};

template <typename Type>
struct MyLinkNode {
    Type data;
    MyLinkNode<Type>* next;
    MyLinkNode(MyLinkNode<Type>* ptr = nullptr) { next = ptr; }
    MyLinkNode(const Type& item, MyLinkNode<Type>* ptr = nullptr) { data = item; next = ptr; }
};

template <typename Type>
class MyList {
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
MyList<Type>::MyList() {
    head = new(nothrow) MyLinkNode<Type>;
    if (head == nullptr) {
        cerr << "Error: Memory allocation failed." << endl;
        exit(-1);
    }
    tail = head;
}

template <typename Type>
MyList<Type>::~MyList() {
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
int MyList<Type>::getLength(void) const {
    int len = 0;
    MyLinkNode<Type>* current = head->next;
    while (current != nullptr) {
        current = current->next;
        len++;
    }
    return len;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::getHead(void) const {
    return head;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::getTail(void) const {
    return tail;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::search(Type item) const {
    Student* current = head->next;
    while (current != nullptr) {
        if (current->data == item)
            break;
        else
            current = current->next;
    }
    return current;
}

template <typename Type>
MyLinkNode<Type>* MyList<Type>::locate(int i) const {
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
bool MyList<Type>::getData(int i, Type& item) const {
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
bool MyList<Type>::setData(int i, Type& item) {
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
bool MyList<Type>::insert(int i, Type& item) {
    MyLinkNode<Type>* prev = locate(i - 1);
    if (prev == nullptr)
        return false;
    MyLinkNode<Type>* newNode = new(nothrow)MyLinkNode<Type>(item);
    if (newNode == nullptr) {
        cerr << "Error: Memory allocation failed." << endl;
        exit(-1);
    }
    newNode->next = prev->next;
    prev->next = newNode;
    if (tail == head)
        tail = newNode;
    return true;
}

template <typename Type>
bool MyList<Type>::remove(int i, Type& item) {
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
bool MyList<Type>::isEmpty(void) const {
    return head->next == nullptr;
}

bool isNumberString(const string str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9')
            i++;
        else
            return false;
    }
    return true;
}

string subString(string& str, int len) {
    if (str.length() > len) {
        cout << endl << "����" << len << "���ַ������Զ��ض�" << endl;
        str = str.substr(0, len);
    }
    return str;
}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt) {
    cout << "������" << prompt << " [������Χ: " << lowerLimit << "~" << upperLimit << "]:";
    double input;
    while (true) {
        cin >> input;
        if (cin.good() && input == static_cast<int>(input) && input >= lowerLimit && input <= upperLimit) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            return static_cast<int>(input);
        }
        else {
            cerr << endl << ">>> " << prompt << "���벻�Ϸ������������룡" << endl << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

class Manager {
private:
    MyList<Student> student;
public:
    Manager(int stuNum);
    void buildStudentList(int stuNum);
    int findPosByStuNo(const string id);
    int GetPosByStuNo(const char* prompt);
    Student studentInput();
    void addStudent();
    void deleteStudent();
    void findStudent();
    void updateStudent();
    void statisticsFunction();
    void outputRules(); 
};

Manager::Manager(int stuNum) {
    buildStudentList(stuNum);
}

Student Manager::studentInput() {
    Student newstudent;
    while (true) {
        cin >> newstudent.examID;
        newstudent.examID = subString(newstudent.examID, 12);
        if (!isNumberString(newstudent.examID)) {
            cout << endl << "ѧ�����벻�Ϸ������������룡" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        else if (findPosByStuNo(newstudent.examID)) {
            cout << endl << "�Ѵ��ڿ�����ͬ�Ŀ��������������룡" << endl << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    while (true) {
        cin >> newstudent.name;
        newstudent.name = subString(newstudent.name, 24);
        break;
    }
    while (true) {
        cin >> newstudent.gender;
        if (newstudent.gender == "��" || newstudent.gender == "Ů")
            break;
        else
            cout << endl << "�Ա����벻�Ϸ������������룡" << endl << endl;
    }
    while (true) {
        cin >> newstudent.age;
        if (newstudent.age >= 1 && newstudent.age <= 99)
            break;
        else
            cout << endl << "�������벻�Ϸ������������룡" << endl << endl;
    }
    while (true) {
        cin >> newstudent.examType;
        newstudent.examType = subString(newstudent.examType, 36);
        break;
    }
    return newstudent;
}

void inputPrompt(const string str) {
    cout << endl << ">>> ����������" << str << "�Ŀ��ţ��������Ա����估�������" << endl << endl;
}

void printHeader(string str) {
    cout << ">>> " << str << endl;
    cout << "+------------------------------------------------------------------------------+" << endl;
    cout << "|      ����      |          ����          | �Ա� | ���� |       �������       |" << endl;
    cout << "+------------------------------------------------------------------------------+" << endl;
}

void printStuinfo(Student stu) {
    cout << "| ";
    cout << setw(14) << left << stu.examID << " | "
        << setw(22) << left << stu.name << " | "
        << setw(4) << left << stu.gender << " | "
        << setw(4) << left << stu.age << " | "
        << setw(20) << left << stu.examType << " |";
    cout << endl;
}

void printFooter() {
    cout << "+------------------------------------------------------------------------------+" << endl;
}
    

void Manager::buildStudentList(int stuNum) {
    inputPrompt("ȫ�忼��");
    for (int count = 1; count <= stuNum; count++) {
        Student temp = studentInput();
        student.insert(count, temp);
    }
    cout << endl << ">>> ����ϵͳ������� (��������: " << stuNum << " )" << endl;
    printHeader("ȫ�忼������Ϣ����");
    for (int i = 1; i <= stuNum; i++) {
        Student temp;
        student.getData(i, temp);
        printStuinfo(temp);
    }
    printFooter();
}

int Manager::findPosByStuNo(const string id) {
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

int Manager::GetPosByStuNo(const char* prompt) {
    string id;
    cout << "������" << prompt << "�Ŀ���:";
    while (true) {
        cin >> id;
        id = subString(id, 12);
        if (!isNumberString(id)) {
            cout << endl << "ѧ�����벻�Ϸ������������룡" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n'); 
            continue;
        }
        break;
    }
    return findPosByStuNo(id);
}

void Manager::addStudent() {
    int pos = inputInteger(1, student.getLength() + 1, "����ӿ�����λ��");
    inputPrompt("����ӿ���");
    Student temp = studentInput();
    student.insert(pos, temp);
}

void Manager::deleteStudent() {
    if (student.getLength() == 0) {
        cout << endl << ">>> ������Ϣ�б�Ϊ�գ��޷���������!" << endl;
        return;
    }
    int pos = GetPosByStuNo("��ɾ���Ŀ���");
    if (pos == 0) {
        cout << endl << ">>> δ��ѯ���ÿ���" << endl;
        return;
    }
    Student temp;
    student.remove(pos, temp);
}

void Manager::findStudent() {
    if (student.isEmpty()) {
        cout << endl << ">>> ������Ϣ�б�Ϊ��!" << endl;
        return;
    }
    int pos = GetPosByStuNo("����ѯ�Ŀ���");
    if (pos == 0) {
        cout << endl << ">>> δ��ѯ���ÿ���" << endl;
        return;
    }
    Student temp;
    student.getData(pos, temp);
    printHeader("�ÿ�������Ϣ����");
    printStuinfo(temp);
    printFooter();
}

void Manager::updateStudent() {
    if (student.isEmpty()) {
        cout << endl << ">>> ������Ϣ�б�Ϊ��!" << endl;
        return;
    }
    int pos = GetPosByStuNo("���޸ĵĿ���");
    if (pos == 0) {
        cout << endl << ">>> δ��ѯ���ÿ���" << endl;
        return;
    }
    inputPrompt("���޸Ŀ���");
    Student temp = studentInput();
    student.setData(pos, temp);
}

void Manager::statisticsFunction() {
    if (student.isEmpty()) {
        cout << endl << ">>> ������Ϣ�б�Ϊ��!" << endl;
        return;
    }
    printHeader("ȫ�忼������Ϣ����");
    for (int i = 1; i <= student.getLength(); i++) {
        Student temp;
        student.getData(i, temp);
        printStuinfo(temp);
    }
    printFooter();
    cout << endl;

}

void Manager::outputRules() {
    cout << endl;
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << "| [����Ϣϵͳ�ɽ��еĲ���] [1]���� [2]ɾ�� [3]���� [4]�޸� [5]ͳ�� [6]���� [0]�˳� |" << endl;
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << endl;
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << "|                            [��Ϣ����ʱ��ʽ��ע������]                            |" << endl;
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << "| [�����ʽ] ���� ���� �Ա� ���� ������� (�ÿո�ָ�����)                         |" << endl;
    cout << "| [��    ��] ������ 12 �������ַ���ɵ��ַ������������ֽ����ض�                    |" << endl;
    cout << "| [��    ��] ������ 24 ��Ӣ���ַ��� 12 �������ַ���ɵ��ַ������������ֽ����ض�    |" << endl;
    cout << "| [��    ��] �� / Ů                                                               |" << endl;
    cout << "| [��    ��] �� 1 �� 99 ��Χ�ڵ���������                                           |" << endl;
    cout << "| [�������] ������ 32 ��Ӣ���ַ��� 16 �������ַ���ɵ��ַ������������ֽ����ض�    |" << endl;
    cout << "+----------------------------------------------------------------------------------+" << endl;
    cout << endl;
}

Operation selectOperation() {
    cout << endl << ">>> �˵�: [1] ���빦�� [2]ɾ������ [3]��ѯ���� [4]�޸Ĺ��� [5]ͳ�ƹ��� [6]����˵�� [0]�˳�ϵͳ" << endl;
    cout << endl << "��ѡ����Ҫ���еĲ���:";
    char optn;
    while (true) {
        optn = _getch();
        if (optn == 0 || optn == -32) {
            optn = _getch();
        }
        else if (optn >= '0' && optn <= '6') {
            cout << "[" << optn << "]" << endl << endl;
            return Operation(optn - '0');
        }
    }
}

void examRegistrationSystem() {
    cout << "+----------------------------+" << endl;
    cout << "|        ���Ա���ϵͳ        |" << endl;
    cout << "|  Exam Registration System  |" << endl;
    cout << "+----------------------------+" << endl;
    cout << endl << ">>> �뽨��������Ϣϵͳ" << endl;
    cout << endl << "�����뿼������ [������Χ: 1~2147483647]:";
    int studentNum = inputInteger(1, INT_MAX, "��������");
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
            cout << endl << ">>> ���Ա���ϵͳ���˳�" << endl;
            break;
        }
    }
}

int main() {
    examRegistrationSystem();
    return 0;
}