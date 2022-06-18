#include "mytemplate.h"
#include "myclass.h"
#include <stdio.h>

int main()
{
    MyClass *myObj = new MyClass;
    myObj->func();
    delete myObj;
    return 0;
}

// const int MyClass::ClassI = 3;

void MyClass::func() 
{
    int i = my_max(5, MyClass::ClassI);
    printf("max: %d\n", i);
}
