#include <stdio.h>

class A {
public:
    int i1;
    char a, b;
    int i2;
};

class B {
public:
    char s[10];
};

class C : public A, public B {
public:
    C() {}
    // virtual ~C() {}
    // virtual void func() {}
};

int main() {
    C *c = new C;
    A *a = static_cast<A*>(c);
    B *b = static_cast<B*>(c);
    printf("a: %p, b: %p, c: %p\n", a, b, c);
    delete c;
    return 0;
}
