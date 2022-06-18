#include <stdlib.h>
#include <iostream>

class A
{
public:
    A(int i) : i_(i)
    {
        std::cout << "A::A(" << i_ << ")" << std::endl;
    }

    virtual ~A()
    {
        std::cout << "A::~A()" << std::endl;
    }
    
private:
    int i_;
};

int main() 
{
    A* a = (A*)malloc(sizeof(A));
    new(a) A(2);
    a->~A();
    free(a);
}
