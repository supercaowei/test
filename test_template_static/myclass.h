#ifndef _CLASS_H_
#define _CLASS_H_

class MyClass {
public:
    MyClass() {}
    ~MyClass() {}
    virtual void func();

    const static int ClassI = 3;
    // const static int ClassI;
};

#endif
