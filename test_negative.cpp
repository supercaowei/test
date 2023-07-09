#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        return -1;
    }
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int num3 = atoi(argv[3]);
    int value = -1000000 * abs(num1) - 1000 * abs(num2) + (num3 < 0 ? num3 : (num3 % 1000) - 1000);
    printf("%d, %d, %d, %d, %d, %d, %d\n", num1, num2, num3, value,
        value / 1000000, (value % 1000000) / 1000, value % 1000);
    return 0;
}
