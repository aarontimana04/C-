#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int numero = 25; // Número a escribir en el archivo

    // Escritura en archivo binario
    ofstream archivoSalida("datos.bin", ios::binary);
    archivoSalida.write(reinterpret_cast<char*>(&numero), sizeof(numero));
    archivoSalida.close();

    cout << "Número guardado en datos.bin" << endl;

    // Lectura desde archivo binario
    int numeroLeido = 0;
    ifstream archivoEntrada("datos.bin", ios::binary);
    archivoEntrada.read(reinterpret_cast<char*>(&numeroLeido), sizeof(numeroLeido));
    archivoEntrada.close();

    cout << "Número leído desde datos.bin: " << numeroLeido << endl;

    return 0;
}
