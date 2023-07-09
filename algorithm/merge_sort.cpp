#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <assert.h> 

void mergeSort(int *a, int n) {
    if (n <= 1) {
        return;
    }
    mergeSort(a, n / 2);
    mergeSort(a + (n / 2), n - (n / 2));
    int* b = (int*)malloc(sizeof(int) * n);
    int i = 0, j = n / 2, k = 0;
    while (i < n / 2 && j < n) {
        if (a[i] <= a[j]) {
            b[k++] = a[i++];
        } else {
            b[k++] = a[j++];
        }
    }
    while (i < n / 2) {
        b[k++] = a[i++];
    }
    while (j < n) {
        b[k++] = a[j++];
    }
    assert(k == n);
    memcpy(a, b, sizeof(int) * n);
    free(b);
}

int main() {
    int a[100] = {0};
    for (int i = 0; i < 100; i++) {
        a[i] = rand() % 1000;
    }
    mergeSort(a, 100);
    for (int i = 0; i < 100; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}

