#include <stdio.h>

class Base {
public:
    Base() {
    }

    virtual ~Base() {
        func();
    }

    virtual void func() {
        printf("Base::func()\n");
    }
};

class Derived : public Base {
public:
    Derived() {
        func();
    }

    virtual ~Derived() {
    }

    virtual void func() override {
        printf("Derived::func()\n");
    }
};

int main() {
    Derived a;
    return 0;
}
