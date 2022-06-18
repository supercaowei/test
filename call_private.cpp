#include <stdio.h>

class C {
public:
    virtual ~C() {
        printf("~C\n");
    }

private:
    virtual void hello(int i) {
        printf("hello %d\n", i);
    }

    virtual void bye(int i) {
        printf("bye %d\n", i);
    }
};

typedef void (*Func)(C*, int);
typedef void (*Destructor)(C*);

int main() {
    C *c = new C();
    intptr_t** vtable = reinterpret_cast<intptr_t**>(c);
    for (int i = 0; vtable[0][i] != 0; i++) {
        printf("vtable[0][%d]: %p\n", i, reinterpret_cast<void*>(vtable[0][i]));
    }
    Destructor d = reinterpret_cast<Destructor>(vtable[0][0]);
    Func f = reinterpret_cast<Func>(vtable[0][2]);
    Func f1 = reinterpret_cast<Func>(vtable[0][3]);
    f(c, 5);
    f1(c, 10);
    d(c);
    return 0;
}
