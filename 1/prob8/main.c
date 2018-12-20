#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int gcd(int a, int b) {
    while (a != 0) {
        int r = b % a;
        b = a;
        a = r;
    }
    return b;
}

int fundamentalPeriod(int arr[], int n) {
    int ans = arr[0];

    for (int i = 1; i < n; i++) {
        ans = (arr[i] * ans) / (gcd(arr[i], ans));
    }
    return ans;
}

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

int main(int argc, char *argv[]) {
    int n;
    int *array;
    if (!scanf("%d", &n)) {
        abort();
    }
    array = makeIntArray(n);
    for (int i = 0; i < n; i++) {
        if (!scanf("%*d %d %*f", &array[i])) {
            abort();
        }
    }
    printf("%d\n", fundamentalPeriod(array, n));
    destroyArray(array);
    return 0;
}