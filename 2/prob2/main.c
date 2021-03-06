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

int *applyFIRFilter(int *h, int *x, int n, int m, int *k) {
    (*k) = n + m - 1;
    int *y = makeIntArray(*k);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            y[i + j] = y[i + j] + h[i] * x[j];
        }
    }
    return y;
}

int main() {
    int *h1, *h2, *x, *y1, *y2, lenH1, lenH2, lenX, lenY;

    h1 = inputIntSignal(&lenH1);
    h2 = inputIntSignal(&lenH2);
    x = inputIntSignal(&lenX);


    y1 = applyFIRFilter(h1, x, lenH1, lenX, &lenY);
    y2 = applyFIRFilter(h2, y1, lenH2, lenY, &lenY);

    printSignal(lenY, y2);

    destroyArray(h1);
    destroyArray(h2);
    destroyArray(x);
    destroyArray(y1);
    destroyArray(y2);
    return 0;
}