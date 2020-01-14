#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int uint;

uint prime = 40961;
uint pRoot = 243;

int *readSignal(uint *len) {
    int *x;
    char c;
    scanf("%d:", len);
    x = calloc(*len, sizeof(int));
    do c = getchar(); while (c != '[');
    if (*len > 0) {
        scanf("%d", &x[0]);
        for (int i = 1; i < *len; i++) scanf(",%d", &x[i]);
    }
    do c = getchar(); while (c != ']');
    return x;
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

uint inverse(uint a, uint prime) {
    int x, y;
    euclidianGCD(a, prime, &x, &y);
    if (x < 0) {
        x += (1 + (-x) / prime) * prime;
    }
    return (uint) x % prime;
}

int findRoot(int length, uint *omega) {
    int len;
    *omega = pRoot;
    for (len = 8192; len / 2 >= length; len /= 2) {
        *omega = ((*omega) * (*omega)) % prime;
    }
    return len;
}


void recNTT(uint length, uint root, uint prime, uint *a) {
    uint j, len, omega, sqroot, *odd, *even;
    if (length == 1) {
        return;
    }

    len = length / 2;
    even = malloc(len * sizeof(uint));
    odd = malloc(len * sizeof(uint));
    for (int i = j = 0; i < len; i++) {
        even[i] = a[j++];
        odd[i] = a[j++];
    }

    sqroot = (root * root) % prime;
    recNTT(len, sqroot, prime, even);
    recNTT(len, sqroot, prime, odd);

    omega = 1;
    for (int i = 0; i < len; i++) {
        uint h = (omega * odd[i]) % prime;
        a[i] = (even[i] + h) % prime;
        a[i + len] = (even[i] + prime - h) % prime;
        omega = (omega * root) % prime;
    }

    free(even);
    free(odd);
}

void NTT(uint prime, uint omega, uint length, uint *x) {
    recNTT(length, omega, prime, x);
}

void iNTT(uint prime, uint omega, uint length, uint *x) {
    uint inv, invOmega = powmod(omega, length - 1, prime);
    recNTT(length, invOmega, prime, x);
    inv = inverse(length, prime);
    for (int i = 0; i < length; i++) {
        x[i] = (x[i] * inv) % prime;
    }
}

int *reverseArray(uint length, int *arr) {
    int *y = malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        y[i] = arr[length - 1 - i];
    }
    return y;
}

uint *oneDCorrelator(int *x, int *y, uint *k, uint n, uint m) {
    uint omega, finalLen, *tmp, *z;
    int *h = reverseArray(n, x);

    *k = n + m - 1;
    finalLen = findRoot(*k, &omega);
    z = calloc(finalLen, sizeof(uint));
    tmp = calloc(finalLen, sizeof(uint));

    for (int i = 0; i < n; i++) {
        tmp[i] = h[i];
    }
    for (int i = 0; i < m; i++) {
        z[i] = y[i];
    }

    NTT(prime, omega, finalLen, tmp);
    NTT(prime, omega, finalLen, z);

    for (uint i = 0; i < finalLen; i++) {
        z[i] = (z[i] * tmp[i]) % prime;
    }

    free(tmp);

    iNTT(prime, omega, finalLen, z);
    return (uint *) z;
}

uint *getSteadyState(uint lenH, uint lenX, uint *lenY, uint *y) {
    uint *steadyState, temp;
    temp = *lenY;
    *lenY = lenX - lenH + 1;
    temp = (temp - *lenY) / 2;
    steadyState = calloc(*lenY, sizeof(uint));

    for (int i = 0; i < *lenY; i++) {
        steadyState[i] = y[i + temp];
    }
    return steadyState;
}

double mean(uint lenX, int *x, int d, uint lenH) {
    double count = 0.0, sum = 0.0;
    for (int i = d; i < d + lenH && i < lenX; i++) {
        sum += x[i];
        count++;
    }
    return sum / count;
}

double sumOfSquares(uint lenX, int *x, int d, uint lenH) {
    double sum = 0.0;
    for (int i = d; i < d + lenH && i < lenX; i++) {
        sum += x[i] * x[i];
    }
    return sum;
}

double *pearsonCorrelator1D(uint lenH, int *h, uint lenX, int *x, uint lenCor, uint *cor) {
    double *pCor, xbar, sumX, hbar, sumH;
    pCor = malloc(lenCor * sizeof(double));
    hbar = mean(lenH, h, 0, lenH);
    sumH = sumOfSquares(lenH, h, 0, lenH);
    for (int i = 0; i < lenCor; i++) {
        xbar = mean(lenX, x, i, lenH);
        sumX = sumOfSquares(lenX, x, i, lenH);
        pCor[i] = (cor[i] - lenH * xbar * hbar)
                  / (sqrt(sumX - lenH * xbar * xbar) * sqrt(sumH - lenH * hbar * hbar));
    }
    return pCor;
}

void destroyArray(void *p) {
    free(p);
}

int main(int argc, char *argv[]) {
    float T;
    int *h, *x;
    uint n, m, k, *y, *cor;
    double *pearsonY;

    h = readSignal(&n);
    x = readSignal(&m);

    y = oneDCorrelator(h, x, &k, n, m);
    cor = getSteadyState(n, m, &k, y);

    pearsonY = pearsonCorrelator1D(n, h, m, x, k, cor);

    printDoubleSignal(k, pearsonY);

    destroyArray(y);
    destroyArray(x);
    destroyArray(h);
    destroyArray(cor);
    destroyArray(pearsonY);
    return 0;
}