/*
#include <iostream>
#include <string>

using namespace std;

int main(){
    cout<<"Hola, mundo!"
    return 0;
}
    
*/

// EJERCICIO 1
// String, fstream, for and while

#include <iostream>
#include <fstream>
using namespace std;

int main(){
/*
    // LECTURA
    ifstream datos_archivo("datos.txt"); // abrir el archivo
    string palabra;
    while (getline(datos_archivo, palabra))
    {
        cout<<palabra<<endl;
    }

    datos_archivo.close(); // cerrar archivo

    // ESCRITURA

    ofstream nuevo_archivo("datos3.txt");
    nuevo_archivo<<"Learning c++"<<endl;
    nuevo_archivo<< 101<<endl;

    nuevo_archivo.close();
*/
    // EJERCICIO
    int N;
    double suma = 0.0;
    double nota;

    ofstream archivo("info.txt");

    cout<<"Ingresar num de estudiantes"<<endl;
    cin>>N;

    archivo<< N <<endl;

    for(int i = 0 ; i < N ; i++){
        cout<<"Ingresar nota "<< i+1 << ":";
        cin>> nota;
        suma = suma + nota;
        archivo<<nota<<endl;
    }

    cout<<"Suma general: "<<suma<<endl;

    archivo<<suma<<endl;

    cout<<"Promedio: "<<suma/N<<endl;
    archivo<<suma/N<<endl;

    
    archivo.close();

    ifstream archivo2("info.txt"); // archivo2 es sobrenombre para leer info.txt

    string words;
    while(getline(archivo2, words)){
        cout<<words<<endl;
    }

    archivo2.close();

}
