// ProyectV2.h
#include <string>
#include <cmath>
#include <iostream> // Agregado para cout en las clases

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
    Nodo<E>* siguiente;

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
            // Manejar error de forma más robusta en un caso real
            // Por simplicidad, se podría lanzar una excepción o devolver un valor por defecto.
            // Aquí se imprime un mensaje y puede causar comportamiento indefinido.
            // cout << "Posicion invalida" << endl;
            // Para evitar crasheos, se devuelve un valor por defecto.
            return E();
        }
        Nodo<E>* actual = cabeza;
        for (int i = 0; i < pos; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
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
            cout << actual->getData() << " ";
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
        if (_tamMax > 0)
            arreglo = new E[tamMax];
        else
            arreglo = nullptr;
    }

    ~ACola() {
        delete[] arreglo;
    }
    // push and pop
    void push(const E& item) {
        if (tam == tamMax) {
            cout << "Cola llena" << endl;
            return;
        }
        arreglo[final] = item;
        final = (final + 1) % tamMax;
        tam++;
    }

    E pop() {
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
    int tam;
    E* arreglo;
    
    void merge(E* a, E* a1, int n1, E* a2, int n2, E* resultado) {
        int i = 0, j = 0, k = 0;
        while (i < n1 && j < n2) {
            if (a1[i] <= a2[j]) {
                resultado[k++] = a1[i++];
            } else {
                resultado[k++] = a2[j++];
            }
        }
        while (i < n1) resultado[k++] = a1[i++];
        while (j < n2) resultado[k++] = a2[j++];
    }

    void mergeSortRecursivo(E* a, int n) {
        if (n <= 1) return;
        int n1 = n / 2;
        int n2 = n - n1;
        E* a1 = new E[n1];
        E* a2 = new E[n2];
        
        for (int i = 0; i < n1; ++i) a1[i] = a[i];
        for (int i = 0; i < n2; ++i) a2[i] = a[n1 + i];
        
        mergeSortRecursivo(a1, n1);
        mergeSortRecursivo(a2, n2);
        merge(a, a1, n1, a2, n2, a);
        
        delete[] a1;
        delete[] a2;
    }


public:
    APila(int _tamMax) : tamMax(_tamMax), top(0), tam(0) {
        if (_tamMax > 0)
            arreglo = new E[tamMax];
        else
            arreglo = nullptr;
    }

    ~APila() {
        delete[] arreglo;
    }

    void push(const E& item) {
        if (tam == tamMax) {
            cout << "Pila llena" << endl;
            return;
        }
        arreglo[top] = item;
        top = (top + 1); // No es modular en una pila de arreglo simple
        tam++;
    }

    E pop() {
        if (tam == 0) {
            cout << "Pila vacia" << endl;
            return E();
        }
        top--;
        E valor = arreglo[top];
        tam--;
        return valor;
    }

    E obtener(int pos) const {
        if (pos < 0 || pos >= tam) {
            cout << "Posicion invalida en la pila" << endl;
            return E();
        }
        return arreglo[pos];
    }

    int longitud() const {
        return tam;
    }

    void ordenarMergeSort() {
        mergeSortRecursivo(arreglo, tam);
        top = tam;
    }
};

// Lista basada en Arreglo (Array List)
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