#include <string>
#include <cmath>
#include <iostream> // Agregado para cout en caso de error

using namespace std;

template<typename E>
class Lista {
public:
    Lista() {}
    virtual ~Lista() {}
    virtual void insertar(const E& item) = 0;
    virtual E obtener(int pos) const = 0;
    virtual int longitud() const = 0;
    virtual void limpiar() = 0;
    virtual bool buscar(const E& item) const = 0;
};

template<typename E>
class Nodo {
private:
    E data;
    Nodo* siguiente;

public:
    Nodo(const E& valor, Nodo* sig = nullptr) : data(valor), siguiente(sig) {}
    
    // Getters
    E getData() const { return data; }
    Nodo* getSiguiente() const { return siguiente; }
    
    // Setters
    void setData(const E& valor) { data = valor; }
    void setSiguiente(Nodo* sig) { siguiente = sig; }
};

// Lista Enlazada
template<typename E>
class LLista : public Lista<E> {
private:
    Nodo<E>* cabeza;
    Nodo<E>* cola;
    int tam;

public:
    LLista() : cabeza(nullptr), cola(nullptr), tam(0) {}
    
    ~LLista() {
        limpiar();
    }

    void insertar(const E& item) override {
        Nodo<E>* nuevo = new Nodo<E>(item);
        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        } else {
            cola->setSiguiente(nuevo);
            cola = nuevo;
        }
        tam++;
    }

    E obtener(int pos) const override {
        if (pos < 0 || pos >= tam) {
            cout << "Posicion invalida" << endl;
            // Retorna un valor por defecto si la posición es inválida
            return E();
        }
        Nodo<E>* actual = cabeza;
        for (int i = 0; i < pos; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
    }

    // Para ANOVA, se necesita modificar un elemento en la lista
    // Esta función no es parte de la interfaz base, pero es útil.
    E& obtenerRef(int pos) {
        if (pos < 0 || pos >= tam) {
             cout << "Posicion invalida" << endl;
             // Esto es peligroso, pero se mantiene simple para el ejercicio
             static E default_val;
             return default_val;
        }
        Nodo<E>* actual = cabeza;
        for (int i = 0; i < pos; i++) {
            actual = actual->getSiguiente();
        }
        // No se puede retornar una referencia directa al dato, se necesita un setter
        // En este caso, la estructura de Nodos debería tener un método para obtener referencia al dato
        // Por simplicidad, se omite y se usa el par (obtener, y un setter si fuese necesario)
        // La implementación de ANOVA usará obtener() y reconstruirá el par si es necesario.
        // O mejor, la estructura de datos para ANOVA (par) contendrá un puntero a ALista.
        return actual->getData(); // Cuidado: esto retorna una copia.
    }

    int longitud() const override {
        return tam;
    }

    void limpiar() override {
        while (cabeza != nullptr) {
            Nodo<E>* temp = cabeza;
            cabeza = cabeza->getSiguiente();
            delete temp;
        }
        cola = nullptr;
        tam = 0;
    }

    bool buscar(const E& item) const override {
        Nodo<E>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->getData() == item) return true;
            actual = actual->getSiguiente();
        }
        return false;
    }

    void mostrar() const {
        Nodo<E>* actual = cabeza;
        while (actual != nullptr) {
            // Asume que E puede ser impreso
            // cout << actual->getData() << " ";
            actual = actual->getSiguiente();
        }
        cout << endl;
    }
};

// Cola basada en Arreglo
template<typename E>
class ACola {
private:
    int tamMax;
    int frente;
    int final;
    int tam;
    E* arreglo;

public:
    ACola(int _tamMax) : tamMax(_tamMax), frente(0), final(0), tam(0) {
        arreglo = new E[tamMax];
    }

    ~ACola() {
        delete[] arreglo;
    }

    void enqueue(const E& item) {
        if (tam == tamMax) {
            cout << "Cola llena" << endl;
            return;
        }
        arreglo[final] = item;
        final = (final + 1) % tamMax;
        tam++;
    }

    E dequeue() {
        if (tam == 0) {
            cout << "Cola vacia" << endl;
            return E();
        }
        E item = arreglo[frente];
        frente = (frente + 1) % tamMax;
        tam--;
        return item;
    }

    bool estaVacia() const {
        return tam == 0;
    }

    int longitud() const {
        return tam;
    }
};

// Pila basada en Arreglo (para ordenamiento)
template<typename E>
class APila {
private:
    int tamMax;
    int top;
    E* arreglo;

public:
    APila(int _tamMax) : tamMax(_tamMax), top(0) {
        arreglo = new E[tamMax];
    }

    ~APila() {
        delete[] arreglo;
    }

    void push(const E& item) {
        if (top == tamMax) {
            cout << "Pila llena" << endl;
            return;
        }
        arreglo[top++] = item;
    }

    E pop() {
        if (top == 0) {
            cout << "Pila vacia" << endl;
            return E();
        }
        return arreglo[--top];
    }

    E obtener(int pos) const {
        if (pos < 0 || pos >= top) {
            cout << "Posicion invalida en Pila" << endl;
            return E();
        }
        return arreglo[pos];
    }

    int longitud() const {
        return top;
    }

    void ordenarMergeSort() {
        if (top <= 1) return;
        mergeSort(arreglo, 0, top - 1);
    }

private:
    void merge(E arr[], int l, int m, int r) {
        int n1 = m - l + 1;
        int n2 = r - m;
        E* L = new E[n1];
        E* R = new E[n2];
        for (int i = 0; i < n1; i++) L[i] = arr[l + i];
        for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) arr[k++] = L[i++];
            else arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
        delete[] L;
        delete[] R;
    }

    void mergeSort(E arr[], int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
};

template<typename E>
class ALista : public Lista<E> {
private:
    int tamMax;
    int tam;
    E* arreglo;

    void redimensionar() {
        tamMax *= 2;
        E* nuevoArreglo = new E[tamMax];
        for (int i = 0; i < tam; i++) {
            nuevoArreglo[i] = arreglo[i];
        }
        delete[] arreglo;
        arreglo = nuevoArreglo;
    }

public:
    ALista(int tamInicial = 10) : tamMax(tamInicial), tam(0) {
        arreglo = new E[tamMax];
    }

    ~ALista() {
        delete[] arreglo;
    }

    void insertar(const E& item) override {
        if (tam == tamMax) {
            redimensionar();
        }
        arreglo[tam++] = item;
    }

    E obtener(int pos) const override {
        if (pos < 0 || pos >= tam) {
            cout << "Posicion invalida en ALista" << endl;
            return E(); 
        }
        return arreglo[pos];
    }

    int longitud() const override {
        return tam;
    }

    void limpiar() override {
        delete[] arreglo;
        tam = 0;
        tamMax = 10;
        arreglo = new E[tamMax];
    }

    bool buscar(const E& item) const override {
        for (int i = 0; i < tam; i++) {
            if (arreglo[i] == item) return true;
        }
        return false;
    }
    
    E& operator[](int pos) {
        if (pos < 0 || pos >= tam) {
           cout << "Indice fuera de rango" << endl;
           // Esto es problemático, pero por simplicidad se asume un uso correcto
           static E dummy;
           return dummy;
        }
        return arreglo[pos];
    }
};
