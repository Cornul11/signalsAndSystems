#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

typedef complex double cd;
typedef int boolean;

#define false 0
#define true 1

const double pi = 3.141592653589793238462643383279502884;

void *safeMalloc(int sz) {
    void *p = calloc(sz, 1);
    if (p == NULL) {
        fprintf(stderr, "Fatal error: safeMalloc(%d) failed.\n", sz);
        exit(EXIT_FAILURE);
    }
    return p;
}

int *makeIntArray(int n) {
    /* allocates dynamic int array of size/length n */
    return safeMalloc(n * sizeof(int));
}

cd *makeComplexDoubleArray(int n) {
    /* allocates dynamic int array of size/length n */
    return safeMalloc(n * sizeof(cd));
}

void destroyArray(void *p) {
    free(p);
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

void printSignal(int length, int *arr) {
    printf("%d: ", length);
    printIntArray(length, arr);
}

int *inputIntSignal(int *length) {
    if (!scanf(" %d:", length)) {
        abort();
    }
    int *arr = makeIntArray((*length));
    for (int i = 0; i < (*length); i++) {
        if (i > 0 && i != (*length) - 1) {
            if (!scanf(" %d,", &arr[i])) {
                abort();
            }
        } else if (i == 0) {
            if (!scanf(" [%d,", &arr[i])) {
                abort();
            }
        } else {
            if (!scanf("%d]", &arr[i])) {
                abort();
            }
        }
    }
    return arr;
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

void copyComplexArray(cd *arr1, int *arr2, int len) {
    for (int i = 0; i < len; i++) {
        arr1[i] = arr2[i];
    }
}

cd computeOmega(int len, boolean negFlag) {
    int aux = negFlag ? -2 : 2;
    return cexp(aux * pi * I / len);
}

cd *FFT(cd *arr, cd omega, int len, boolean inversedFlag) {
    cd j, *evenArr, *oddArr, *yEvenArr, *yOddArr, *y;
    int halfLen;

    if (len == 1) {
        complex double *yyy = calloc(len, sizeof(complex double));
        for (int i = 0; i < len; i++) {
            yyy[i] = arr[i];
        }
        return yyy;
    }

    halfLen = len / 2;

    evenArr = makeComplexDoubleArray(halfLen);
    oddArr = makeComplexDoubleArray(halfLen);

    for (int i = 0; i < halfLen; i++) {
        evenArr[i] = arr[i * 2];
    }
    for (int i = 0; i < halfLen; i++) {
        oddArr[i] = arr[i * 2 + 1];
    }

    yEvenArr = FFT(evenArr, omega * omega, halfLen, false);
    yOddArr = FFT(oddArr, omega * omega, halfLen, false);
    y = makeComplexDoubleArray(len);

    j = 1;
    for (int i = 0; i < halfLen; i++) {
        y[i + halfLen] = yEvenArr[i] - j * yOddArr[i];
        y[i] = yEvenArr[i] + j * yOddArr[i];
        j = j * omega;
    }

    if (inversedFlag == true) {
        for (int i = 0; i < len; i++) {
            y[i] = y[i] / len;
        }
    }

    //cleaning up
    if (evenArr == yEvenArr) {
        destroyArray(evenArr);

    } else {
        destroyArray(evenArr);
        destroyArray(yEvenArr);
    }

    if (oddArr == yOddArr) {
        destroyArray(oddArr);
    } else {
        destroyArray(oddArr);
        destroyArray(yOddArr);
    }
    return y;
}

int *oneDConvolution(int *h, int *x, int *k, int n, int m) {
    int bigN, powerOfTwo, *y;
    cd *hC, *xC, omega, omegaInversed, *hFFT, *xFFT, *finalFFT, *doubleY;

    powerOfTwo = findBigN(m > n ? m : n);
    bigN = 2 * (int) pow(2, powerOfTwo);

    h = padZeros(h, n, bigN);
    x = padZeros(x, m, bigN);

    hC = makeComplexDoubleArray(bigN);
    xC = makeComplexDoubleArray(bigN);

    copyComplexArray(hC, h, bigN);
    copyComplexArray(xC, x, bigN);

    omega = computeOmega(bigN, false);
    omegaInversed = computeOmega(bigN, true);

    hFFT = FFT(hC, omega, bigN, false);
    xFFT = FFT(xC, omega, bigN, false);

    finalFFT = makeComplexDoubleArray(bigN);

    for (int i = 0; i < bigN; i++) {
        finalFFT[i] = xFFT[i] * hFFT[i]; //convolution
    }

    *k = m - n + 1;
    y = makeIntArray(*k);

    doubleY = FFT(finalFFT, omegaInversed, bigN, true);

    for (int i = 0; i < *k; i++) {
        y[i] = (int) round(creal(doubleY[i]));
    }

    //cleaning up
    destroyArray(h);
    destroyArray(x);
    destroyArray(hC);
    destroyArray(xC);
    destroyArray(hFFT);
    destroyArray(xFFT);
    destroyArray(finalFFT);
    destroyArray(doubleY);
    return y;
}

int main() {
    int *h, *x, *y, n, m, k;

    h = inputIntSignal(&n);
    x = inputIntSignal(&m);

    y = oneDConvolution(h, x, &k, n, m);

    printSignal(k, y);

    destroyArray(y);
    return 0;
}