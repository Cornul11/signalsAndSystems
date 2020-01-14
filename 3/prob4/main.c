#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int uint;
uint prime = 40961;

uint *readSignal(uint *len) {
    uint *x;
    char c;
    scanf("%u:", len);
    x = calloc(*len, sizeof(uint));
    do c = getchar(); while (c != '[');
    if (*len > 0) {
        scanf("%u", &x[0]);
        for (int i = 1; i < *len; i++) scanf(",%u", &x[i]);
    }
    do c = getchar(); while (c != ']');
    return x;
}

void destroyArray(void *arr) {
    free(arr);
}

void printIntArray(int length, int *arr) {
    printf("[");
    if (length > 0) {
        printf("%d", arr[0]);
        for (int i = 1; i < length; i++) {
            printf(",%d", arr[i]);
        }
    }
    printf("]\n");
}

void printDoubleArray(uint length, double *arr) {
    printf("[");
    if (length > 0) {
        printf("%.5lf", arr[0]);
        for (int i = 1; i < length; i++) {
            printf(",%.5lf", arr[i]);
        }
    }
    printf("]\n");
}

void printIntSignal(int length, int *arr) {
    printf("%d: ", length);
    printIntArray(length, arr);
}

void printDoubleSignal(uint length, double *arr) {
    printf("%d: ", length);
    printDoubleArray(length, arr);
}

void findRoot(int length, uint *w) {
    int len;
    *w = 243;
    for (len = 8192; len / 2 >= length; len /= 2) {
        *w = ((*w) * (*w)) % prime;
    }
}

void NTT(uint length, uint *arr, uint w) {
    int i, len;
    uint *oddArr, *evenArr, x = 1;

    if (length == 1) {
        return;
    }

    len = length / 2;
    
    evenArr = calloc(len, sizeof(uint));
    oddArr = calloc(len, sizeof(uint));
    
    for (i = 0; i < len; i++) {
        evenArr[i] = arr[2 * i] % prime;
        oddArr[i] = arr[2 * i + 1] % prime;
    }

    NTT(len, evenArr, (w * w) % prime);
    NTT(len, oddArr, (w * w) % prime);

    //main operation    
    for (i = 0; i < len; i++) {
        uint component = x * oddArr[i];
        arr[i] = (evenArr[i] + component % prime) % prime;
        arr[i + len] = (evenArr[i] - component % prime + prime) % prime;
        x = (x * w) % prime;
    }

    destroyArray(evenArr);
    destroyArray(oddArr);
}

int main(int argc, char *argv[]) {
    uint *A, lenA, w;

    A = readSignal(&lenA);
    findRoot(lenA, &w);
    NTT(lenA, A, w);
    printIntSignal(lenA, A);

    destroyArray(A);
    return 0;
}