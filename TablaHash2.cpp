#include <iostream>
#include <string>
using namespace std;

template<typename E>
struct Nodo {
    E data;
    Nodo* next;
    Nodo(const E& valor, Nodo* sig = nullptr) {
        data = valor;
        next = sig;
    }
};

template<typename E>
class MergeSort {
public:
    void dividirLista(Nodo<E>* fuente, Nodo<E>** frente, Nodo<E>** atras) {
        Nodo<E>* rapido;
        Nodo<E>* lento;

        if (fuente == nullptr || fuente->next == nullptr) {
            *frente = fuente;
            *atras = nullptr;
        } else {
            lento = fuente;
            rapido = fuente->next;

            while (rapido != nullptr) {
                rapido = rapido->next;
                if (rapido != nullptr) {
                    lento = lento->next;
                    rapido = rapido->next;
                }
            }

            *frente = fuente;
            *atras = lento->next;
            lento->next = nullptr;
        }
    }

    Nodo<E>* mezclarListas(Nodo<E>* a, Nodo<E>* b) {
        if (a == nullptr)
            return b;
        else if (b == nullptr)
            return a;

        Nodo<E>* resultado = nullptr;

        if (a->data <= b->data) {
            resultado = a;
            resultado->next = mezclarListas(a->next, b);
        } else {
            resultado = b;
            resultado->next = mezclarListas(a, b->next);
        }

        return resultado;
    }

    void ordenar(Nodo<E>** cabezaRef) {
        Nodo<E>* cabeza = *cabezaRef;
        Nodo<E>* a;
        Nodo<E>* b;

        if ((cabeza == nullptr) || (cabeza->next == nullptr)) {
            return;
        }

        dividirLista(cabeza, &a, &b);

        ordenar(&a);
        ordenar(&b);

        *cabezaRef = mezclarListas(a, b);
    }
};

template<typename E>
class LLista {
private:
    Nodo<E>* head;
    Nodo<E>* tail;
    int tam;
    MergeSort<E> mergeSort; // Instancia de la clase MergeSort

public:
    LLista() {
        head = tail = nullptr;
        tam = 0;
    }

    ~LLista() {
        limpiar();
    }

    void insertarFinal(const E& valor) {
        Nodo<E>* nuevoNodo = new Nodo<E>(valor);
        if (head == nullptr) {
            head = tail = nuevoNodo;
        } else {
            tail->next = nuevoNodo;
            tail = nuevoNodo;
        }
        tam++;
    }

    bool eliminar(const E& valor) {
        Nodo<E>* actual = head;
        Nodo<E>* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->data == valor) {
                if (anterior == nullptr) {
                    head = actual->next;
                } else {
                    anterior->next = actual->next;
                }
                if (actual == tail) {
                    tail = anterior;
                }
                delete actual;
                tam--;
                return true;
            }
            anterior = actual;
            actual = actual->next;
        }
        return false;
    }

    void limpiar() {
        while (head != nullptr) {
            Nodo<E>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        tam = 0;
    }

    E obtener(int index) const {
        if (index < 0 || index >= tam) {
            cout << "Indice fuera de rango" << endl;
            return E();
        }
        Nodo<E>* actual = head;
        for (int i = 0; i < index; i++) {
            actual = actual->next;
        }
        return actual->data;
    }

    int longitud() const {
        return tam;
    }

    void mostrar() const {
        Nodo<E>* actual = head;
        while (actual != nullptr) {
            cout << actual->data << " -> ";
            actual = actual->next;
        }
        cout << "NULL" << endl;
    }

    void ordenarMergeSort() {
        mergeSort.ordenar(&head);
        tail = head;
        while (tail && tail->next) {
            tail = tail->next;
        }
    }

    bool buscar(const E& valor) const {
        Nodo<E>* actual = head;
        while (actual != nullptr) {
            if (actual->data == valor) {
                return true;
            }
            actual = actual->next;
        }
        return false;
    }

    E* buscarReferencia(const E& valor) {
        Nodo<E>* actual = head;
        while (actual != nullptr) {
            if (actual->data == valor) {
                return &(actual->data);
            }
            actual = actual->next;
        }
        return nullptr;
    }
};

