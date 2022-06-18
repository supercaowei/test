#include <stdio.h>
#include <string>

class aggr {
public:
    constexpr aggr() {}
    constexpr aggr(int i1, const char* s1) : i(i1), s(s1) {}
    int i = 3;
    const char* s = nullptr;
    std::string* ps = 0;
};

int main() {
    constexpr aggr a;
    printf("i %d, s %s\n", a.i, a.s);
    return 0;
}
