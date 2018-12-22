#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void destroyArray(void *p) {
    free(p);
}

void printDoubleArray(int length, double *arr) {
    printf("[");
    if (length > 0) {
        printf("%.2lf", arr[0]);
        for (int i = 1; i < length; i++) {
            printf(",%.2lf", arr[i]);
        }
    }
    printf("]\n");
}

void printDoubleArrayPrecision(int length, double *arr, int precision) {
    printf("[");
    if (length > 0) {
        printf("%.*lf", precision, arr[0]);
        for (int i = 1; i < length; i++) {
            printf(",%.*lf", precision, arr[i]);
        }
    }
    printf("]\n");
}


double *makeDoubleArray(int n) {
    /* allocates dynamic double array of size/length n */
    return safeMalloc(n * sizeof(double));
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

void printDoubleSignal(int length, double *arr, int precision) {
    printf("%d: ", length);
    printDoubleArrayPrecision(length, arr, precision);
}

void printIntSignal(int length, int *arr) {
    printf("%d: ", length);
    printIntArray(length, arr);
}

int *inputIntSignal(int *length) {
    scanf(" %d:", length);
    int *arr = makeIntArray(*length);
    if ((*length) > 1) {
        for (int i = 0; i < (*length); i++) {
            if (i > 0 && i != (*length) - 1) {
                scanf(" %d,", &arr[i]);
            } else if (i == 0) {
                scanf(" [%d,", &arr[i]);
            } else {
                scanf("%d]", &arr[i]);
            }
        }
    } else {
        scanf(" [%d]", &arr[0]);
    }
    return arr;
}

int *correlator1D(int *h, int *x, int hLength, int xLength, int *yLength) {
    (*yLength) = 1 + xLength - hLength;
    int *y = makeIntArray(*yLength);
    for (int d = 0; d < *yLength; d++) {
        int sum = 0;
        for (int i = 0; i < hLength; i++) {
            sum += x[i + d] * h[i];
        }
        y[d] = sum;
    }
    return y;
}

double computeIntMean(int *arr, int len) {
    double mean = 0;
    for (int i = 0; i < len; i++) {
        mean += arr[i];
    }
    mean = (mean / len);
    return mean;
}

double computeDoubleMean(double *arr, int len) {
    double mean = 0;
    for (int i = 0; i < len; i++) {
        mean += arr[i];
    }
    return (mean / len);
}

double *pearsonCorrelator1D(int *h, int *x, int *y, int hLength, int yLength) {
    double *pearsonY = makeDoubleArray(yLength), hMean, xMean, sum1, sum2, sum3;
    hMean = computeIntMean(h, hLength);
    for (int d = 0; d < yLength; d++) {
        sum2 = sum3 = xMean = 0;

        for (int i = 0; i < hLength; i++) {
            xMean += x[i + d];
        }
        xMean = xMean / hLength;

        sum1 = y[d] - hLength * xMean * hMean;

        for (int i = 0; i < hLength; i++) {
            sum2 += pow(x[i + d], 2);
        }
        sum2 = sqrt(sum2 - hLength * pow(xMean, 2));

        for (int i = 0; i < hLength; i++) {
            sum3 += pow(h[i], 2);
        }
        sum3 = sqrt(sum3 - hLength * pow(hMean, 2));

        pearsonY[d] = sum1 / (sum2 * sum3);
    }
    return pearsonY;
}

int main() {
    int *h, *x, *y, hLength, xLength, yLength;
    double *pearsonY, T;

    scanf("%lf", &T);

    h = inputIntSignal(&hLength);
    x = inputIntSignal(&xLength);

    y = correlator1D(h, x, hLength, xLength, &yLength);

    pearsonY = pearsonCorrelator1D(h, x, y, hLength, yLength);

    for (int i = 0; i < yLength; i++) {
        if (pearsonY[i] > T) {
            printf("%d %.5lf\n", i, pearsonY[i]);
        }
    }

    destroyArray(h);
    destroyArray(x);
    destroyArray(y);
    destroyArray(pearsonY);
    return 0;
}