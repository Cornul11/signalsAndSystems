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
    int samplingRate, n, m, *samples;
    double *amp, *f, *phi;
    if (!scanf("%d %d %d", &samplingRate, &n, &m)) {
        abort();
    }
    amp = makeDoubleArray(m);
    f = makeDoubleArray(m);
    phi = makeDoubleArray(m);
    for (int i = 0; i < m; i++) {
        if (!scanf("%lf %lf %lf", &amp[i], &f[i], &phi[i])) {
            abort();
        }
    }
    double t = 1.00 / samplingRate;
    double sumOfSinusoids;
    samples = makeIntArray(n);
    for (int i = 0; i < n; i++) {
        sumOfSinusoids = 0;
        for (int j = 0; j < m; j++) {
            sumOfSinusoids += amp[j] * cos(2.00 * pi * f[j] * i * t + phi[j]);
        }
        samples[i] = trunc(sumOfSinusoids);
    }
    printIntArray(n, samples);
    destroyArray(samples);
    destroyArray(amp);
    destroyArray(f);
    destroyArray(phi);
    return 0;
}