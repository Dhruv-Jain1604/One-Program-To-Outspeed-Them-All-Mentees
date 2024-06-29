#include "matrix.h"
#include<thread>
#include<bits/stdc++.h>
#include<mutex>
#include <immintrin.h>
#include <iomanip>
#define MAX_THREADS 8
using namespace std;

matrix::matrix(unsigned long rowNum, unsigned long colNum){
    data.resize(rowNum*colNum,0);
    rows = rowNum;
    cols = colNum;
}

matrix::matrix(unsigned long size){
    matrix(size,1);
}

matrix::matrix(const matrix& other) {
    data = other.data;
    rows = other.rows;
    cols = other.cols;
}

matrix& matrix::operator=(const matrix& other) {
    // Allocate new resource
    rows = other.rows;
    cols = other.cols;
    data = other.data;

    return *this;
}

matrix operator+(const matrix& first, const matrix& second){
   if (first.rows!=second.rows && first.cols!=second.cols){
        throw std::invalid_argument("Cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    else if (first.rows == second.rows && first.cols == second.cols) {
        matrix sum(first.rows,first.cols);
        unsigned long size = (first.rows)*(first.cols);
        __m256d out_operation, element1_operation, element2_operation;
        for(unsigned long k=0; k<size-(size%4);k+=4){
            element1_operation=_mm256_loadu_pd(&first.data[k]);
            element2_operation=_mm256_loadu_pd(&second.data[k]);
            out_operation = _mm256_add_pd(element1_operation,element2_operation);
            _mm256_storeu_pd(&sum.data[k], out_operation);
        }
        for(unsigned long l=size-(size%4);l<size;l++){
            sum.data[l]=first.data[l]+second.data[l];
        }
        return sum;
    }
    else if (first.rows == second.rows) {
        if (first.cols == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long row= second.rows;
            unsigned long size=second.rows*second.cols;
           // double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            vector<double> temp(first.data);
            temp.push_back(first.data[0]);
            temp.push_back(first.data[1]);
            temp.push_back(first.data[2]);
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
               //for(unsigned long p=0;p<4;p++){
                //arr_first[p]=first.data[(k+p)%row];
                //arr_second[p]=second.data[k+p];
            //}
                element1_operation=_mm256_loadu_pd(&temp[k%row]);
                element2_operation=_mm256_loadu_pd(&second.data[k]);
                out_operation = _mm256_add_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                //for(unsigned long p=0;p<4;p++){
                //sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%row]+second.data[l];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else if (second.cols == 1) {
            matrix sum(first.rows, first.cols);
            unsigned long row= first.rows;
            unsigned long size=first.rows*first.cols;
            //double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            vector<double> temp(second.data);
            temp.push_back(second.data[0]);
            temp.push_back(second.data[1]);
            temp.push_back(second.data[2]);
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                //for(unsigned long p=0;p<4;p++){
                //arr_first[p]=first.data[k+p];
                //arr_second[p]=second.data[(k+p)%row];
            //}
                
                element1_operation=_mm256_loadu_pd(&first.data[k]);
                element2_operation=_mm256_loadu_pd(&temp[k%row]);
                out_operation = _mm256_add_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                //for(unsigned long p=0;p<4;p++){
               // sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]+second.data[l%row];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    }
    else if (first.cols == second.cols) {
        if (first.rows == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long column= first.cols;
            unsigned long size=second.rows*second.cols;
            vector<double> temp(first.data);
            temp.push_back(first.data[0]);
            temp.push_back(first.data[1]);
            temp.push_back(first.data[2]);
            //double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
               // for(unsigned long p=0;p<size;p++){
                //arr_first[p]=first.data[(k+p)%column];
                //arr_second[p]=second.data[k+p];
            //}
                element1_operation=_mm256_loadu_pd(&temp[k%column]);
                element2_operation=_mm256_loadu_pd(&second.data[k]);
                out_operation = _mm256_add_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
               // for(unsigned long p=0;p<4;p++){
                //sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%column]+second.data[l];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else if (second.rows == 1) {
            matrix sum(first.rows, first.cols);
            unsigned long column= first.cols;
            unsigned long size=first.rows*first.cols;
            vector<double> temp(second.data);
            temp.push_back(second.data[0]);
            temp.push_back(second.data[1]);
            temp.push_back(second.data[2]);
            //double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            //for(unsigned long p=0;p<size;p++){
              //  arr_first[p]=first.data[p];
                //arr_second[p]=second.data[p%column];
            //}
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                //for(unsigned long p=0;p<size;p++){
                //arr_first[p]=first.data[k+p];
                //arr_second[p]=second.data[(k+p)%column];
            //}
                element1_operation=_mm256_loadu_pd(&first.data[k]);
                element2_operation=_mm256_loadu_pd(&temp[k%column]);
                out_operation = _mm256_add_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                //for(unsigned long p=0;p<4;p++){
                //sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]+second.data[l%column];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    }
    return zeros(1,1);
}

matrix operator-(const matrix& first, const matrix& second){
    if (first.rows!=second.rows && first.cols!=second.cols){
        throw std::invalid_argument("Cannot subtract ( "+ to_string(second.rows) +" , " + to_string(second.cols) + " ) from ( " + to_string(first.rows) + " , " + to_string(first.cols) + " )" );
        }
    else if (first.rows == second.rows && first.cols == second.cols) {
        matrix sum(first.rows,first.cols);
        unsigned long size = (first.rows)*(first.cols);
        __m256d out_operation, element1_operation, element2_operation;
        for(unsigned long k=0; k<size-(size%4);k+=4){
            element1_operation=_mm256_loadu_pd(&first.data[k]);
            element2_operation=_mm256_loadu_pd(&second.data[k]);
            out_operation = _mm256_sub_pd(element1_operation,element2_operation);
            _mm256_storeu_pd(&sum.data[k], out_operation);
        }
        for(unsigned long l=size-(size%4);l<size;l++){
            sum.data[l]=first.data[l]-second.data[l];
        }
        return sum;
    }
    else if (first.rows == second.rows) {
        if (first.cols == 1) { // np.broadcast
           matrix sum(second.rows, second.cols);
            unsigned long row= second.rows;
            unsigned long size=second.rows*second.cols;
           // double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            vector<double> temp(first.data);
            temp.push_back(first.data[0]);
            temp.push_back(first.data[1]);
            temp.push_back(first.data[2]);
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
               //for(unsigned long p=0;p<4;p++){
                //arr_first[p]=first.data[(k+p)%row];
                //arr_second[p]=second.data[k+p];
            //}
                element1_operation=_mm256_loadu_pd(&temp[k%row]);
                element2_operation=_mm256_loadu_pd(&second.data[k]);
                out_operation = _mm256_sub_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                //for(unsigned long p=0;p<4;p++){
                //sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%row]-second.data[l];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else if (second.cols == 1) {
            matrix sum(first.rows, first.cols);
            unsigned long row= first.rows;
            unsigned long size=first.rows*first.cols;
            //double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            vector<double> temp(second.data);
            temp.push_back(second.data[0]);
            temp.push_back(second.data[1]);
            temp.push_back(second.data[2]);
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                //for(unsigned long p=0;p<4;p++){
                //arr_first[p]=first.data[k+p];
                //arr_second[p]=second.data[(k+p)%row];
            //}
                
                element1_operation=_mm256_loadu_pd(&first.data[k]);
                element2_operation=_mm256_loadu_pd(&temp[k%row]);
                out_operation = _mm256_sub_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                //for(unsigned long p=0;p<4;p++){
               // sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]-second.data[l%row];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot subtract ( "+ to_string(second.rows) +" , " + to_string(second.cols) + " ) from ( " + to_string(first.rows) + " , " + to_string(first.cols) + " )" );
        }
    }
    else if (first.cols == second.cols) {
        if (first.rows == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long column= first.cols;
            unsigned long size=second.rows*second.cols;
            vector<double> temp(first.data);
            temp.push_back(first.data[0]);
            temp.push_back(first.data[1]);
            temp.push_back(first.data[2]);
            //double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
               // for(unsigned long p=0;p<size;p++){
                //arr_first[p]=first.data[(k+p)%column];
                //arr_second[p]=second.data[k+p];
            //}
                element1_operation=_mm256_loadu_pd(&temp[k%column]);
                element2_operation=_mm256_loadu_pd(&second.data[k]);
                out_operation = _mm256_sub_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
               // for(unsigned long p=0;p<4;p++){
                //sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%column]-second.data[l];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else if (second.rows == 1) {
           matrix sum(first.rows, first.cols);
            unsigned long column= first.cols;
            unsigned long size=first.rows*first.cols;
            vector<double> temp(second.data);
            temp.push_back(second.data[0]);
            temp.push_back(second.data[1]);
            temp.push_back(second.data[2]);
            //double* arr_first= new double[4];
            //double* arr_second = new double[4];
            //double* array_final = new double[4];
            //for(unsigned long p=0;p<size;p++){
              //  arr_first[p]=first.data[p];
                //arr_second[p]=second.data[p%column];
            //}
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                //for(unsigned long p=0;p<size;p++){
                //arr_first[p]=first.data[k+p];
                //arr_second[p]=second.data[(k+p)%column];
            //}
                element1_operation=_mm256_loadu_pd(&first.data[k]);
                element2_operation=_mm256_loadu_pd(&temp[k%column]);
                out_operation = _mm256_sub_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                //for(unsigned long p=0;p<4;p++){
                //sum.data[p+k]=array_final[p];
            //}
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]-second.data[l%column];
            }
            //delete[] arr_first;
            //delete[] arr_second;
            //delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot subtract ( "+ to_string(second.rows) +" , " + to_string(second.cols) + " ) from ( " + to_string(first.rows) + " , " + to_string(first.cols) + " )" );
        }
    }
    return zeros(1,1);
}

matrix operator*(const matrix& first, const matrix& second){
    if (first.rows!=second.rows && first.cols!=second.cols){
        throw std::invalid_argument("Cannot multiply ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    else if (first.rows == second.rows && first.cols == second.cols) {
        matrix sum(first.rows,first.cols);
        unsigned long size = (first.rows)*(first.cols);
        __m256d out_operation, element1_operation, element2_operation;
        for(unsigned long k=0; k<size-(size%4);k+=4){
            element1_operation=_mm256_loadu_pd(&first.data[k]);
            element2_operation=_mm256_loadu_pd(&second.data[k]);
            out_operation = _mm256_mul_pd(element1_operation,element2_operation);
            _mm256_storeu_pd(&sum.data[k], out_operation);
        }
        for(unsigned long l=size-(size%4);l<size;l++){
            sum.data[l]=first.data[l]*second.data[l];
        }
        return sum;
    }
    else if (first.rows == second.rows) {
        if (first.cols == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long row= second.rows;
            unsigned long size=second.rows*second.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
               for(unsigned long p=0;p<4;p++){
                arr_first[p]=first.data[(k+p)%row];
                arr_second[p]=second.data[k+p];
            }
                element1_operation=_mm256_loadu_pd(arr_first);
                element2_operation=_mm256_loadu_pd(arr_second);
                out_operation = _mm256_mul_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(array_final, out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%row]*second.data[l];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else if (second.cols == 1) {
            matrix sum(first.rows, first.cols);
            unsigned long row= first.rows;
            unsigned long size=first.rows*first.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                for(unsigned long p=0;p<4;p++){
                arr_first[p]=first.data[k+p];
                arr_second[p]=second.data[(k+p)%row];
            }
                element1_operation=_mm256_loadu_pd(&first.data[k]);
                element2_operation=_mm256_loadu_pd(&second.data[k%row]);
                out_operation = _mm256_mul_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]*second.data[l%row];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot multiply ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    }
    else if (first.cols == second.cols) {
        if (first.rows == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long column= first.cols;
            unsigned long size=second.rows*second.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                for(unsigned long p=0;p<size;p++){
                arr_first[p]=first.data[(k+p)%column];
                arr_second[p]=second.data[k+p];
            }
                element1_operation=_mm256_loadu_pd(arr_first);
                element2_operation=_mm256_loadu_pd(arr_second);
                out_operation = _mm256_mul_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(array_final, out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%column]*second.data[l];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else if (second.rows == 1) {
             matrix sum(first.rows, first.cols);
            unsigned long column= first.cols;
            unsigned long size=first.rows*first.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            for(unsigned long p=0;p<size;p++){
                arr_first[p]=first.data[p];
                arr_second[p]=second.data[p%column];
            }
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                for(unsigned long p=0;p<size;p++){
                arr_first[p]=first.data[k+p];
                arr_second[p]=second.data[(k+p)%column];
            }
                element1_operation=_mm256_loadu_pd(arr_first);
                element2_operation=_mm256_loadu_pd(arr_second);
                out_operation = _mm256_mul_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(array_final, out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]*second.data[l%column];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot multiply ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    }
    return zeros(1,1);
}

matrix operator/(const matrix& first, const matrix& second){
    if (first.rows!=second.rows && first.cols!=second.cols){
        throw std::invalid_argument("Cannot divide ( "+ to_string(second.rows) +" , " + to_string(second.cols) + " ) from ( " + to_string(first.rows) + " , " + to_string(first.cols) + " )" );
        }
    else if (first.rows == second.rows && first.cols == second.cols) {
        matrix sum(first.rows,first.cols);
        unsigned long size = (first.rows)*(first.cols);
        __m256d out_operation, element1_operation, element2_operation;
        for(unsigned long k=0; k<size-(size%4);k+=4){
            element1_operation=_mm256_loadu_pd(&first.data[k]);
            element2_operation=_mm256_loadu_pd(&second.data[k]);
            out_operation = _mm256_div_pd(element1_operation,element2_operation);
            _mm256_storeu_pd(&sum.data[k], out_operation);
        }
        for(unsigned long l=size-(size%4);l<size;l++){
            sum.data[l]=first.data[l]/second.data[l];
        }
        return sum;
    }
    else if (first.rows == second.rows) {
        if (first.cols == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long row= second.rows;
            unsigned long size=second.rows*second.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
               for(unsigned long p=0;p<4;p++){
                arr_first[p]=first.data[(k+p)%row];
                arr_second[p]=second.data[k+p];
            }
                element1_operation=_mm256_loadu_pd(arr_first);
                element2_operation=_mm256_loadu_pd(arr_second);
                out_operation = _mm256_div_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(array_final, out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%row]/second.data[l];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else if (second.cols == 1) {
            matrix sum(first.rows, first.cols);
            unsigned long row= first.rows;
            unsigned long size=first.rows*first.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                for(unsigned long p=0;p<4;p++){
                arr_first[p]=first.data[k+p];
                arr_second[p]=second.data[(k+p)%row];
            }
                element1_operation=_mm256_loadu_pd(&first.data[k]);
                element2_operation=_mm256_loadu_pd(&second.data[k%row]);
                out_operation = _mm256_div_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(&sum.data[k], out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]/second.data[l%row];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot divide ( "+ to_string(second.rows) +" , " + to_string(second.cols) + " ) from ( " + to_string(first.rows) + " , " + to_string(first.cols) + " )" );
        }
    }
    else if (first.cols == second.cols) {
        if (first.rows == 1) { // np.broadcast
            matrix sum(second.rows, second.cols);
            unsigned long column= first.cols;
            unsigned long size=second.rows*second.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                for(unsigned long p=0;p<size;p++){
                arr_first[p]=first.data[(k+p)%column];
                arr_second[p]=second.data[k+p];
            }
                element1_operation=_mm256_loadu_pd(arr_first);
                element2_operation=_mm256_loadu_pd(arr_second);
                out_operation = _mm256_div_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(array_final, out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l%column]/second.data[l];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else if (second.rows == 1) {
            matrix sum(first.rows, first.cols);
            unsigned long column= first.cols;
            unsigned long size=first.rows*first.cols;
            double* arr_first= new double[4];
            double* arr_second = new double[4];
            double* array_final = new double[4];
            for(unsigned long p=0;p<size;p++){
                arr_first[p]=first.data[p];
                arr_second[p]=second.data[p%column];
            }
            __m256d out_operation, element1_operation, element2_operation;
            for(unsigned long k=0; k<size-(size%4);k+=4){
                for(unsigned long p=0;p<size;p++){
                arr_first[p]=first.data[k+p];
                arr_second[p]=second.data[(k+p)%column];
            }
                element1_operation=_mm256_loadu_pd(arr_first);
                element2_operation=_mm256_loadu_pd(arr_second);
                out_operation = _mm256_div_pd(element1_operation,element2_operation);
                _mm256_storeu_pd(array_final, out_operation);
                for(unsigned long p=0;p<4;p++){
                sum.data[p+k]=array_final[p];
            }
            }
            for(unsigned long l=size-(size%4);l<size;l++){
                sum.data[l]=first.data[l]/second.data[l%column];
            }
            delete[] arr_first;
            delete[] arr_second;
            delete[] array_final;
            return sum;
        }
        else {
            throw std::invalid_argument("Cannot divide ( "+ to_string(second.rows) +" , " + to_string(second.cols) + " ) from ( " + to_string(first.rows) + " , " + to_string(first.cols) + " )" );
        }
    }
    return zeros(1,1);
}

matrix operator*(const matrix& first, const double t) {
    matrix product(first.rows, first.cols);
    unsigned long size = (first.rows)*(first.cols);
    double arr_original[size], array_final[size];
    for(unsigned long p=0;p<size;p++){
        arr_original[p]=first.data[p];
    }
    __m256d constant_operation, element_operation;
    constant_operation= _mm256_set1_pd(t);
    double temp_arr[4];
    for(unsigned long k=0; k<size-(size%4);k+=4){
        element_operation=_mm256_loadu_pd(arr_original+k);
        element_operation = _mm256_mul_pd(element_operation,constant_operation);
        _mm256_storeu_pd(array_final+k, element_operation);
    }
    for(unsigned q=0;q<size-(size%4);q++){
        product.data[q]=array_final[q];
    }
    for(unsigned long l=size-(size%4);l<size;l++){
        product.data[l]=first.data[l]*t;
    }
    return product;
}
/*
void const_add(matrix &m1, matrix &result, unsigned long start, unsigned long end, double t){
    if(start==end) return;
    __m256d constant_add, element_add;
    constant_add= _mm256_set1_pd(t);

    for(unsigned long p=start; p< end- ((end-start)%4);p+=4){
    
        double add_load[4];
        for(int s=0;s<4;s++){
            add_load[s]=m1.data[p+s];
        }
        element_add = _mm256_loadu_pd(add_load);
    
        element_add= _mm256_add_pd(element_add,constant_add);
        _mm256_storeu_pd(add_load,element_add);
        for(int u=0;u<4;u++){
            result.data[p+u]=add_load[u];
        }
    }
    for(unsigned long v=end- ((end-start)%4);v<end;v++){
        result.data[v]=m1.data[v]+t;
    }
    return;
}


void const_add(matrix &m1, matrix &result, unsigned long start, unsigned long end, double t){
    if(start==end) return;
    for(unsigned long z=start;z<end;z++){
        result.data[z]=m1.data[z]+t;
    }
    return;
}

matrix operator+(const matrix& first, const double t) {
    double adder = t;
    matrix sum(first.rows, first.cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long size=first.rows*first.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(size/MAX_THREADS);
        unsigned long end=(j+1)*(size/MAX_THREADS);
        T[j] = thread(const_add, ref(first),ref(sum),start,end,adder);
    }
    unsigned long end2 = (MAX_THREADS-1)*(size/MAX_THREADS);
    T[MAX_THREADS-1]= thread(const_add, ref(first),ref(sum),end2 ,size,adder);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return sum;
}
*/

matrix operator+(const matrix& first, const double t) {
    matrix sum(first.rows, first.cols);
    unsigned long size = (first.rows)*(first.cols);
    double arr_original[size], array_final[size];
    for(unsigned long p=0;p<size;p++){
        arr_original[p]=first.data[p];
    }
    __m256d constant_operation, element_operation;
    constant_operation= _mm256_set1_pd(t);
    double temp_arr[4];
    for(unsigned long k=0; k<size-(size%4);k+=4){
        element_operation=_mm256_loadu_pd(arr_original+k);
        element_operation = _mm256_add_pd(element_operation,constant_operation);
        _mm256_storeu_pd(array_final+k, element_operation);
    }
    for(unsigned q=0;q<size-(size%4);q++){
        sum.data[q]=array_final[q];
    }
    for(unsigned long l=size-(size%4);l<size;l++){
        sum.data[l]=first.data[l]+t;
    }
    return sum;
}

matrix operator-(const matrix& first, const double t) {
    matrix difference(first.rows, first.cols);
    unsigned long size = (first.rows)*(first.cols);
    double arr_original[size], array_final[size];
    for(unsigned long p=0;p<size;p++){
        arr_original[p]=first.data[p];
    }
    __m256d constant_operation, element_operation;
    constant_operation= _mm256_set1_pd(t);
    double temp_arr[4];
    for(unsigned long k=0; k<size-(size%4);k+=4){
        element_operation=_mm256_loadu_pd(arr_original+k);
        element_operation = _mm256_sub_pd(element_operation,constant_operation);
        _mm256_storeu_pd(array_final+k, element_operation);
    }
    for(unsigned q=0;q<size-(size%4);q++){
        difference.data[q]=array_final[q];
    }
    for(unsigned long l=size-(size%4);l<size;l++){
        difference.data[l]=first.data[l]-t;
    }
    return difference;
}

matrix operator/(const matrix& first, const double t) {
    matrix quotient(first.rows, first.cols);
    unsigned long size = (first.rows)*(first.cols);
    double arr_original[size], array_final[size];
    for(unsigned long p=0;p<size;p++){
        arr_original[p]=first.data[p];
    }
    __m256d constant_operation, element_operation;
    constant_operation= _mm256_set1_pd(t);
    double temp_arr[4];
    for(unsigned long k=0; k<size-(size%4);k+=4){
        element_operation=_mm256_loadu_pd(arr_original+k);
        element_operation = _mm256_div_pd(element_operation,constant_operation);
        _mm256_storeu_pd(array_final+k, element_operation);
    }
    for(unsigned q=0;q<size-(size%4);q++){
        quotient.data[q]=array_final[q];
    }
    for(unsigned long l=size-(size%4);l<size;l++){
        quotient.data[l]=first.data[l]/t;
    }
    return quotient;
}

matrix matmul(const matrix& first, const matrix& second){
    pair<unsigned long, unsigned long> dim1 = first.shape();
    pair<unsigned long, unsigned long> dim2 = second.shape();
    if( dim1.second != dim2.first){
        throw std::invalid_argument("cannot matmul ( "+ to_string(dim1.first) +" , " + to_string(dim1.second) + " ) with ( " + to_string(dim2.first) + " , " + to_string(dim2.second) + " )" );
    }
    else{
        matrix net(dim1.first,dim2.second);
        for( unsigned long i=0;i< dim1.first;i++){
            for(unsigned long j=0;j< dim2.second;j++){
                double sum=0;
                for(unsigned long k=0;k< dim1.second;k++){
                    sum+=first(i,k)*second(k,j);
                }
                net(i,j)=sum;
            }
        }
        return net;
    }
}

matrix zeros(unsigned long rows, unsigned long cols){
    return matrix(rows,cols);
}

matrix zeros(unsigned long size){
    return matrix(size);
}


void mlt_thread_eye(matrix &m1,  unsigned long start, unsigned long end){
        if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m1(i,i)=1;
        }
        return;
        
}

matrix eye(unsigned long size){
    matrix diag(size,size);
    thread* T = new thread[MAX_THREADS];
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(size/MAX_THREADS);
        unsigned long end=(j+1)*(size/MAX_THREADS);
        T[j]=thread(mlt_thread_eye, ref(diag),start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_eye, ref(diag),(MAX_THREADS-1)*(size/MAX_THREADS) ,size);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return diag;
}

matrix eye(unsigned long rows, unsigned long cols){
    matrix diag(rows,cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long size= min(rows,cols);
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(size/MAX_THREADS);
        unsigned long end=(j+1)*(size/MAX_THREADS);
        T[j]=thread(mlt_thread_eye, ref(diag),start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_eye, ref(diag),(MAX_THREADS-1)*(size/MAX_THREADS) ,size);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return diag;
}


matrix identity(unsigned long size){
    return eye(size);
}

//mutex n;

void mlt_max_specific_0(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //n.lock();
    for(unsigned long no_col=start; no_col<end;no_col++){
        double max_value=orig(0,no_col);
        for(unsigned long no_row=1;no_row<orig.rows;no_row++){
            if(orig(no_row,no_col)>max_value){
                max_value=orig(no_row,no_col);
            }
        }
        result(0,no_col)=max_value;
    }
    //n.unlock();
    return;
}


void mlt_max_specific_1(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //n.lock();
    for(unsigned long no_row=start; no_row<end;no_row++){
        double max_value=orig(no_row,0);
        for(unsigned long no_col=1;no_col<orig.cols;no_col++){
            if(orig(no_row,no_col)>max_value){
                max_value=orig(no_row,no_col);
            }
        }
        result(no_row,0)=max_value;
    }
    //n.unlock();
    return;
}


matrix max(matrix &arr,int axis) {
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);

    if (axis == 0) {  //largest each col 
        thread *T = new thread[MAX_THREADS];
        unsigned long total_columns= result.cols;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_columns/MAX_THREADS);
            unsigned long end=(j+1)*(total_columns/MAX_THREADS);
            T[j]=thread(mlt_max_specific_0, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_max_specific_0, ref(arr),ref(result),(MAX_THREADS-1)*(total_columns/MAX_THREADS) ,total_columns);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
        delete[] T;

    } else {  //largest for each row
        thread *T = new thread[MAX_THREADS];
        unsigned long total_rows= result.rows;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_rows/MAX_THREADS);
            unsigned long end=(j+1)*(total_rows/MAX_THREADS);
            T[j]=thread(mlt_max_specific_1, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_max_specific_1, ref(arr),ref(result),(MAX_THREADS-1)*(total_rows/MAX_THREADS) ,total_rows);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
        delete[] T;
    }
    return result;
}

void mlt_argmax_specific_0(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //n.lock();
    for(unsigned long no_col=start; no_col<end;no_col++){
        double max_value=orig(0,no_col);
        unsigned long max_index =0;
        for(unsigned long no_row=1;no_row<orig.rows;no_row++){
            if(orig(no_row,no_col)>max_value){
                max_value=orig(no_row,no_col);
                max_index=no_row;
            }
        }
        result(0,no_col)=max_index;
    }
    //n.unlock();
    return;
}


void mlt_argmax_specific_1(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //n.lock();
    for(unsigned long no_row=start; no_row<end;no_row++){
        double max_value=orig(no_row,0);
        unsigned long max_index =0;
        for(unsigned long no_col=1;no_col<orig.cols;no_col++){
            if(orig(no_row,no_col)>max_value){
                max_value=orig(no_row,no_col);
                max_index=no_col;
            }
        }
        result(no_row,0)=max_index;
    }
    //n.unlock();
    return;
}

matrix argmax(matrix &arr,int axis) {

    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);

    if (axis == 0) {  // Argmax along columns (resulting in row vector)
         thread *T = new thread[MAX_THREADS];
        unsigned long total_columns= result.cols;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_columns/MAX_THREADS);
            unsigned long end=(j+1)*(total_columns/MAX_THREADS);
            T[j]=thread(mlt_argmax_specific_0, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_argmax_specific_0, ref(arr),ref(result),(MAX_THREADS-1)*(total_columns/MAX_THREADS) ,total_columns);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
        delete[] T;
    } else {  // Argmax along rows (resulting in column vector)
        thread *T = new thread[MAX_THREADS];
        unsigned long total_rows= result.rows;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_rows/MAX_THREADS);
            unsigned long end=(j+1)*(total_rows/MAX_THREADS);
            T[j]=thread(mlt_argmax_specific_1, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_argmax_specific_1, ref(arr),ref(result),(MAX_THREADS-1)*(total_rows/MAX_THREADS) ,total_rows);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
        delete[] T;
    }

    return result;
}

matrix max (matrix &arr) {
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
    if (arrRows != 1 && arrCols != 1) {
        return max(arr,0);
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long col = 1; col < arrCols; ++col) {
            t(0,0) = max(t(0,0),arr(0,col));
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long row = 1; row < arrRows; ++row) {
            t(0,0) = max(t(0,0),arr(row,0));
        }
        return t;
    }

}


matrix argmax (matrix &arr) {
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
    if (arrRows != 1 && arrCols != 1) {
        return argmax(arr,0);
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long col = 1; col < arrCols; ++col) {
            if (arr(0,t(0,0)) < arr(col)) {
                t(0,0) = col;
            }
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long row = 1; row < arrRows; ++row) {
            if (arr(t(0,0),0) < arr(row)) {
                t(0,0) = row;
            }
        }
        return t;
    }

}

//mutex m;

void mlt_min_specific_0(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //m.lock();
    for(unsigned long no_col=start; no_col<end;no_col++){
        double max_value=orig(0,no_col);
        for(unsigned long no_row=1;no_row<orig.rows;no_row++){
            if(orig(no_row,no_col)<max_value){
                max_value=orig(no_row,no_col);
            }
        }
        result(0,no_col)=max_value;
    }
    //m.unlock();
    return;
}


void mlt_min_specific_1(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //m.lock();
    for(unsigned long no_row=start; no_row<end;no_row++){
        double max_value=orig(no_row,0);
        for(unsigned long no_col=1;no_col<orig.cols;no_col++){
            if(orig(no_row,no_col)<max_value){
                max_value=orig(no_row,no_col);
            }
        }
        result(no_row,0)=max_value;
    }
    //m.unlock();
    return;
}

matrix min(matrix &arr,int axis) {
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);

    if (axis == 0) {  //largest each col 
        thread *T = new thread[MAX_THREADS];
        unsigned long total_columns= result.cols;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_columns/MAX_THREADS);
            unsigned long end=(j+1)*(total_columns/MAX_THREADS);
            T[j]=thread(mlt_min_specific_0, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_min_specific_0, ref(arr),ref(result),(MAX_THREADS-1)*(total_columns/MAX_THREADS) ,total_columns);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
    } else {  //largest for each row
        thread *T = new thread[MAX_THREADS];
        unsigned long total_rows= result.rows;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_rows/MAX_THREADS);
            unsigned long end=(j+1)*(total_rows/MAX_THREADS);
            T[j]=thread(mlt_min_specific_1, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_min_specific_1, ref(arr),ref(result),(MAX_THREADS-1)*(total_rows/MAX_THREADS) ,total_rows);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
    }
    return result;
}

