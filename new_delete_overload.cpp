#include <iostream>

using namespace std;

void* operator new(size_t sz) {
    void* m = malloc(sz);
    cout << "global new(" << sz << "): " << m << endl;
    return m;
}

void* operator new[](size_t sz)  //重载new[]
{
    void* m = malloc(sz);
    cout << "global new[](" << sz << "): " << m << endl;
    return m;
}

void* operator new(size_t sz, const char* file, int line) {
    void* m = malloc(sz);
    cout << "global new(" << sz << "): " << m << "(" << file << ":" << line << ")" << endl;
    return m;
}

void* operator new[](size_t sz, const char* file, int line) {
    void* m = malloc(sz);
    cout << "global new[](" << sz << "): " << m << "(" << file << ":" << line << ")" << endl;
    return m;
}

void operator delete(void* m) noexcept   //重载delete
{
    cout << "global delete(" << m << ")" << endl;
    free(m);
}

void operator delete[](void* m) noexcept  //重载delete[]
{
    cout << "global delete[](" << m << ")" << endl;
    free(m);
}

class MyClass
{
public:
    MyClass() = default;
    ~MyClass() = default;

    void* operator new(size_t sz)    //重载new
    {
        void* m = malloc(sz);
        cout << "class new(" << sz << "): " << m << endl;
        return m;
    }

    void* operator new(size_t sz, int extra)    //重载new
    {
        void* m = malloc(sz);
        cout << "class new(" << sz << ", " << extra << "): " << m << endl;
        return m;
    }

    void* operator new[](size_t sz)  //重载new[]
    {
        void* m = malloc(sz);
        cout << "class new[](" << sz << "): " << m << endl;
        return m;
    }

    void* operator new[](size_t sz, int extra)  //重载new[]
    {
        void* m = malloc(sz);
        cout << "class new[](" << sz << ", " << extra << "): " << m << endl;
        return m;
    }

    void operator delete(void* m) noexcept   //重载delete
    {
        cout << "class delete(" << m << ")" << endl;
        free(m);
    }

    void operator delete[](void* m) noexcept  //重载delete[]
    {
        cout << "class delete[](" << m << ")" << endl;
        free(m);
    }

private:
    int num[3];
};

class MyClass2 : public MyClass {
public:
    MyClass2(int i) {
        i++;
        cout << "MyClass2 ctor, i: " << i << endl;
    }

private:
    char c[5];
};

#define TRACE_NEW new(__FILE__, __LINE__)

int main()
{
    MyClass* mclass = new MyClass;
    cout << "size of " << sizeof(*mclass) << endl;
    delete mclass;
 
    MyClass* mgroup = new(88) MyClass[2];
    cout << "size of " << sizeof(*mgroup) << endl;
    delete[] mgroup;

    MyClass2* c2 = new(7) MyClass2(5);
    cout << "size of " << sizeof(*c2) << endl;
    delete c2;

    int *a = TRACE_NEW int[3];
    delete[] a;

    double *b = TRACE_NEW double;
    delete b;

    return 0;
}
