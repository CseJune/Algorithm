#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

template <typename T>
class SimpleVector {
private:
	T* data; // int* data (템플릿 안쓰면)
	int size;
	int capacity;

public:
	SimpleVector() : capacity(10), size(0) {
		data = new T[capacity]; // new int[capacity]
	}

	SimpleVector(int cap) : capacity(cap), size(0) {
		data = new T[capacity]; // new int[capacity]
	}

	~SimpleVector() {
		delete[] data;
	}

	void push_back(T value) // (int value)
	{
		if (size < capacity) {
			data[size] = value;
			size++;
		}
		else { cout << " 배열이 가득 찼습니다. " << endl; }
	}

	void pop_back() {
		if (size > 0) { size--; }
		else { cout << " 비열이 비어 있습니다." << endl; }
	}

	int get_size() const { return size; }
	int get_capacity() const { return capacity; }
};

int main() {

	SimpleVector<int> vec1;
	cout << "현재 크기 : " << vec1.get_size() << endl;
	cout << "용량 : " << vec1.get_capacity() << endl;

	vec1.push_back(1);
	cout << "현재 크기 : " << vec1.get_size() << endl;
	cout << "용량 : " << vec1.get_capacity() << endl;

	vec1.push_back(2);
	cout << "현재 크기 : " << vec1.get_size() << endl;
	cout << "용량 : " << vec1.get_capacity() << endl;

	vec1.pop_back();
	cout << "현재 크기 : " << vec1.get_size() << endl;
	cout << "용량 : " << vec1.get_capacity() << endl;

	SimpleVector<double> vec2(5); // 템플릿 안쓰면 여기서 double로 못함! int로 만들었단 가정하에;
	cout << " 현재 크기 : " << vec2.get_size() << "|용량(vec2): " << vec2.get_capacity() << endl;

	vec2.push_back(3.14);
	cout << " 현재 크기 : " << vec2.get_size() << "|용량(vec2): " << vec2.get_capacity() << endl;


	return 0;
}

