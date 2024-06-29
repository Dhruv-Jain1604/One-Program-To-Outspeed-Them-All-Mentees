#include <iostream>
#include <thread>
#include <immintrin.h>
#include <chrono>
#include <bits/stdc++.h>
#define MAX_THREADS 8
using namespace std;

#define MAX 10000000
#define MIN 0.000001

/*

TASK : Given an array of floats A, return an array of probabilities B, such that B[i] = softmax(1/sqrt(A[i]))
The softmax function is defined is follows:
softmax(A)[i] = A[i]/sum_{j=0}^{j=sizeof(A)}(A[j])
The "naive" way of doing this is implemented for you

*/

float quake3Algo (float number) {

    // check out https://en.wikipedia.org/wiki/Fast_inverse_square_root

    union {
        float f;
        uint32_t i;
    } conv;

    float x2;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    conv.f  = number;
    conv.i  = 0x5f3759df - ( conv.i >> 1 ); // evil floating point bit level hacking
    conv.f  = conv.f * ( threehalfs - ( x2 * conv.f * conv.f ) );
    return conv.f;
}

void naive (float *A, float *B, int n) {
/*

INPUT : float *A, int n
OUTPUT : float *B = softmax(1/sqrt(A))
To be honest this algorithm is not quite naive, it uses the quake 3 algorithm to compute fast inverse square root in O(nlogn)
Then it computes softmax in O(n)

*/

    for (int i=0; i<n; i++) {
        B[i] = quake3Algo(A[i]); // yes u could just use math.sqrt instead but it would not be nearly this cool
    }
    float sum = 0;
    for (int i=0; i<n; i++) {
        B[i] = exp(-B[i]);
    }

    for (int i=0; i<n; i++) {
        sum += B[i];
    }

    for (int i=0; i<n; i++) {
        B[i] = B[i]/sum;
    }

}

void exp_soft(float* &B, int start, int end, float &sum){
    if (start==end) return;
    for(int p=start;p<end;p++){
        B[p]=exp(-B[p]);
        sum+=B[p];
    }
    return;
}

void div_soft(float* &B, int start, int end, float sum){
    if(start==end) return;
    for(int q=start;q<end;q++){
        B[q]/=sum;
    }
    return;
}

void optimal (float *A, float *B, int n) {

/*

STUDENT CODE BEGINS HERE, ACHIEVE A SPEEDUP OVER NAIVE IMPLEMENTATION
YOU MAY EDIT THIS FILE HOWEVER YOU WANT
HINT : USE SIMD INSTRUCTIONS, YOU MAY FIND SOMETHING BEAUTIFUL ONLINE. THEN USE MULTITHREADING FOR SOFTMAX
(Note we do not expect to see a speedup for low values of n, but for n > 100000)

*/  
    __m128 half, three_half;
    half= _mm_set1_ps(0.5F);
    three_half= _mm_set1_ps(1.5F);
     __m128i magic_number = _mm_set1_epi32(0x5f3759df);
    for(int i=0;i<n-(n%4);i+=4){
        __m128 number_float,x2;
        number_float= _mm_loadu_ps(A+i);
        x2 = _mm_mul_ps(number_float,half);
        number_float=_mm_castsi128_ps(_mm_sub_epi32(magic_number, _mm_srli_epi32(_mm_castps_si128(number_float), 1)));
        number_float = _mm_mul_ps(number_float,_mm_mul_ps(number_float,_mm_sub_ps(three_half,_mm_mul_ps(x2,_mm_mul_ps(number_float,number_float)))));
        _mm_storeu_ps(B+i,number_float);
    }
    for(int i=n-(n%4);i<n;i++){
        B[i]=quake3Algo(A[i]);
    }
    float sum=0;
    thread* T = new thread[MAX_THREADS];
    for(int j=0; j<MAX_THREADS-1;j++){
        unsigned long start=j*(n/MAX_THREADS);
        unsigned long end=(j+1)*(n/MAX_THREADS);
        T[j]=thread(exp_soft,ref(B),start,end,ref(sum));
    }
    T[MAX_THREADS-1] = thread(exp_soft,ref(B),(MAX_THREADS-1)*(n/MAX_THREADS),n,ref(sum));
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    delete[] T;
    thread* R = new thread[MAX_THREADS];
    for(int j=0; j<MAX_THREADS-1;j++){
        unsigned long start=j*(n/MAX_THREADS);
        unsigned long end=(j+1)*(n/MAX_THREADS);
        R[j]=thread(div_soft,ref(B),start,end,sum);
    }
    R[MAX_THREADS-1] = thread(div_soft,ref(B),(MAX_THREADS-1)*(n/MAX_THREADS),n,sum);
    for(int k=0;k<MAX_THREADS;k++){
        R[k].join();
    }
    delete[] R;
    //cout<<"Student code not implemented\n";
    //exit(1);

}

int main () {

    int n;
    cin>>n;
    float A[n];
    for (int i=0; i<n; i++) {
        if (i%100 != 0) {
            A[i] = MIN;
        }
        else {
            A[i] = MAX;
        }
    }
    float BNaive[n];
    float BOptim[n];

    auto startNaive = chrono::high_resolution_clock::now();
    naive(A,BNaive,n);
    auto endNaive = chrono::high_resolution_clock::now();
    auto naiveTime = chrono::duration_cast<chrono::duration<double>>(endNaive - startNaive);

    auto startOptim = chrono::high_resolution_clock::now();
    optimal(A,BOptim,n);
    auto endOptim = chrono::high_resolution_clock::now();
    auto optimTime = chrono::duration_cast<chrono::duration<double>>(endOptim - startOptim);

    cout<<"Naive answer : ";
    for (int i=0; i<n; i++) {
        cout<<BNaive[i]<<" ";
    }
    cout<<endl;
    cout<<"Optim answer : ";
    for (int i=0; i<n; i++) {
        cout<<BOptim[i]<<" ";
    }
    cout<<endl;
    cout<<"Naive time : "<<naiveTime.count()<<endl;
    cout<<"Optim time : "<<optimTime.count()<<endl;

}