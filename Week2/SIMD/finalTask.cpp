#include <iostream>
#include <immintrin.h>
#include <iomanip>
#include <chrono>
using namespace std;
/*

COMPUTE DOT PRODUCT OF TWO VECTORS
INPUTS :n => dimension of vectors
        vec1 => the first vector
        vec2 => the second vector

OUTPUT:dotProduct = vec1.vec2

dotProduct of two vectors is defined as summation_{i=0}^{i=n} [vec1[i]*vec2[i]];

*/
int main () {

    int n;
    cin>>n;
    float vec1[n];
    float vec2[n];
    for (int i=0; i<n; i++) {
        vec1[i]=i;
    }
    for (int i=0; i<n; i++) {
        vec2[i]=i;
    }
    float naiveTimeTaken = 0;
    float SIMDTimeTaken = 0;
    float dotProduct = 0;
    for (int i=0; i<1000; i++) {
        dotProduct = 0;
        auto start = chrono::high_resolution_clock::now();
        //NAIVE IMPLEMENTATION FOR COMPARISION PURPOSES
        for (int i=0; i<n; i++) {
            dotProduct += vec1[i]*vec2[i];
        }
        //NAIVE IMPLEMENTATION ENDS
        auto end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::duration<double>>(end - start);
        naiveTimeTaken += (elapsed.count())/1000;
    }

    cout<<dotProduct<<" "<<naiveTimeTaken<<" ";
    
    for (int i=0; i<1000; i++) {
        dotProduct = 0;
        auto start2 = chrono::high_resolution_clock::now();
        //STUDENT CODE BEGINS HERE
        __m256 a,b,c;
        float half[4];
        for(int i=0;i<n-(n%8);i+=8){
            a= _mm256_loadu_ps(vec1+i);
            b= _mm256_loadu_ps(vec2+i);
            c= _mm256_mul_ps(a,b);
            __m128 d= _mm256_extractf128_ps(c,0);
            __m128 e= _mm256_extractf128_ps(c,1);
            __m128 f= _mm_hadd_ps(d,e);
            __m128 g= _mm_hadd_ps(f,f);
            _mm_storeu_ps(half,g);
            dotProduct+=(half[0]+half[1]);
            
        }
        for(int i=n-(n%8);i<n;i++){
            dotProduct+=(vec1[i])*(vec2[i]);
        }

        //cout<<"STUDENT CODE IMPLEMENTED!\n";
        //exit(-1);
        //END OF STUDENT CODE
        auto end2 = chrono::high_resolution_clock::now();
        auto elapsed2 = chrono::duration_cast<chrono::duration<double>>(end2 - start2);
        SIMDTimeTaken += (elapsed2.count())/1000;
    }
    cout<<dotProduct<<" "<<SIMDTimeTaken<<" ";
}