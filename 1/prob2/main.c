#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const double pi = 3.141592653589793238462643383279502884;

int main(int argc, char *argv[]) {
	double r, theta, x, y;
	if (!scanf("%lf %lf", &r, &theta)) {
		abort();
	}
	x = r * cos(theta);
	y = r * sin(theta);
	printf("%.2lf %.2lf\n", x, y);
	return 0;
}