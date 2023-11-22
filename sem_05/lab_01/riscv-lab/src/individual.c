#include <stdio.h>

int main() {
    int len = 8;       
    int enroll = 2;
    int elem_sz = 4;

    int _x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    int x20 = len / enroll;
    int* x1 = _x;
    int x31 = 0;

    while (x20 != 0) {
        int x2 = _x[0];
        int x3 = _x[4];
        x1 += elem_sz * enroll;
        x20--;

        x31 += x2;
        x31 += x3;
    }

    x31++;

    printf("%d \n", x31);

    while (1) { }

    return 0;
}
