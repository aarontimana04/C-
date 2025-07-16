#include <iostream>

using namespace std;
// 1. FIBONACCI
// 0 1 1 2 3 5 8 13
// Recursivo
int Fibonacci(int n){
    // caso base
    if(n == 0) {
        return 0;}
    else if(n == 1){
        return 1;}
    else {
        return Fibonacci(n - 1) + Fibonacci(n - 2);
    }
}
// Iterativo
int Fibonacci2(int n){
    if(n==0){return 0;}
    else if(n==1){return 1;}
    else{
        int a = 0, b = 0, c;
        for(int i = 0; i<=n; i++){
            c = a + b;
            a = b;
            b = c;
        }
        return b;
    }
}

// plantillas

template <typename T>
T hallar_menor(T a, T b){
    if(a < b){return a;}
    else{
        return b;
    }
}
// equivalentes
int hallar_menor(int a, int b){
    if(a < b){return a;}
    else{
        return b;
    }
}

string hallar_menor(string a, string b){
    if(a < b){return a;}
    else{
        return b;
    }
}



int main(){
    int a = 10, b = 11;
    int aws;
    aws = hallar_menor(a,b);
    cout<< aws<<endl;

    string str1, str2;
    str1 = "b"; // 2
    str2 = "h"; // 8

    string rpta;

    rpta = hallar_menor(str1, str2);

    cout<< rpta<<endl;

    int n;
    //cout<<"terminos de fibo: "<<endl;
    cin>>n;

    int resultado;

    //resultado = Fibonacci(n);
    //cout<< resultado<<endl;
    return 0;
}