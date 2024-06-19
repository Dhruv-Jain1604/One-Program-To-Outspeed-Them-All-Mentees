#include "matrix.h"
#include<bits/stdc++.h>
#define Loop(i,a,b) for (int i = a ; i < b ; i++)
#define MAX_THREADS 8
using namespace std;

Matrix::Matrix(int a, int b) { // generate a matrix (2D array) of dimensions a,b
    this->n = a;
    this->m = b;
    this->M = new int*[a];
    Loop(i, 0, n) this->M[i] = new int[b];
    this->initialiseMatrix();
}

Matrix::~Matrix() { // cleanup heap memory
    Loop(i, 0, this->n) delete[] this->M[i];
    delete[] this->M;
}

void Matrix::initialiseMatrix(){ // initialise entries to 0
    Loop(i, 0, this->n) {
        Loop(j, 0, this->m) this->M[i][j] = 0;
    }
}

void Matrix::inputMatrix() { // take input
    Loop(i, 0, this->n) {
        Loop(j, 0, this->m) cin >> this->M[i][j];
    }
}

void Matrix::displayMatrix() { // print matrix
    Loop(i, 0, this->n) {
        Loop(j, 0, this->m) cout << this->M[i][j] << " ";
        cout << "\n";
    }
}
int** Matrix::T(){
    int** MT = new int*[this->m];
    Loop(i,0,m) MT[i] = new int[this->n];
    Loop(i,0,m){
        Loop(j,0,n){
            MT[i][j] = this->M[j][i];
        }
    }
    return MT;
}


void multiply_array(int thread_no, int start_element, int end_element, Matrix* P,int dim1_P,int dim2_P, Matrix* Q,int dim2_Q, Matrix* R){
    for(int k=start_element;k<=end_element;k++){
        int coord_x= k/dim2_Q;
        int coord_y=k%dim2_Q;
        int temp_sum=0;
        for(int l=0;l<dim2_P;l++){
            temp_sum+=(P->M[coord_x][l])*(Q->M[l][coord_y]);
        }
        R->set(coord_x,coord_y,temp_sum);
    }

}

Matrix* Matrix::multiplyMatrix(Matrix* N) {
    if (this->m != N->n) {
        return NULL;
    }
    Matrix *c = new Matrix(this->n,N->m);

    /*
    
    BEGIN STUDENT CODE
    INPUT : this : pointer to matrix A
            N    : pointer to matrix B

    OUTPUT : C   : pointer to matrix C = A*B

    matrix multiplication is defined as following:
    if input matrix A is a matrix of dimensions n1 by n2 and B is a matrix of dimension n2 by n3 then matrix product C = A*B is defined as
    C[i][j] = sum over k = 0 to n2-1 {A[i][k]*B[k][j]}

    */

    thread *T = new thread[MAX_THREADS];
    int *element_limit = new int[MAX_THREADS];
    for(int i=0;i<MAX_THREADS-1;i++){
        element_limit[i]=(this->n)*(N->m)/MAX_THREADS;
    }
    element_limit[MAX_THREADS-1]=(this->n)*(N->m);

    for(int i=0;i<MAX_THREADS;i++){
        if(i==0){
            T[i]=thread(multiply_array,0,0,element_limit[i]-1,this,this->n,this->m,N,N->m,c);
        }
        else {
            T[i]=thread(multiply_array,i,element_limit[i-1],element_limit[i]-1,this,this->n,this->m,N,N->m,c);
        }
    }

    for(int i=0;i<MAX_THREADS;i++){
        T[i].join();
    }

    //cout<<"STUDENT CODE NOT IMPLEMENTED!\n";
    //exit(1);
    return c;
}

