#include <stdio.h>

class A {
public:
	A(): i(0) {
		printf("A::A()\n");
	}

	virtual ~A() {};

protected:
	int i;
};

class B : public A {
public:
	B(): i(setI()) {

	}

	int setI() {
		printf("B::setI()\n");
		return 1;
	}
};


int main()
{
	B b;
	return 0;
}