void mlt_argmin_specific_0(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //m.lock();
    for(unsigned long no_col=start; no_col<end;no_col++){
        double max_value=orig(0,no_col);
        unsigned long max_index=0;
        for(unsigned long no_row=1;no_row<orig.rows;no_row++){
            if(orig(no_row,no_col)<max_value){
                max_value=orig(no_row,no_col);
                max_index = no_row;
            }
        }
        result(0,no_col)=max_index;
    }
    //m.unlock();
    return;
}


void mlt_argmin_specific_1(matrix &orig, matrix &result, unsigned long start, unsigned long end ){
    if(start==end) return;
    //m.lock();
    for(unsigned long no_row=start; no_row<end;no_row++){
        double max_value=orig(no_row,0);
        unsigned long max_index=0;
        for(unsigned long no_col=1;no_col<orig.cols;no_col++){
            if(orig(no_row,no_col)<max_value){
                max_value=orig(no_row,no_col);
                max_index= no_col;
            }
        }
        result(no_row,0)=max_index;
    }
    //m.unlock();
    return;
}

matrix argmin(matrix &arr,int axis) {
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);

    if (axis == 0) {  // Argmax along columns (resulting in row vector)
        thread *T = new thread[MAX_THREADS];
        unsigned long total_columns= result.cols;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_columns/MAX_THREADS);
            unsigned long end=(j+1)*(total_columns/MAX_THREADS);
            T[j]=thread(mlt_argmin_specific_0, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_argmin_specific_0, ref(arr),ref(result),(MAX_THREADS-1)*(total_columns/MAX_THREADS) ,total_columns);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
    } else {  // Argmax along rows (resulting in column vector)
       thread *T = new thread[MAX_THREADS];
        unsigned long total_rows= result.rows;
        for(int j=0;j<MAX_THREADS-1;j++){
            unsigned long start=j*(total_rows/MAX_THREADS);
            unsigned long end=(j+1)*(total_rows/MAX_THREADS);
            T[j]=thread(mlt_argmin_specific_1, ref(arr),ref(result),start,end);
        }
         T[MAX_THREADS-1]= thread(mlt_argmin_specific_1, ref(arr),ref(result),(MAX_THREADS-1)*(total_rows/MAX_THREADS) ,total_rows);
        for(int k=0;k<MAX_THREADS;k++){
            T[k].join();
        }
    }

    return result;
}

