#include <stdio.h>

class Base1 {
public:
    Base1() {
        printf("Base1::Base1()\n");
    }

    virtual ~Base1() {
        printf("Base1::~Base1()\n");
    }
};

class Base2 {
public:
    Base2() {
        printf("Base2::Base2()\n");
    }

    virtual ~Base2() {
        printf("Base2::~Base2()\n");
    }
};

class Base3 {
public:
    Base3() {
        printf("Base3:: Base3()\n");
    }

    virtual ~Base3() {
        printf("Base3::~Base3()\n");
    }
};

class Derived1 : virtual public Base1, virtual public Base2 {
public:
    Derived1() {
        printf("Derived1::Derived1()\n");
    }

    virtual ~Derived1() {
        printf("Derived1::~Derived1()\n");
    }
};

class Derived2 : virtual public Base2, virtual public Base1 {
public:
    Derived2() {
        printf("Derived2::Derived2()\n");
    }

    virtual ~Derived2() {
        printf("Derived2::~Derived2()\n");
    }
};

class Derived : public Derived1, virtual public Base3, public Derived2 {
public:
    Derived() {
        printf("Derived::Derived()\n");
    }

    virtual ~Derived() {
        printf("Derived::~Derived()\n");
    }
};

int main() {
    Derived d;
    return 0;
}
