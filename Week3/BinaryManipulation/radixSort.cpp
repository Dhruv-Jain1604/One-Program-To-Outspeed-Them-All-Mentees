#include <iostream>
#include<bits/stdc++.h>
using namespace std;

/*

TASK : Implement Radix Sort

Binary radix sort is a non-comparative integer sorting algorithm that sorts data by processing individual bits of the numbers.
The algorithm sorts the numbers by examining each bit from the most significant bit (MSB) to the least significant bit (LSB).
For each bit position, it partitions the numbers into two groups: those with the bit set to 0 and those with the bit set to 1.
This process is repeated for each bit position, and the numbers are merged back together after each bit pass. 
Binary radix sort is efficient for fixed-length integer keys, offering a time complexity of O(n) per pass, where n is the number of elements, 
making it suitable for sorting large datasets of integers.
EXPECTED TIME COMPLEXITY : O(n*log(q)) where q = max(arr)

*/

void countsort(int *arr, int size, int exponent){
    int temp[size];
    int i=0;
    int count_digit[2]={0,0};
    for(i=0;i<size;i++){
        count_digit[(arr[i]/exponent)%2]++;
    }
    count_digit[1]+=count_digit[0];
    for(i=size-1;i>=0;i--){
        temp[count_digit[(arr[i]/exponent)%2]-1]=arr[i];
        count_digit[(arr[i]/exponent)%2]--;
    }
    for(i=0;i<size;i++){
        arr[i]=temp[i];
    }

    return;

}
void radixSort (int *arr, int size) {

    int* max_pointer= max_element(arr, arr+size);
    int max = *(max_pointer);
    for(int exponent=1; max/exponent>0;exponent*=2){
        countsort(arr,size,exponent);
    }
    //cout<<"STUDENT CODE NOT IMPLEMENTED!\n";
    //exit(1);
}

int main () {

    int n;
    cin>>n;
    int arr[n];
    for (int i=0; i<n; i++) {
        arr[i] = rand();
    }
    radixSort(arr,n);
    for (int i=0; i<n-1; i++) {
        if (arr[i] > arr[i+1]) {
            cout<<"Sorting not done correctly!\n";
            exit(1);
        }
    }
    cout<<"Good job, array is sorted\n";
    return 0;
}