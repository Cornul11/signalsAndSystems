#include <stdio.h>
#include <stdlib.h>

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
    printf("%d: [", length);
    if (length > 0) {
        printf("%d", arr[0]);
        for (int i = 1; i < length; i++) {
            printf(",%d", arr[i]);
        }
    }
    printf("]\n");
}

int computeSumForH(int *x, int xLength, int *h, int i) {
    int sum = 0;
    for (int j = 0; j < i; j++) {
        if (i - j < 0 || i - j >= xLength) {
            continue;
        }
        sum = sum + (x[i - j] * h[j]);
    }
    return sum;
}

int *checkFIRFilter(int *x, int *y, int xLength, int yLength, int *hLength) {
    int *h, divider = x[0];
    (*hLength) = yLength - xLength + 1;
    h = makeIntArray(*hLength);
    for (int i = 0; i < (*hLength); i++) {
        h[i] = (y[i] - computeSumForH(x, xLength, h, i)) / divider;
    }
    return h;
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

int *applyFIRFilter(int *h, int *x, int n, int m, int *k) {
    (*k) = n + m - 1;
    int *y = makeIntArray(*k);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            y[i + j] += h[i] * x[j];
        }
    }
    return y;
}

int checkArraysSame(int *arr1, int *arr2, int len1, int len2) {
    if (len1 != len2) {
        return 0;
    }
    for (int i = 0; i < len1; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int *x, *y, *h, *testingY, xLength, yLength, hLength, testingYLength;

    x = inputIntSignal(&xLength);
    y = inputIntSignal(&yLength);

    h = checkFIRFilter(x, y, xLength, yLength, &hLength);

    testingY = applyFIRFilter(h, x, hLength, xLength, &testingYLength);

    if (checkArraysSame(y, testingY, yLength, testingYLength)) {
        printSignal(hLength, h);
    } else {
        printf("NO FIR\n");
    }

    destroyArray(x);
    destroyArray(y);
    destroyArray(h);
    destroyArray(testingY);
    return 0;
}