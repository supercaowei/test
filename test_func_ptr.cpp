#include <iostream>
using namespace std;
 
class A{
public:
    A(){
    }
    
    void funa(void){
        puts("A");
    }
    
    static void funb(void){
        puts("B");
    }
};
 
typedef void (A::*P)(void);

int main()
{
    A *a = new A;
    // void (A::*p)(void);
    P p = (P)(reinterpret_cast<void*>(&A::funa));
    (a->*p)();//打印 A
    
    void (*pp)(void);
    pp = &A::funb;
	pp(); //打印 B
    
    delete a;
    return 0;
}
