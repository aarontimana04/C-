#include <iostream>
using namespace std;
// funciones
void sumar(int n){
    int suma = 0;
    for(int i = 0; i< n; i++){
        suma = suma + i+1; // 1, 2, 3
    }
    cout<<suma;
}

int sumar2(int n){
    int suma = 0;
    for(int i = 0; i< n; i++){
        suma = suma + i+1; // 1, 2, 3
    }
    return suma;
}



int main() {
    int n_valores = 3;
    sumar(n_valores); // void
    cout<<endl;
    int suma_total;
    suma_total = sumar2(n_valores); // int -> retorna un valor

    cout<<suma_total;
    return 0;
}