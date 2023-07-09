#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	B() {
		i = setI();
	}

	int setI() {
		printf("B::setI()\n");
		return 1;
	}
};


class C {
public:
	C() {
		func();
	}
	virtual void func() {
		printf("C::func\n");
	}
};

class D : public C {
public:
	D() {
		func();
	}
	void func() override {
		printf("D::func\n");
	}
};

int main()
{
	// B b;

	// bool a;
	// srand((unsigned int) time(0));
	// for (int i = 0; i < 10; i++) {
	// 	*(unsigned char*)(&a) = (char)(rand() % 255);
	// 	printf("%s %d\n", a ? "true" : "false", *(unsigned char*)(&a));
	// }

	C* c = new D();
	delete c;
	return 0;
}
