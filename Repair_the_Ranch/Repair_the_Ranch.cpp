#include<iostream>
#include<new>

template<typename Type>
class MyPriorityQueue {
private:
	Type* elements;
	int maxsize;
	int currsize;
	void heapifyUp(int index);
	void heapifyDown(int index);
public:
	MyPriorityQueue(int _maxsize);
	~MyPriorityQueue() { delete[]elements; }
	bool isEmpty(void) const { return currsize == 0; }
	bool isFull(void) const { return currsize == maxSize; }
	int getSize(void) const { return currsize; }
	bool insert(const Type& item);
	bool remove(Type& item);
	bool getFront(Type& item) const;
};

template<typename Type>
void MyPriorityQueue<Type>::heapifyUp(int index) {

	while (index > 0) {

		int parent = (index - 1) / 2;

		if (elements[index] < elements[parent]) {
			Type temp = elements[parent];
			elements[parent] = elements[index];
			elements[index] = temp;
			index = parent;
		}
		else {
			break;
		}

	}

}

template<typename Type>
void MyPriorityQueue<Type>::heapifyDown(int index) {
	while (true) {

		int least = index;

		int left = 2 * index + 1;
		int right = 2 * index + 2;

		if (left < currsize && elements[left] < elements[least]) {
			least = left;
		}

		if (right < currsize && elements[right] < elements[least]) {
			least = right;
		}

		if (least != index) {
			Type temp = elements[least];
			elements[least] = elements[index];
			elements[index] = temp;
			index = least;
		}
		else {
			break;
		}

	}
}

template<typename Type>
MyPriorityQueue<Type>::MyPriorityQueue(int _maxsize){
	maxsize = _maxsize;
	currsize = 0;
	elements = new (nothrow) Type[maxsize];
	if (elements == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
}

template<typename Type>
bool MyPriorityQueue<Type>::insert(const Type& item) {
	if (isFull())
		return false;
	else {
		elements[currsize++] = item;
		heapifyUp(currsize - 1);
		return true;
	}
}

template<typename Type>
bool MyPriorityQueue<Type>::remove(Type& item) {
	if (isEmpty())
		return false;
	else {
		item = elements[0];
		elements[0] = elements[--currsize];
		heapifyDown(0);
		return true;
	}
}

template<typename Type>
bool MyPriorityQueue<Type>::getFront(Type& item) const {
	if (isEmpty())
		return false;
	else {
		item = elements[0];
		return true;
	}
}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt) {
	std::cout << ">>> " << "请输入" << prompt << " 整数范围: " << lowerLimit << "~" << upperLimit << "]: ";
	int input;
	while (true) {
		std::cin >> input;
		if (std::cin.good() && input >= lowerLimit && input <= upperLimit) {
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

int main() {
	int num = inputInteger(1, INT_MAX, "请输入要将木头切成的块数: ");
	MyPriorityQueue<int> pq(num);
	int totalCost;
	std::cout << "请依次输入 " << num << " 段木头的长度: " << std::endl;
	for (int i = 0; i < num; i++) {
		int length;
		std::cin >> length;
		pq.insert(length);
	}
	while (pq.getSize() > 1) {
		int first, second;
		pq.remove(first);
		pq.remove(second);
		int cost = first + second;
		pq.insert(cost);
	}
	pq.getFront(totalCost);
	std::cout << "最小切割成本为: " << totalCost<< std::endl;
	return 0;
}
