#include <stdio.h>
#include <exception>

class DevideZeroException : public std::exception {
public:
    const char* what() const noexcept override {
        return "device by zero";
    }

    ~DevideZeroException() {
        printf("~DevideZeroException\n");
    }
};

int main() {
    try {
        int devide = 0;
        if (devide == 0) {
            throw new DevideZeroException();
        }
        int i = 1000 / devide;
        printf("i: %d\n", i);
    } catch (const std::exception* e) {
        printf("exception: %s\n", e->what());
        delete e;
    }
    return 0;
}
