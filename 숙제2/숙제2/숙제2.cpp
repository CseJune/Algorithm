#include <iostream>
using namespace std;

class Animal {
public:
	Animal() {
		cout << " Animal �⺻������ ȣ�� " << endl;
	}
	virtual void makesound() = 0;
	~Animal() {
		cout << " Animal �Ҹ��� ȣ��" << endl;
	}
};

class Dog : public Animal {
public:
	Dog() {
		cout << "DOG�⺻������ ȣ��" << endl;
	}
	void makesound() {
		cout << "�� ¢�� �Ҹ�" << endl;
	}
	~Dog() {
		cout << " Dog�Ҹ��� ȣ�� " << endl;
	}
};

class Cat : public Animal {
public:
	Cat() {
		cout << " Cat �⺻������ ȣ��" << endl;
	}
	void makesound() {
		cout << "����� �����Ҹ�" << endl;
	}
	~Cat() {
		cout << " Cat �Ҹ��� ȣ�� " << endl;
	}
};

class Cow : public Animal {
public:
	Cow() {
		cout << " Cow �⺻������ ȣ�� " << endl;
	}
	void makesound() {
		cout << "�� �����Ҹ�" << endl;
	}
	~Cow() {
		cout << " Cow �Ҹ��� ȣ�� " << endl;
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