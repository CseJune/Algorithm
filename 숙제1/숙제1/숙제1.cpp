#include <iostream>
using namespace std;

// 1. �迭�� ���� ���ϴ� �Լ� �����
int ssum(int num[],int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += num[i];
	}return sum;
}

// 2. ���� ��ճ��� �Լ� �����
int aavg(int num[],int size) {
	double sum = ssum(num, size);
	return sum / size;
}

// 3. �����ϴ� ���� �Լ� �ۼ��ϱ�

int main() {
	const int size = 5;
	int numbers[size];
	cout << "5���� ������ �Է��� �ּ���." << endl;
	for (int a = 0; a < size; a++) {
		cout << a+1 << "�� ° ���� : ";
		cin >> numbers[a];
	}
	int lastsum = ssum(numbers, size);
	double lastavg = aavg(numbers, size);

	cout << "�� : " << lastsum << endl;
	cout << "��� : " << lastavg << endl;
}