#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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


int main(int argc, char *argv[]) {
    int samplingRate, n, *samples;
    double amp, f, phi;
    if (!scanf("%d %d %lf %lf %lf", &samplingRate, &n, &amp, &f, &phi)) {
        abort();
    }
    double t = 1.00 / samplingRate;
    samples = makeIntArray(n);
    for (int i = 0; i < n; i++) {
        samples[i] = trunc(amp * cos(2.00 * pi * f * i * t + phi));
    }
    printIntArray(n, samples);
    destroyArray(samples);
    return 0;
}