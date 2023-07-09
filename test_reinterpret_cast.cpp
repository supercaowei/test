#include <stdio.h>

class Base1 
{
public:
    Base1() {}
    virtual ~Base1() {}
    int i1 = 1;
};

class Base2
{
public:
    Base2() {}
    virtual ~Base2() {}
    virtual void func2()
    {
        printf("Base2::func2, i2 = %d\n", i2);
    }
    int i2 = 2;
};

class Derived : public Base1, public Base2
{
public:
    Derived() {}
    virtual ~Derived() {}
    int i3 = 3;
};

int main()
{
    Derived* o = new Derived;
    printf("Obj address as Derived %p, as Base1 %p, as Base2 %p.\n", o, static_cast<Base1*>(o), static_cast<Base2*>(o));
    int64_t addr = reinterpret_cast<int64_t>(static_cast<Base1*>(o));
    Base2* o1 = static_cast<Base2*>(static_cast<Derived*>(reinterpret_cast<Base1*>(addr)));
    o1->func2();
    Base2* o2 = reinterpret_cast<Derived*>(addr);
    o2->func2();
    delete o;

    return 0;
}
