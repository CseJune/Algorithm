#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

template <typename T>
class SimpleVector {
private:
	T* data; // int* data (���ø� �Ⱦ���)
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
		else { cout << " �迭�� ���� á���ϴ�. " << endl; }
	}

	void pop_back() {
		if (size > 0) { size--; }
		else { cout << " ���� ��� �ֽ��ϴ�." << endl; }
	}

	int get_size() const { return size; }
	int get_capacity() const { return capacity; }
};

int main() {

	SimpleVector<int> vec1;
	cout << "���� ũ�� : " << vec1.get_size() << endl;
	cout << "�뷮 : " << vec1.get_capacity() << endl;

	vec1.push_back(1);
	cout << "���� ũ�� : " << vec1.get_size() << endl;
	cout << "�뷮 : " << vec1.get_capacity() << endl;

	vec1.push_back(2);
	cout << "���� ũ�� : " << vec1.get_size() << endl;
	cout << "�뷮 : " << vec1.get_capacity() << endl;

	vec1.pop_back();
	cout << "���� ũ�� : " << vec1.get_size() << endl;
	cout << "�뷮 : " << vec1.get_capacity() << endl;

	SimpleVector<double> vec2(5); // ���ø� �Ⱦ��� ���⼭ double�� ����! int�� ������� �����Ͽ�;
	cout << " ���� ũ�� : " << vec2.get_size() << "|�뷮(vec2): " << vec2.get_capacity() << endl;

	vec2.push_back(3.14);
	cout << " ���� ũ�� : " << vec2.get_size() << "|�뷮(vec2): " << vec2.get_capacity() << endl;


	return 0;
}