matrix min (matrix &arr) {
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
    if (arrRows != 1 && arrCols != 1) {
       return min(arr,0);
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long col = 1; col < arrCols; ++col) {
            t(0,0) = min(t(0,0),arr(0,col));
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long row = 1; row < arrRows; ++row) {
            t(0,0) = min(t(0,0),arr(row,0));
        }
        return t;
    }

}


matrix argmin (matrix &arr) {
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
    if (arrRows != 1 && arrCols != 1) {
        return argmin(arr,0);
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long col = 1; col < arrCols; ++col) {
            if (arr(0,t(0,0)) > arr(col)) {
                t(0,0) = col;
            }
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long row = 1; row < arrRows; ++row) {
            if (arr(t(0,0),0) > arr(row)) {
                t(0,0) = row;
            }
        }
        return t;
    }

}

void mlt_thread_ones(matrix &m1,  unsigned long start, unsigned long end){
        if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m1.data[i]=1;
        }
        return;
        
}

matrix ones (unsigned long rows, unsigned long cols) {
    matrix t(rows,cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = t.rows*t.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_thread_ones, ref(t),start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_ones, ref(t),(MAX_THREADS-1)*(no_elements/MAX_THREADS), no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return t;
}

void mlt_thread_fabs(matrix &m1, matrix &m2, unsigned long start, unsigned long end){
        if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m2.data[i]=fabs(m1.data[i]);
        }
        return;
        
}

