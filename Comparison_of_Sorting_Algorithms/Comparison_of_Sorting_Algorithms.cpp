#include<iostream>
#include<random>
#include<conio.h>
#include<ctime>

const char Algorithm[8][35]
{
"冒泡排序 Bubble Sort",
"选择排序 Selection Sort",
"插入排序 Insertion Sort",
"希尔排序 Shell Sort",
"快速排序 Quick Sort",
"堆 排 序 Heap Sort",
"归并排序 Merge Sort",
"基数排序 Radix Sort"
};

template<typename Type>
class SortAlgorithm {
private:

	Type* arr;
	int numCount;
	int compareCount;

public:

	SortAlgorithm(int num);
	~SortAlgorithm();

	void inputArr(Type _arr[]);
	bool inputOptn();

	void bubbleSort();
	void selectionSort();
	void insertionSort();
	void shellSort();
	int partition(int low, int high);
	void quickSort(int low, int high);
	void heapify(int n, int i);
	void heapSort();
	void merge(int left, int mid, int right);
	void mergeSort(int left, int right);
	Type getMaxVal();
	void countSort(int exp);
	void radixSort();
};

template<typename Type>
SortAlgorithm<Type>::SortAlgorithm(int num) {
	numCount = num;
	compareCount = 0;
	arr = new(std::nothrow) int[num];
	if (arr == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
}

template<typename Type>
SortAlgorithm<Type>::~SortAlgorithm() {
	delete[]arr;
}

template<typename Type>
void SortAlgorithm<Type>::inputArr(Type _arr[]) {
	for (int i = 0; i < numCount; i++) {
		arr[i] = _arr[i];
	}
}

template <typename Type>
void mySwap(Type& a, Type& b){
	Type tmp = a;
	a = b;
	b = tmp;
}

template<typename Type>
bool SortAlgorithm<Type>::inputOptn() {

	std::cout << std::endl << ">>> 请选择排序算法: ";
	char option;
	
	while (true) {
		option = _getch();
		if (option == 0 || option == -32) {
			option = _getch();
		}
		else if (option >= '1' || option <= '9') {
			std::cout << "[" << option << "]" << std::endl << std::endl;
			break;
		}
	}

	compareCount = 0;
	std::time_t start = std::time(nullptr);

	if (option == '1')
		bubbleSort();
	else if (option == '2')
		selectionSort();
	else if (option == '3')
		insertionSort();
	else if (option == '4')
		shellSort();
	else if (option == '5')
		quickSort(0, numCount - 1);
	else if (option == '6')
		heapSort();
	else if (option == '7')
		mergeSort(0, numCount - 1);
	else if (option == '8')
		radixSort();
	else if (option == '9')
		return false;

	std::time_t end = std::time(nullptr);
	double elapsedtime = double(end - start) / CLOCKS_PER_SEC;
	int alg = option - '0' - 1;
	if (alg >= 0 || alg <= 9) {
		std::cout << Algorithm[alg] << "所用时间: " << elapsedtime << std::endl;
		std::cout << Algorithm[alg] << "交换次数: " << compareCount << std::endl << std::endl;
	}

	return true;
}

template<typename Type>
void SortAlgorithm<Type>::bubbleSort() {
	for (int i = 0; i < numCount - 1; i++) {
		for (int j = 0; j < numCount - i - 1; j++) {
			compareCount++;
			if (arr[j] > arr[j + 1]) 
				mySwap(arr[j], arr[j + 1]);
		}
	}
}

template<typename Type>
void SortAlgorithm<Type>::selectionSort() {
	for (int i = 0; i < numCount - 1; i++) {
		int minIdx = i;
		for (int j = i + 1; j < numCount; j++) {
			compareCount++;
			if (arr[j] < arr[minIdx]) 
				minIdx = j;
		}
		mySwap(arr[i], arr[minIdx]);
	}
}

template<typename Type>
void SortAlgorithm<Type>::insertionSort() {
	for (int i = 1; i < numCount; i++) {
		int key = arr[i], j = i - 1;
		while (j >= 0 && arr[j] > key) {
			compareCount++;
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

template<typename Type>
void SortAlgorithm<Type>::shellSort() {
	for (int gap = numCount / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < numCount; i++) {
			Type temp = arr[i], j = i;
			while (j >= gap && arr[j - gap] > temp) {
				compareCount++;
				arr[j] = arr[j - gap];
				j -= gap;
			}
			arr[j] = temp;
		}
	}
}

template<typename Type>
int SortAlgorithm<Type>::partition(int low, int high) {
	int pivot = arr[high];
	int i = low - 1;
	for (int j = low; j < high; j++) {
		compareCount++;
		if (arr[j] < pivot) 
			mySwap(arr[++i], arr[j]);
	}
	mySwap(arr[i + 1], arr[high]);
	return i + 1;
}

template<typename Type>
void SortAlgorithm<Type>::quickSort(int low, int high) {
	if (low < high) {
		int pi = partition(low, high);
		quickSort(low, pi - 1);
		quickSort(pi + 1, high);
	}
}

template<typename Type>
void SortAlgorithm<Type>::heapify(int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	compareCount++;
	if (left < numCount && arr[left] > arr[largest]) 
		largest = left;
	compareCount++;
	if (right < numCount && arr[right] > arr[largest]) 
		largest = right;
	compareCount++;
	if (largest != i) {
		mySwap(arr[i], arr[largest]);
		heapify(numCount, largest);
	}
}

template<typename Type>
void SortAlgorithm<Type>::heapSort() {
	for (int i = numCount / 2 - 1; i >= 0; i--)
		heapify(numCount, i);
	for (int i = numCount - 1; i > 0; i--) {
		mySwap(arr[0], arr[i]);
		heapify(i, 0);
	}
}

template<typename Type>
void SortAlgorithm<Type>::merge(int left, int mid, int right) {
	int n1 = mid - left + 1, n2 = right - mid, i = 0, j = 0, k = left;
	Type* leftArr = new(std::nothrow) Type[n1];
	if (leftArr == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	Type* rightArr = new(std::nothrow) Type[n2];
	if (rightArr == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < n1; i++)
		leftArr[i] = arr[left + i];
	for (int i = 0; i < n2; i++)
		rightArr[i] = arr[mid + 1 + i];
	while (i < n1 && j < n2) {
		compareCount++;
		if (leftArr[i] <= rightArr[j])
			arr[k++] = leftArr[i++];
		else
			arr[k++] = rightArr[j++];
	}
	while (i < n1) {
		compareCount++;
		arr[k++] = leftArr[i++];
	}
	while (j < n2) {
		compareCount++;
		arr[k++] = rightArr[j++];
	}
	delete[] leftArr;
	delete[] rightArr;
}

template<typename Type>
void SortAlgorithm<Type>::mergeSort(int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		mergeSort(left, mid);
		mergeSort(mid + 1, right);
		merge(left, mid, right);
	}
}

template<typename Type>
Type SortAlgorithm<Type>::getMaxVal() {
	Type maxVal = arr[0];
	for (int i = 1; i < numCount; i++)
		if (arr[i] > maxVal)
			maxVal = arr[i];
	return maxVal;
}

template<typename Type>
void SortAlgorithm<Type>::countSort(int exp) {
	Type* output = new(std::nothrow) Type[numCount];
	if (output == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	int i, count[10] = { 0 };
	for (i = 0; i < numCount; i++)
		count[(arr[i] / exp) % 10]++;
	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];
	for (i = numCount - 1; i >= 0; i--) {
		output[count[(arr[i] / exp) % 10] - 1] = arr[i];
		count[(arr[i] / exp) % 10]--;
	}
	for (i = 0; i < numCount; i++)
		arr[i] = output[i];
	delete[] output;
}

template<typename Type>
void SortAlgorithm<Type>::radixSort() {
	Type maxVal = getMaxVal();
	for (int exp = 1; maxVal / exp > 0; exp *= 10) {
		countSort(exp);
	}
}

int inputInteger(int lowerLimit, int upperLimit, const char* prompt) {
	std::cout << std::endl;
	std::cout << ">>>" << "请输入" << prompt << " 整数范围: " << lowerLimit << "~" << upperLimit << "]: ";
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

int main(){

	srand((unsigned int)time(0));

	std::cout << "+------------------------------------+" << std::endl;
	std::cout << "|            排序算法比较            |" << std::endl;
	std::cout << "|  Comparison of Sorting Algorithms  |" << std::endl;
	std::cout << "+------------------------------------+" << std::endl;
	std::cout << "|          [1] --- 冒泡排序          |" << std::endl;
	std::cout << "|          [2] --- 选择排序          |" << std::endl;
	std::cout << "|          [3] --- 插入排序          |" << std::endl;
	std::cout << "|          [4] --- 希尔排序          |" << std::endl;
	std::cout << "|          [5] --- 快速排序          |" << std::endl;
	std::cout << "|          [6] --- 堆 排 序          |" << std::endl;
	std::cout << "|          [7] --- 归并排序          |" << std::endl;
	std::cout << "|          [8] --- 基数排序          |" << std::endl;
	std::cout << "|          [9] --- 退出程序          |" << std::endl;
	std::cout << "+------------------------------------+" << std::endl;

	int num = inputInteger(1, INT_MAX, "要生成随机数的个数");
	int* _arr = new(std::nothrow) int[num];
	if (_arr == nullptr) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(-1);
	}
	for (int i = 0; i < num; i++) {
		_arr[i] = rand();
	}
	SortAlgorithm<int> sort(num);
	sort.inputArr(_arr);
	std::cout << std::endl << ">>> 随机数生成成功 (生成数量:" << num << ")" << std::endl;

	while (sort.inputOptn()) { sort.inputArr(_arr); }

	std::cout << std::endl << ">>> 排序算法比较结束" << std::endl;
	delete[]_arr;
	return 0;
}
