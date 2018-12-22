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

int *findSteadyState(int *h, int *x, int hLength, int xLength, int *yLength) {
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

int main() {
    int *h, *x, *y, hLength, xLength, yLength;

    h = inputIntSignal(&hLength);
    x = inputIntSignal(&xLength);

    y = findSteadyState(h, x, hLength, xLength, &yLength);

    printf("%d: ", yLength);
    printIntArray(yLength, y);

    destroyArray(h);
    destroyArray(x);
    destroyArray(y);
    return 0;
}