void mlt_thread_exp(matrix &m1, matrix &m2, unsigned long start, unsigned long end){
        if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m2.data[i]=exp(m1.data[i]);
        }
        return;
}

void mlt_thread_log(matrix &m1, matrix &m2, unsigned long start, unsigned long end, double logbase){
        if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m2.data[i]=log(m1.data[i])/log(logbase);
        }
        return;
}

void mlt_thread_sqrt(matrix &m1, matrix &m2, unsigned long start, unsigned long end){
        if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m2.data[i]=sqrt(m1.data[i]);
        }
        return;
}

void mlt_thread_tanh(matrix &m1, matrix &m2, unsigned long start, unsigned long end){
       if(start==end) {
            return;
        }
        for(unsigned long i=start;i<end;i++){
            m2.data[i]=tanh(m1.data[i]);
        }
        return;
}

matrix fabs(matrix &a) {
    matrix res(a.rows,a.cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = a.rows*a.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_thread_fabs, ref(a), ref(res), start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_fabs, ref(a), ref(res),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return res;
}

matrix exp(matrix &a) {
    matrix res(a.rows,a.cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = a.rows*a.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_thread_exp, ref(a), ref(res), start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_exp, ref(a), ref(res),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return res;;
}

matrix tanh(matrix &a) {
    matrix res(a.rows,a.cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = a.rows*a.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_thread_tanh, ref(a), ref(res), start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_tanh, ref(a), ref(res),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }

    return res;
}

matrix log(matrix &a, double logbase) {
    matrix res(a.rows,a.cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = a.rows*a.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_thread_log, ref(a), ref(res), start,end,logbase);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_log, ref(a), ref(res),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements,logbase);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    return res;
}

matrix sqrt(matrix &a) {
    matrix res(a.rows,a.cols);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = a.rows*a.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_thread_sqrt, ref(a), ref(res), start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_thread_sqrt, ref(a), ref(res),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }

    return res;
}

