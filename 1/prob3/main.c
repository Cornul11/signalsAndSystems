#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const double pi = 3.141592653589793238462643383279502884;

int main(int argc, char *argv[]) {
    double f, amp0, phi0, amp1, phi1, amp, phi;
    if (!scanf("%lf %lf %lf %lf %lf", &f, &amp0, &phi0, &amp1, &phi1)) {
        abort();
    }
    amp = sqrt(pow(amp0 * cos(phi0) + amp1 * cos(phi1), 2) + pow(amp0 * sin(phi0) + amp1 * sin(phi1), 2));
    phi = atan2((amp0 * sin(phi0) + amp1 * sin(phi1)), (amp0 * cos(phi0) + amp1 * cos(phi1)));
    if (amp > 0.00001) {
        printf("x(t)=%.2lf*cos(2*pi*%.2lf*t%s%.2lf)\n", amp, f, (phi < 0 ? "" : "+"), phi);
    } else {
        printf("x(t)=%.2lf", amp);
    }
    return 0;
}