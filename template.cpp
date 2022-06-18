#include <iostream>

template<typename T>
class A {
public:
    A(const T& t) : t_(t) {

    }

    virtual void func1(T t) {
        std::cout << "t: " << t << std::endl;
    }

    virtual void func2() {
        std::cout << "t_: " << t_ << std::endl;
    }

    int compare(T t1, T t2) {
        if (t1 < t2) {
            return -1;
        } else if (t1 == t2) {
            return 0;
        } else {
            return 1;
        }
    }

private:
    T t_;
};

int main() {
    A<int> a(2);
    a.func1(3);
    a.func2();
    std::cout << a.compare(5, 8) << std::endl;
    return 0;
}
