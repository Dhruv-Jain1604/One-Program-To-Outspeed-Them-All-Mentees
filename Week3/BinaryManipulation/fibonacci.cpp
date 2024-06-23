#include<bits/stdc++.h>
typedef long long ll;
using namespace std;
#define pll pair<ll,ll>
#define pp pair<pll,pll>
#define mp make_pair
// #define f first
// #define s second 

ll naive (ll n) {
    ll dp[n+1];
    dp[1] = 1;
    dp[2] = 1;
    for (ll i=3; i<=n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
}

void mat_mul(ll** A, ll** B, ll** Result){
    ll D[2][2];
    for(int p=0;p<2;p++){
        for(int q=0;q<2;q++){
            D[p][q]=0;
        }
    }
    //Matrix multiplication
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            for(int k=0;k<2;k++){
                D[i][k]+=(A[i][j])*(B[j][k]);
            }
        }
    }
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            Result[i][j]=D[i][j];
        }
    }
}

ll optim(ll n){
/*

STUDENT CODE BEGINS HERE, ACHIEVE A SPEEDUP OVER NAIVE IMPLEMENTATION
YOU MAY EDIT THIS FILE HOWEVER YOU WANT
HINT : Use the same principle behind fast exponentiation, to calculate the nth fibonacci number in O(logn) time complexity
You can view the tuple (f(n), f(n-1)) as a matrix multiplication of ((1,1),(1,0))*(f(n-1),f(n-2)) 
because f(n) = f(n-1)*1 + f(n-2)*1 while f(n-1) = 1*f(n-1) + 0*f(n-2)
Therefore, we can compute (f(n),f(n-1)) is equal to {((1,1),(1,0))^(n-1)} * (f(1),f(0))
This can be computed in O(logn) time
We expect your code to be faster (and completely inaccurate, as even naive method is inaccurate) for n >= 1000

*/

if(n==0) return 0;
if(n==1 || n==2) return 1;

ll** C = new ll*[2];
C[0]= new ll[2];
C[1] = new ll[2];
C[0][0]=1;
C[0][1]=1;
C[1][0]=1;
C[1][1]=0;

ll** id = new ll*[2];
id[0]= new ll[2];
id[1] = new ll[2];
id[0][0]=1;
id[0][1]=0;
id[1][0]=0;
id[1][1]=1;
//Aim: To use fast exponentiation to obtain C^(n-1) to get f(n)

ll** Ans = id;
ll** Pow_2=C;
ll i=1;

while(n-1>=i){
    if(n&i) mat_mul(Ans,Pow_2,Ans);
    mat_mul(Pow_2,Pow_2,Pow_2);
    i=i<<1;
}

return Ans[0][1];

//cout<<"Student code not implemented\n";
//exit(1);

}

int main(){
    ll n; cin >> n;
    auto startNaive = chrono::high_resolution_clock::now();
    ll slow = naive(n);
    auto endNaive = chrono::high_resolution_clock::now();
    auto naiveTime = chrono::duration_cast<chrono::duration<double>>(endNaive - startNaive);

    auto startOptim = chrono::high_resolution_clock::now();
    ll fast = optim(n);
    auto endOptim = chrono::high_resolution_clock::now();
    auto optimTime = chrono::duration_cast<chrono::duration<double>>(endOptim - startOptim);

    cout<<"Answer from naive technique : "<<slow<<endl;
    cout<<"Answer from optimal technique : "<<fast<<endl;
    cout<<"Time taken by naive technique : "<<naiveTime.count()<<endl;
    cout<<"Time taken by optimal technique : "<<optimTime.count()<<endl;

}