void mlt_extract_inverse(matrix &result, matrix &augmented, unsigned long start, unsigned long end){
    if (start==end) return;
    unsigned long rows_number = result.rows;
    for(unsigned long counter=start; counter<end;counter++){
        result.data[counter] = augmented.data[counter+ rows_number+(counter/rows_number)*rows_number];
    }
    return;

}

matrix matrix::inverse(){
    matrix a = *this;
    pair<unsigned long, unsigned long> dim = a.shape();
    if (dim.first != dim.second) 
        throw std::invalid_argument("Cannot invert ( "+ to_string(dim.first) +" , " + to_string(dim.second) + " )");
    unsigned long n = a.rows;
    matrix augmented(n, 2 * n);
    // Initialize the augmented matrix with the identity matrix on the right
    for (unsigned long i = 0; i < n; ++i) {
        for (unsigned long j = 0; j < n; ++j) {
            augmented(i, j) = a(i, j);
            augmented(i, j + n) = (i == j) ? 1.0 : 0.0;
        }
    }
    // Perform Gauss-Jordan elimination
    for (unsigned long i = 0; i < n; ++i) {
        // Find the pivot
        double pivot = augmented(i, i);
        if (pivot == 0.0) {
            throw runtime_error("Matrix is singular and cannot be inverted.");
        }

        // Normalize the pivot row
        __m256d pivot_register,temp_div;
        pivot_register= _mm256_set1_pd(pivot);
        for(unsigned long k=0;k<(2*n)-((2*n)%4);k+=4){
            temp_div=_mm256_loadu_pd(&augmented(i,k));
            temp_div=_mm256_div_pd(temp_div,pivot_register);
            _mm256_storeu_pd(&augmented(i,k),temp_div);
        }
        for(unsigned long l=(2*n)-((2*n)%4);l<(2*n);l++){
            augmented(i,l)/=pivot;
        }

        // Eliminate the current column in other rows
        for (unsigned long k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented(k, i);
                __m256d factor_register, sub_upl_register,final_register;
                factor_register= _mm256_set1_pd(factor);
                for(unsigned long x=0;x<(2*n)-((2*n)%4);x+=4){
                    sub_upl_register=_mm256_loadu_pd(&augmented(i,x));
                    final_register=_mm256_loadu_pd(&augmented(k,x));
                    sub_upl_register=_mm256_mul_pd(factor_register,sub_upl_register);
                    final_register = _mm256_sub_pd(final_register,sub_upl_register);
                    _mm256_storeu_pd(&augmented(k,x),final_register);
                }
                for (unsigned long j =(2*n)-((2*n)%4) ; j < 2 * n; ++j) {
                    augmented(k, j) -= factor * augmented(i, j);
                }
            }
        }
    }
    // Extract the inverse matrix from the augmented matrix
    matrix result(n, n);
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = result.rows*result.cols;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_extract_inverse, ref(result), ref(augmented), start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_extract_inverse, ref(result), ref(augmented),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    delete[] T;
   /*
   for (unsigned long i = 0; i < n; ++i) {
        for (unsigned long j = 0; j < n; ++j) {
            result(i, j) = augmented(i, j + n);
        }
    }*/
    return result;

}

