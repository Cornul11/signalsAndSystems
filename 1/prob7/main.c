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

double *cartesianToPolar(double x, double y) {
    double *result = makeDoubleArray(2);
    double theta, thetaFinal = 0;
    result[0] = hypot(x, y);
    if (x != 0) {
        if (y != 0) {
            theta = atan(y / x);
            if ((x > 0 && y > 0) || (x == -y)) { //Q1
                thetaFinal = theta;
            } else if (x < 0 && y > 0) { //Q2
                thetaFinal = theta + pi;
            } else if (x < 0 && y < 0) { //Q3
                thetaFinal = theta - pi;
            } else if (x > 0 && y < 0) { //Q4
                thetaFinal = 2.00 * pi - theta;
            }
        }
    }
    if (x == 0) { //exceptions when no actual angle is present
        if (y > 0) {
            thetaFinal = pi / 2.00;
        } else {
            thetaFinal = -(pi / 2.00);
        }
    }
    if (y == 0) {
        if (x > 0) {
            thetaFinal = 0;
        } else {
            thetaFinal = -pi;
        }
    }
    result[1] = thetaFinal;
    return result;
}

double *polarToCartesian(double r, double theta) {
    double *result = makeDoubleArray(2);
    result[0] = r * cos(theta);
    result[1] = r * sin(theta);
    return result;
}

int main(int argc, char *argv[]) {
    int *x;
    double amp, f, phi, *observedSignal, soundSpeed = 343, timeDist1, timeDist2;
    x = makeIntArray(3);
    if (!scanf("%lf %lf %lf %d %d %d", &amp, &f, &phi, &x[0], &x[1], &x[2])) {
        abort();
    }

    timeDist1 = abs(x[2] - x[0]) / soundSpeed;
    timeDist2 = abs(x[2] - x[1]) / soundSpeed;

    double r1 = 2.0 * pi * f * timeDist1 + phi;
    double r2 = 2.0 * pi * f * timeDist2 + phi;

    double *complex0 = polarToCartesian(amp, r1);
    double *complex1 = polarToCartesian(amp, r2);

    observedSignal = cartesianToPolar(complex0[0] + complex1[0], complex0[1] + complex1[1]);

    printf("%.2lf\n", observedSignal[0]);
    destroyArray(x);
    destroyArray(complex0);
    destroyArray(complex1);
    destroyArray(observedSignal);
    return 0;
}