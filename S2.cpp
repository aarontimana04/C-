#include <iostream>
#include <cmath>
using namespace std;

class Func_General{
    public:
    virtual double calcular() = 0; 
};

class DesviacionEstandar : public Func_General {
    private:
    int N; // cant de valores
    double *numeros; // ptr al array [1,2,3,4,5]
    double promedio; // 
    double desviacion; // sumatoria
    public:
    // constructor
    DesviacionEstandar(int _N){ // constructor base para acceder a las propiedades de la clase
        N = _N; // para todo dentro del metodo, se usará N en lugar de n
        numeros = new double[N]; // ptr que apunta al array con N valores
        promedio = 0;
        desviacion = 0;
    }
    // destructor
    ~DesviacionEstandar(){
        delete[] numeros;
    }
    // METODOS DE LA CLASE
    void calcular_prom(){
        double suma = 0;
        for(int i = 0; i < N; i++){
            cout<<"Ingresa el numero "<<i + 1<<endl;
            cin>> numeros[i]; // i = 0 -> numeros[i] = 0
            suma = suma + numeros[i];
        }

        promedio = suma / N;
    }

    double calcular() {
        double sumatoria = 0;
        double resta = 0;
        for(int i = 0; i < N; i++){
            resta = numeros[i] - promedio;
            sumatoria = sumatoria + (resta * resta);
        }
        desviacion = sqrt(sumatoria / (N - 1)); // calculo de la desviacion estandar general

        desviacion = round(desviacion * 100.0) / 100.0; // redondeo a 2 decimales
        return desviacion;
    }

};

int main(){
    int N;
    cout<<"Ingresar cantidad de numeros: ";
    cin>>N;
    
    DesviacionEstandar *C1 = new DesviacionEstandar(N); // ptr de tipo DesviacionEstandar que apunta a un objeto de tipo DesviacionEstandar

    C1->calcular_prom(); // llamamos al metodo calcular_prom del objeto C1
    
    cout<<"La desviacion estandar será: " << C1->calcular()<<endl;

    delete C1;

    return 0;
}