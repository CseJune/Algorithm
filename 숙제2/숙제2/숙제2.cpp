#include <iostream>
using namespace std;

class Animal {
public:
	Animal() {
		cout << " Animal 기본생성자 호출 " << endl;
	}
	virtual void makesound() = 0;
	~Animal() {
		cout << " Animal 소멸자 호출" << endl;
	}
};

class Dog : public Animal {
public:
	Dog() {
		cout << "DOG기본생성자 호출" << endl;
	}
	void makesound() {
		cout << "개 짖는 소리" << endl;
	}
	~Dog() {
		cout << " Dog소멸자 호출 " << endl;
	}
};

class Cat : public Animal {
public:
	Cat() {
		cout << " Cat 기본생성자 호출" << endl;
	}
	void makesound() {
		cout << "고양이 울음소리" << endl;
	}
	~Cat() {
		cout << " Cat 소멸자 호출 " << endl;
	}
};

class Cow : public Animal {
public:
	Cow() {
		cout << " Cow 기본생성자 호출 " << endl;
	}
	void makesound() {
		cout << "소 울음소리" << endl;
	}
	~Cow() {
		cout << " Cow 소멸자 호출 " << endl;
	}
};

int main() {
	Animal* makesounds[3];
	makesounds[0] = new Dog();
	makesounds[1] = new Cat();
	makesounds[2] = new Cow();

	for (int i = 0; i < 3; i++) {
		makesounds[i]->makesound();
	}

	for (int i = 0; i < 3; i++) {
		delete makesounds[i];
	}
	return 0;

}