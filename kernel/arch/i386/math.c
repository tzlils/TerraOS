#include <kernel/math.h>
int sin(int x) {
    static const int qN = 13, qA= 12, qP= 15, qR= 2*qN-qP, qS= qN+qP+1-qA;
    x = x<<(30-qN);
        if( (x^(x<<1)) < 0) 
        x= (1<<31) - x;

    x= x>>(30-qN);

    return x * ( (3<<qP) - (x*x>>qR) ) >> qS;
}

int sqrt(int n) {
        int x = n;
        for (int i = 0; i < (n/2); i++)
             x = (x + n / x) / 2;

        return x;
}

int cos(int x) {
    return sqrt(1 - (sin(x))^2);
}