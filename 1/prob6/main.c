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

double *makeDoubleArray(int n) {
    /* allocates dynamic double array of size/length n */
    return safeMalloc(n * sizeof(double));
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
    int samplingRate, n = 2;
    double *amp, *f, *phi;
    amp = makeDoubleArray(n);
    f = makeDoubleArray(n);
    phi = makeDoubleArray(n);
    if (!scanf("%d", &samplingRate)) {
        abort();
    }
    if (!scanf("%lf %lf %lf %lf %lf %lf", &amp[0], &f[0], &phi[0], &amp[1], &f[1], &phi[1])) {
        abort();
    }
    double t = 1.00 / samplingRate;
    double **samples = safeMalloc(n * sizeof(double *));
    samples[0] = safeMalloc(50 * sizeof(double));
    samples[1] = safeMalloc(50 * sizeof(double));
    for (int i = 0; i < 50; i++) {
        samples[0][i] = trunc(amp[0] * cos(2.00 * pi * f[0] * t * i) + phi[0]);
    }
    for (int i = 0; i < 50; i++) {
        samples[1][i] = trunc(amp[1] * cos(2.00 * pi * f[1] * t * i) + phi[1]);
    }
    int flag = 1;
    for (int i = 0; i < 50; i++) {
        if (samples[0][i] != samples[1][i]) {
            flag = 0;
            break;
        }
    }
    printf("%s\n", (flag) ? "YES" : "NO");

    destroyArray(samples[0]);
    destroyArray(samples[1]);
    destroyArray(samples);
    destroyArray(amp);
    destroyArray(f);
    destroyArray(phi);
    return 0;
}