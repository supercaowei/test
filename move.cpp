#include <stdio.h>
#include <utility>
#include <string>

//编译命令需要指明用C++11编译：g++ -std=c++11 move.cpp -o move
//输出：s: , s1: hello, s2: hello

class C {
public:
    C (std::string&& s) {
        strncpy(s_, s.c_str(), s.length());
    }
    const char* c_str() {
        return s_;
    }
private:
    char s_[100];
};

int main() {
    std::string s("hello");
    std::string s1(std::move(s));
    C s2(std::move(s1));
    printf("s: %s, s1: %s, s2: %s\n", s.c_str(), s1.c_str(), s2.c_str()); 
    return 0;
}
