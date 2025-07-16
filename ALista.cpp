#include <iostream>
using namespace std;

// CONTROL DE ERRORES
void Assert(bool condicion, const char* mensaje) {
    if (!condicion) {
        cerr << mensaje << endl;
        exit(1);
    }
}

// Estructura Nodo (adaptada desde LPila / LLista)
struct Nodo {
    int valor;
    Nodo* next;

    Nodo(int v = 0, Nodo* sig = nullptr) {
        valor = v;
        next = sig;
    }

    void mostrar() const {
        cout << "Valor: " << valor << endl;
    }

    bool operator<(const Nodo& otro) const {
    return valor < otro.valor;
    }
};

// Lista basada en arreglo
const int tamPorDefecto = 15;

template<typename E>
class ALista {
private:
    int tamMax, tamLista, actual;
    E* arregloLista;

public:
    ALista(int tam = tamPorDefecto) {
        tamMax = tam;
        tamLista = actual = 0;
        arregloLista = new E[tamMax];
    }

    ~ALista() { delete[] arregloLista; }

    void insertar(const E& elemento) { 
        Assert(tamLista < tamMax, "Capacidad de la lista excedida");
        for (int i = tamLista; i > actual; i--)
            arregloLista[i] = arregloLista[i - 1]; 
        arregloLista[actual] = elemento;
        tamLista++;
    }

    void agregar(const E& elemento) {
        Assert(tamLista < tamMax, "Capacidad de lista excedida");
        arregloLista[tamLista] = elemento;
        tamLista++;
    }

    E eliminar() {
        Assert((actual >= 0) && (actual < tamLista), "No hay elemento");
        E elemento = arregloLista[actual];
        for (int i = actual; i < tamLista - 1; i++)
            arregloLista[i] = arregloLista[i + 1];
        tamLista--;
        return elemento;
    }

    void moverAInicio() { actual = 0; }
    void moverAlFinal() { actual = tamLista; }
    void anterior() { if (actual != 0) actual--; }
    void siguiente() { if (actual < tamLista) actual++; }
    int longitud() const { return tamLista; }
    int getactual() const { return actual; }

    void moverAPosicion(int pos) {
        Assert((pos >= 0) && (pos <= tamLista), "Posición fuera de rango");
        actual = pos;
    }

    E& getElemento(int pos) {
        Assert(pos >= 0 && pos < tamLista, "Índice fuera de rango");
        return arregloLista[pos];
    }
};
// algoritmo de ordenamiento
// Mezcla dos subarreglos ordenados en uno
void merge(Nodo* A, int n, Nodo* A1, int n1, Nodo* A2, int n2) {
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (A1[i] < A2[j]) {
            A[k++] = A1[i++];
        } else {
            A[k++] = A2[j++];
        }
    }
    while (i < n1) A[k++] = A1[i++];
    while (j < n2) A[k++] = A2[j++];
}

// Algoritmo MergeSort para arreglos de Nodo
void mergeSort(Nodo* A, int n) {
    if (n <= 1) return;

    int n1 = n / 2;
    int n2 = n - n1;

    Nodo* A1 = new Nodo[n1];
    Nodo* A2 = new Nodo[n2];

    for (int i = 0; i < n1; i++) A1[i] = A[i];
    for (int i = 0; i < n2; i++) A2[i] = A[n1 + i];

    mergeSort(A1, n1);
    mergeSort(A2, n2);

    merge(A, n, A1, n1, A2, n2);

    delete[] A1;
    delete[] A2;
}


// Función principal
int main() {
    ALista<Nodo> listaNodos;

    // Agregamos nodos con diferentes valores

    listaNodos.agregar(Nodo(15));
    listaNodos.agregar(Nodo(100));
    listaNodos.agregar(Nodo(7));
    listaNodos.agregar(Nodo(1));
    listaNodos.agregar(Nodo(2));
    listaNodos.agregar(Nodo(23));
    listaNodos.agregar(Nodo(20));

    cout << "Contenido de la lista de Nodos:\n";
    listaNodos.moverAInicio();
    for (int i = 0; i < listaNodos.longitud(); i++) {
        listaNodos.moverAPosicion(i);
        Nodo n = listaNodos.eliminar(); // Eliminar para mostrar
        n.mostrar();                    // Mostrar
        listaNodos.insertar(n);         // Reinsertar
        listaNodos.siguiente();         // Avanzar a la siguiente posición
    }

    // Obtener arreglo subyacente
    int n = listaNodos.longitud();
    Nodo* arreglo = new Nodo[n];
    for (int i = 0; i < n; ++i)
        arreglo[i] = listaNodos.getElemento(i);

    // Ordenar con MergeSort
    mergeSort(arreglo, n);

    // Mostrar lista ordenada
    cout << "\nLista ordenada por MergeSort:\n";
    for (int i = 0; i < n; ++i)
        arreglo[i].mostrar();

    delete[] arreglo;

    return 0;
}
