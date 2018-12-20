#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const double pi = 3.141592653589793238462643383279502884;

int main(int argc, char *argv[]) {
    double f0, f1, f2, f3;
    if (!scanf("%lf %lf", &f0, &f1)) {
        abort();
    }
    f2 = abs(f0 - f1)/2.00;
    f3 = abs(f0 + f1)/2.00;
    printf("%.2lf %.2lf\n", f2, f3);
    return 0;
}