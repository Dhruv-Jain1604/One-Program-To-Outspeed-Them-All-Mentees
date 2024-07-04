#include"matrix.h"

int main(){
    matrix temp(6,1);
    temp.data[0]=1;
    temp.data[1]=2;
    temp.data[2]=3;
    temp.data[3]=4;
    temp.data[4]=5;
    temp.data[5]=6;
    //cout<<norm(temp)<<endl;
    temp = 5*temp;
    temp = log(temp);
    temp.printMatrix();
    //temp.printMatrix();
    matrix one(6,5);
    for(uint64_t i=0;i<6;i++){
        for(uint64_t j=0;j<5;j++){
            one(i,j)=i+j;
        }
    }
    matrix two(5,6);
    for(uint64_t i=0;i<5;i++){
        for(uint64_t j=0;j<6;j++){
            two(i,j)=i+j;
        }
    }
    one.printMatrix();
    two.printMatrix();
    (matmul(one,two)).printMatrix();
}