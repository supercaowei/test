#include <stdio.h>

class Base {
public:
    Base() {
        printf("Base::Base()\n");
    }
    Base(const Base& other) {
        printf("Base: copy constructor\n");
    }
    virtual ~Base() = default;
    Base& operator=(const Base& other) {
        printf("Base: copy assignment operator\n");
        return *this;
    }
};

class Derived : public Base {
public:
    Derived() {
        printf("Derived::Derived()\n");
    }
    Derived(const Derived& other) : Base(other) {
        printf("Derived: copy constructor\n");
    }
    Derived& operator=(const Derived& other) {
        Base::operator=(other);
        printf("Derived: copy assignment operator\n");
        return *this;
    }
};

class Derived2 : public Derived {
public:
    Derived2() {
        printf("Derived2::Derived2\n");
    }
    ~Derived2() {
        printf("Derived2::~Derived2\n");
    }
    Derived2& operator=(const Derived2& other) = default;
};

int main() {
    // Derived a;
    // Derived b(a);
    // Derived c;
    // c = a;
    // Derived2 d, e;
    // e = d;
    Derived2 f;
    Derived2 g(f);
    return 0;
}