void mlt_transpose_thread(matrix &m1, matrix &tm, unsigned long start, unsigned long end){
    unsigned long cols=m1.cols;
    for(unsigned p=start;p<end;p++){
        unsigned long i=p/cols;
        unsigned long j=p%cols;
        tm(j,i) = m1.data[i*cols+j];
    }
    return;
}

matrix matrix::transpose(){
    pair<unsigned long,unsigned long> dim = this->shape();
    matrix T(dim.second,dim.first);
    thread* Thread = new thread[MAX_THREADS];
    unsigned long size=dim.first*dim.second;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(size/MAX_THREADS);
        unsigned long end=(j+1)*(size/MAX_THREADS);
        Thread[j]=thread(mlt_transpose_thread, ref(*this), ref(T), start,end);
    }
    Thread[MAX_THREADS-1]= thread(mlt_transpose_thread, ref(*this), ref(T),(MAX_THREADS-1)*(size/MAX_THREADS) ,size);
    for(int k=0;k<MAX_THREADS;k++){
        Thread[k].join();
    }
    delete[] Thread;
    return T;
}

void mlt_mul_det(matrix &m1, double &det,unsigned long start, unsigned long end){
    for(unsigned long p=start;p<end;p++){
        det*=m1(p,p);
    }
}


double matrix::determinant(){
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square to calculate determinant");
    }
    unsigned long n = rows;
    matrix a(*this); // Make a copy of the matrix

    double det = 1;
    for (unsigned long i = 0; i < n; ++i) {
        // Find the pivot
        unsigned long pivot = i;
        for (unsigned long j = i + 1; j < n; ++j) {
            if (abs(a.data[j * n + i]) > abs(a.data[pivot * n + i])) {
                pivot = j;
            }
        }

        // Swap rows if needed
        if (pivot != i) {
            for (unsigned long k = 0; k < n; ++k) {
                std::swap(a.data[i * n + k], a.data[pivot * n + k]);
            }
            det *= -1; // Swap changes the sign of the determinant
        }

        // Check for zero pivot
        if (a.data[i * n + i] == 0) {
            return 0; // Determinant is zero
        }

        // Eliminate the column
        for (unsigned long j = i + 1; j < n; ++j) {
            double factor = a.data[j * n + i] / a.data[i * n + i];
            __m256d factor_register, mul_register, final_register;
            factor_register= _mm256_set1_pd(factor);
            for(unsigned long y=i;y<n-(n%4);y+=4){
                mul_register = _mm256_loadu_pd(&a(i,y));
                final_register = _mm256_loadu_pd(&a(j,y));
                mul_register = _mm256_mul_pd(factor_register,mul_register);
                final_register = _mm256_sub_pd(final_register,mul_register);
                _mm256_storeu_pd(&a(j,y),final_register);
            }
            for (unsigned long k =n-(n%4) ; k < n; ++k) {
                a.data[j * n + k] -= factor * a.data[i * n + k];
            }
        }

        // Multiply the diagonal elements
       // det *= a.data[i * n + i];
    
    }
    thread* T = new thread[MAX_THREADS];
    unsigned long no_elements = a.rows;
    for(unsigned long j=0;j<MAX_THREADS-1;j++){
        unsigned long start=j*(no_elements/MAX_THREADS);
        unsigned long end=(j+1)*(no_elements/MAX_THREADS);
        T[j]=thread(mlt_mul_det, ref(a), ref(det), start,end);
    }
    T[MAX_THREADS-1]= thread(mlt_mul_det, ref(a), ref(det),(MAX_THREADS-1)*(no_elements/MAX_THREADS) ,no_elements);
    for(int k=0;k<MAX_THREADS;k++){
        T[k].join();
    }
    delete[] T;
    return det;
}
