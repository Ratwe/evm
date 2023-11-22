#define len 8
#define enroll 4
#define elem_sz 4
int _x[] = { 1,2,3,4,5,6,7,8 };
void _start() {
    int x20 = len / enroll;
    int* x1 = _x;

    do {
        int x2 = x1[0];
        x31 += x2;
        x2 = x1[1];
        x31 += x2;
        x2 = x1[2];
        x31 += x2;
        x2 = x1[3];
        x31 += x2;
        x1 += enroll;
        x20--;
    } while (x20 != 0);
    x31++;
    while (1) {}
}