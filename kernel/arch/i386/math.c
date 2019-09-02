#include <kernel/math.h>
#include <stdint.h>

// int sin(unsigned int x) {
//         // i have no fucking clue how this works
//         static const int qN = 13, qA= 12, qP= 15, qR= 2*qN-qP, qS= qN+qP+1-qA;
//         x = x<<(30-qN);
//                 if( (x^(x<<1)) < 0) 
//                 x= (1<<31) - x;

//         x= x>>(30-qN);

//         return x * ( (3<<qP) - (x*x>>qR) ) >> qS;
// }

double fabs(double x)
{
	union {double f; uint64_t i;} u = {x};
	u.i &= -1ULL/2;
	return u.f;
}

double sin (double x) {
    int i = 1;
    double cur = x;
    double acc = 1;
    double fact= 1;
    double pow = x;
    while (fabs(acc) > .00000001 &&   i < 100){
        fact *= ((2*i)*(2*i+1));
        pow *= -1 * x*x; 
        acc =  pow / fact;
        cur += acc;
        i++;
    }
    return cur;

}

float cos(float x) {
    return sqrt(1 - pow(sin(x), 2));
}

int min(int a, int b) {
        if(a > b) { return b; }
        else { return a; }
}

int max(int a, int b) {
        if(a < b) { return b; }
        else { return a; }
}

double log(unsigned int n, int b) {
        return (n > 1) ? 1 + log(n/b, b) : 0;
}

int is_even(int n) {
        return (n & 0x1) == 0;
}

int pow(int n, int p) {
        if(p == 0) return 1;
        if(is_even(p)) return pow(n * n, p/2);
        return n * pow(n * n, (p - 1) / 2);
}

double sqrt(double n) {
        return pow(n, .5);
}

float log2(float val) {
    union { float val; int32_t x; } u = { val };
    register float log_2 = (float)(((u.x >> 23) & 255) - 128);              
    u.x   &= ~(255 << 23);
    u.x   += 127 << 23;
    log_2 += ((-0.34484843f) * u.val + 2.02466578f) * u.val - 0.67487759f;
    return (log_2);
} 