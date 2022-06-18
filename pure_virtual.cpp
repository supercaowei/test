
#include <stdio.h>

class Base {
public:
    Base() {}
    virtual ~Base() {
        PureVirtualFunc();
    }

    virtual void PureVirtualFunc() {
        printf("Base::PureVirtualFunc\n");
    }
};

class Derived : public Base {
public:
    Derived() = default;
    virtual ~Derived() = default;

    void PureVirtualFunc() override {
        printf("Derived::PureVirtualFunc\n");
        Base::PureVirtualFunc();
    }
};

int main() {
    Base *o = new Derived;
    delete o;
}
