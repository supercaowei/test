#include <stdio.h>

void insertionSort(int a[], int n) {
    for (int j = 1; j < n; j++) {
        int key = a[j];
        int i = j - 1; 
        for (; i >= 0 && a[i] > key; i--) {
            a[i + 1] = a[i]; //往后挪
        }
        a[i + 1] = key;
    }
}

int main() {
    int a[] = {5, 2, 4, 6, 1, 3};
    insertionSort(a, sizeof(a) / sizeof(int));
    for (int i = 0; i < sizeof(a) / sizeof(int); i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}