template<typename C, typename E>
class ClaveValor {
private:
    C clave;
    E valor;
public:
    ClaveValor() {}
    
    ClaveValor(C clave, E valor) {
        this->clave = clave;
        this->valor = valor;
    }

    ClaveValor(const ClaveValor& o) {
        this->clave = o.clave;
        this->valor = o.valor;
    }

    void operator=(const ClaveValor& o) {
        this->clave = o.clave;
        this->valor = o.valor;
    }

    bool operator<(const ClaveValor& otro) const {
        return valor < otro.valor;
    }

    bool operator==(const ClaveValor& otro) const {
        return clave == otro.clave;
    }

    C getClave() const { return clave; }
    void setClave(const C &clave) { this->clave = clave; }
    E getValor() const { return valor; }
    void setValor(const E &valor) { this->valor = valor; }
};

// Clase abstracta Diccionario (reutilizada)
template<typename C, typename E>
class Diccionario {
public:
    Diccionario() {}
    virtual ~Diccionario() {}
    virtual void limpiar() = 0;
    virtual void insertar(C k, E v) = 0;
    virtual E remover(C k) = 0;
    virtual E removerCualquiera() = 0;
    virtual E encontrar(C k) = 0;
    virtual int longitud() = 0;
};

// Implementación de Tabla Hash con Encadenamiento
template<typename C, typename E>
class TablaHash : public Diccionario<C, E> {
private:
    static const int tam_tabla = 5; // Numero de filas de la tabla hash
    LLista<ClaveValor<C, E>>* tabla[tam_tabla];
    int numElementos;

    // Funcion Hash: Suma de valores ASCII modulo tamaño de tabla
    int funcionHash(const C& clave) const {
        int suma = 0;
        string str = static_cast<string>(clave);
        for (char c : str) {
            suma += static_cast<int>(c);
        }
        return suma % tam_tabla;
    }

public:
    TablaHash() {
        numElementos = 0;
        // Inicializar todas las posiciones con listas vacias
        for (int i = 0; i < tam_tabla; i++) {
            tabla[i] = new LLista<ClaveValor<C, E>>();
        }
    }

    ~TablaHash() {
        limpiar();
        for (int i = 0; i < tam_tabla; i++) {
            delete tabla[i];
        }
    }

    void limpiar() override {
        for (int i = 0; i < tam_tabla; i++) {
            tabla[i]->limpiar();
        }
        numElementos = 0;
    }

    void insertar(C k, E v) override {
        int indice = funcionHash(k);
        ClaveValor<C, E> nuevoPar(k, v);
        
        // Verificar si la clave ya existe
        ClaveValor<C, E>* existente = tabla[indice]->buscarReferencia(nuevoPar);
        if (existente != nullptr) {
            // Actualizar valor existente
            existente->setValor(v);
        } else {
            // Insertar nuevo par
            tabla[indice]->insertarFinal(nuevoPar);
            numElementos++;
        }
    }

    E encontrar(C k) override {
        int indice = funcionHash(k);
        ClaveValor<C, E> parBusqueda(k, E());
        
        ClaveValor<C, E>* encontrado = tabla[indice]->buscarReferencia(parBusqueda);
        if (encontrado != nullptr) {
            return encontrado->getValor();
        }
        return E(); // No encontrado
    }

    E remover(C k) override {
        int indice = funcionHash(k);
        ClaveValor<C, E> parBusqueda(k, E());
        
        ClaveValor<C, E>* encontrado = tabla[indice]->buscarReferencia(parBusqueda);
        if (encontrado != nullptr) {
            E valor = encontrado->getValor();
            tabla[indice]->eliminar(parBusqueda);
            numElementos--;
            return valor;
        }
        return E(); // No encontrado
    }

