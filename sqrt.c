#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define ERROR 1e-6
#define MAXITERATIONS 30
#define BENCHITER (int)1e7


// like stdc but no edge case checking
extern float hwsqrt(float n);
extern float heron(float n);


static inline float tomabs(float f) {
    return f >= 0? f: -f;
}

uint64_t gettime() {
    struct timespec t = {};
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e9 + t.tv_nsec;
}

double delta(uint64_t s, uint64_t e) {
    return (double)(e-s) * 1e-9;
}


/*
 *  |g*2 - t| <= E
 *  
 *  sqrt(t - E) <= g <= sqrt(t+E)
 * */
// binary search implementation
float binsqrt(float n) {
    float high = n >= 1? n: 1;
    float low = 0;
    float mid = (low+high)/2;
    size_t iter = 0;
    do {
        float msqr = mid*mid;
        if(msqr < n) {
            low = mid;
        } else if(msqr > n) {
            high = mid;
        }
        mid = (high + low)/2;
        iter++;
    } while(tomabs(mid*mid - n) > ERROR && iter < MAXITERATIONS); // until acceptable error or MAXITERATIONS reached

    return mid;
}

void printBench(char* name, float in, float out, double delta) {
    printf("%15s sqrt(%f) = %f %f s\n", name, in, out, delta);
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Number not provided\n");
        return 1;
    }

    float n = strtof(argv[1], NULL);
    if(n < 0) {
        printf("Number should be a positive real\n");
        return 1;
    }
    
    uint64_t s, e;
    volatile float sr;

    printf("LOOPS: %d\n", BENCHITER);

    s = gettime();
    for(int i = 0; i < BENCHITER; i++) {
        sr = sqrt(n);
    }
    e = gettime();
    printBench("stdc", n, sr, delta(s, e));
    
    s = gettime();
    for(int i = 0; i < BENCHITER; i++) {
        sr = binsqrt(n);
    }
    e = gettime();
    printBench("Binary Search", n, sr, delta(s, e));

    s = gettime();
    for(int i = 0; i < BENCHITER; i++) {
        sr = heron(n);
    }
    e = gettime();
    printBench("Heron's Method", n, sr, delta(s, e));

    s = gettime();
    for(int i = 0; i < BENCHITER; i++) {
        sr = hwsqrt(n);
    }
    e = gettime();
    printBench("SQRTSS", n, sr, delta(s, e));

    return 0;
}

