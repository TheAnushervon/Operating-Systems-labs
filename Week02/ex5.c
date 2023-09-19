#include <stdio.h>

int Tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    int t0 = 0, t1 = 1, t2 = 1, tn = 0;

    for (int i = 3; i <= n; i++) {
        tn = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }

    return tn;
}

int main() {
    int n1 = 4;
    int n2 = 36;

    printf("Tribonacci(%d) = %d\n", n1, Tribonacci(n1));
    printf("Tribonacci(%d) = %d\n", n2, Tribonacci(n2));
    

    return 0;
}