    E removerCualquiera() override {
        for (int i = 0; i < tam_tabla; i++) {
            if (tabla[i]->longitud() > 0) {
                ClaveValor<C, E> par = tabla[i]->obtener(0);
                tabla[i]->eliminar(par);
                numElementos--;
                return par.getValor();
            }
        }
        return E(); // Tabla vacia
    }

    int longitud() override {
        return numElementos;
    }

    void incrementarContador(const C& clave) {
        E valorActual = encontrar(clave);
        if (valorActual == E()) {
            // Si no existe, insertar con valor 1
            insertar(clave, static_cast<E>(1));
        } else {
            // Si existe, incrementar
            insertar(clave, valorActual + 1);
        }
    }

    // Metodo para mostrar estadisticas de la tabla hash
    void mostrarEstadisticas() const {
        
        int colisiones = 0;
        int posicionesUsadas = 0;
        
        for (int i = 0; i < tam_tabla; i++) {
            int longLista = tabla[i]->longitud();
            if (longLista > 0) {
                posicionesUsadas++;
                if (longLista > 1) {
                    colisiones += longLista - 1;
                }
            }
        }
        
        cout << "Total de colisiones: " << colisiones << endl;
        cout << endl;
    }

    // Metodo para mostrar todos los elementos ordenados alfabeticamente
    void mostrarTodosOrdenados() const {
        if (numElementos == 0) {
            cout << "La tabla esta vacia." << endl;
            return;
        }

        // Recopilar todos los elementos
        ClaveValor<C, E>* elementos = new ClaveValor<C, E>[numElementos];
        int indiceElementos = 0;

        for (int i = 0; i < tam_tabla; i++) {
            for (int j = 0; j < tabla[i]->longitud(); j++) {
                elementos[indiceElementos++] = tabla[i]->obtener(j);
            }
        }

        // Ordenamiento burbuja simple por clave
        for (int i = 0; i < numElementos - 1; i++) {
            for (int j = 0; j < numElementos - i - 1; j++) {
                if (elementos[j].getClave() > elementos[j + 1].getClave()) {
                    ClaveValor<C, E> temp = elementos[j];
                    elementos[j] = elementos[j + 1];
                    elementos[j + 1] = temp;
                }
            }
        }

        // Mostrar elementos ordenados en el formato solicitado
        for (int i = 0; i < numElementos; i++) {
            cout << elementos[i].getClave() << ": " << elementos[i].getValor() << endl;
        }

        delete[] elementos;
    }

    // Metodo adicional para mostrar solo las frecuencias
    void mostrarFrecuencias() const {
        cout << "\n--- FRECUENCIAS ---" << endl;
        mostrarTodosOrdenados();
    }
};

class ContadorFrecuencias {
private:
    TablaHash<string, int>* tabla;

public:
    ContadorFrecuencias() {
        tabla = new TablaHash<string, int>();
    }

    ~ContadorFrecuencias() {
        delete tabla;
    }

    void procesarEtiquetas() {
        string etiqueta;
        cout << "Ingrese las etiquetas (escriba 'FIN' para terminar):" << endl;
        
        while (true) {
            cin >> etiqueta;
            if (etiqueta == "FIN") {
                break;
            }
            tabla->incrementarContador(etiqueta);
        }
    }

    void mostrarResultados() {
        tabla->mostrarFrecuencias();
        tabla->mostrarEstadisticas();
    }

};

int main() {
    ContadorFrecuencias contador;
    int opcion;
    
    do {
        cout << "----------------------------------------" << endl;
        cout << "    Contador basado en TABLA HASH" << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Ingresar etiquetas manualmente" << endl;
        cout << "4. Salir" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                contador.procesarEtiquetas();
                contador.mostrarResultados();
                break;
            case 4:
                cout << "\n¡Hasta luego!" << endl;
                break;
            default:
                cout << "\nOpcion invalida. Seleccione una opcion del 1 al 4." << endl;
        }
    } while (opcion != 4);
    
    return 0;
}
