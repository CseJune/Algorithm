#include <iostream>
using namespace std;

// 1. 배열의 합을 구하는 함수 만들기
int ssum(int num[],int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += num[i];
	}return sum;
}

// 2. 합을 평균내는 함수 만들기
int aavg(int num[],int size) {
	double sum = ssum(num, size);
	return sum / size;
}

// 3. 구현하는 메인 함수 작성하기

int main() {
	const int size = 5;
	int numbers[size];
	cout << "5개의 정수를 입력해 주세요." << endl;
	for (int a = 0; a < size; a++) {
		cout << a+1 << "번 째 숫자 : ";
		cin >> numbers[a];
	}
	int lastsum = ssum(numbers, size);
	double lastavg = aavg(numbers, size);

	cout << "합 : " << lastsum << endl;
	cout << "평균 : " << lastavg << endl;
}