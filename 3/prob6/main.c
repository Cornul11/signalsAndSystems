#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef unsigned int uint;

int prime = 40961;

void *safeMalloc(int sz) {
    void *p = calloc(sz, 1);
    if (p == NULL) {
        fprintf(stderr, "Fatal error: safeMalloc(%d) failed.\n", sz);
        exit(EXIT_FAILURE);
    }
    return p;
}

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

int *makeIntArray(int n) {
    /* allocates dynamic int array of size/length n */
    return safeMalloc(n * sizeof(int));
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

void findRoot(uint *w, int len) {
    *w = 243;
    for (int n = 8192; n / 2 >= len; n /= 2) {
        *w = (*w * (*w)) % prime;
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

uint euclidianGCD(uint a, uint b, int *p, int *q) {
    int x = 0, lx = 1, y = 1, ly = 0;
    while (b > 0) {
        int q = a / b;
        int h = a % b;
        a = b;
        b = h;

        h = x;
        x = lx - q * x;
        lx = h;

        h = y;
        y = ly - q * y;
        ly = h;
    }
    *p = lx;
    *q = ly;
    return a;
}

uint powmod(uint base, uint exponent, uint prime) {
    uint pm = 1;
    base = base % prime;
    while (exponent > 0) {
        if (exponent % 2 == 1) { /* exponent is odd */
            pm = (pm * base) % prime;
        }
        exponent /= 2;
        base = (base * base) % prime;
    }
    return pm;
}

int makeInverse(int old, int prime) {
    int x, y;
    euclidianGCD(old, prime, &x, &y);
    if (x < 0) {
        int temp = ((-x) + 1 / prime) * prime;
        x = x + temp;
    }
    return (int) x % prime;
}

void inverseNTT(uint *x, int len, int prime, int omega) {
    int i;
    int inverse;
    int invOmega = powmod(omega, len - 1, prime);

    NTT(len, x, invOmega);
    inverse = makeInverse(len, prime);

    for (i = 0; i < len; i++) {
        x[i] = (inverse * x[i]) % prime;
    }
}

int findBigN(int m) {
    int bigN = 0, i = 0;
    while (bigN == 0) {
        if (pow(2, i) > m) {
            bigN = i;
        }
        i++;
    }
    return bigN;
}

int *padZeros(int *arr, int oldSize, int newSize) {
    int *newArr = makeIntArray(newSize);
    for (int i = 0; i < oldSize; i++) {
        newArr[i] = arr[i];
    }
    destroyArray(arr);
    for (int i = oldSize; i < newSize; i++) {
        newArr[i] = 0;
    }
    return newArr;
}

uint *oneDConvolution(uint *h, uint *x, uint *k, uint n, uint m) {
    uint bigN, powerOfTwo, w, *y;
    powerOfTwo = findBigN(m > n ? m : n);
    bigN = 2 * (int) pow(2, powerOfTwo);

    findRoot(&w, bigN);

    h = padZeros(h, m, bigN);
    x = padZeros(x, n, bigN);

    y = calloc(bigN, sizeof(uint));

    NTT(bigN, x, w);
    NTT(bigN, h, w);

    for (int i = 0; i < bigN; i++) {
        y[i] = x[i] * h[i]; //convolution
    }

    inverseNTT(y, bigN, prime, w);

    *k = m + n - 1;

    destroyArray(h);
    destroyArray(x);
    return y;
}

int main(int argc, char *argv[]) {
    uint *h, m, k, *x, n, *y;

    x = readSignal(&n);
    h = readSignal(&m);

    y = oneDConvolution(h, x, &k, n, m);

    printIntSignal(k, y);

    destroyArray(y);
    return 0;
}