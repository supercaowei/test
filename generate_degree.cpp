#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int lines = 10;
    if (argc > 1) {
        lines = atoi(argv[1]);
    }
    srand(1);
    for (int i = 0; i < lines; i++) {
        float degree1 = 36 + (rand() % 7) * 0.1f;
        float degree2 = degree1 + ((rand() % 5) - 2) * 0.1f;
        degree2 = degree2 < 36.0 ? 36.0 : (degree2 > 36.6 ? 36.6 : degree2);
        printf("%.1f\t%.1f\n", degree1, degree2);
    }
    return 0;
}
