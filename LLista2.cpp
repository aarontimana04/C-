#include <iostream>
using namespace std;

namespace paso2 {
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

    template<typename E>
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

    template<typename E>
    void mergeSort(Nodo<E>** cabezaRef) {
        Nodo<E>* cabeza = *cabezaRef;
        Nodo<E>* a;
        Nodo<E>* b;

        if ((cabeza == nullptr) || (cabeza->next == nullptr)) {
            return;
        }

        dividirLista(cabeza, &a, &b);

        mergeSort(&a);
        mergeSort(&b);

        *cabezaRef = mezclarListas(a, b);
    }

    template<typename E>
    class LLista {
    private:
        Nodo<E>* head; // Apunta al primer nodo
        Nodo<E>* tail; // Apunta al último nodo
        int tam; // Tamaño de la lista

    public:
        LLista() {
            head = tail = nullptr;
            tam = 0;
        }

        ~LLista() {
            limpiar();
        }

        // Inserta un nuevo nodo al final
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

        // Elimina el primer nodo con el valor indicado
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
            return false; // No encontrado
        }

        // Limpia toda la lista
        void limpiar() {
            while (head != nullptr) {
                Nodo<E>* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;
            tam = 0;
        }

        // Retorna el valor en la posición dada (0-indexed)
        E obtener(int index) const {
            if (index < 0 || index >= tam) {
                cout << "Índice fuera de rango" << endl;
                return E();
            }
            Nodo<E>* actual = head;
            for (int i = 0; i < index; i++) {
                actual = actual->next;
            }
            return actual->data;
        }

        // Retorna el número de elementos
        int longitud() const {
            return tam;
        }

        // Muestra todos los elementos
        void mostrar() const {
            Nodo<E>* actual = head;
            while (actual != nullptr) {
                cout << actual->data << " -> ";
                actual = actual->next;
            }
            cout << "NULL" << endl;
        }

        void ordenarMergeSort() {
        mergeSort(&head);

        // Actualizar tail al final de la lista
        tail = head;
        while (tail && tail->next) {
            tail = tail->next;
        }
        }

    };
}


// Ejemplo de uso en main
int main() {
    paso2::LLista<int> lista;
    int nvalores;

    cout << "Ingrese el número de datos: ";
    cin >> nvalores;

    for (int i = 0; i < nvalores; ++i) {
        int valor;
        cout << "Ingrese el valor " << i + 1 << ": ";
        cin >> valor;
        lista.insertarFinal(valor);
    }

    cout << "\nContenido de la lista:\n";
    lista.mostrar();

    cout << "\nNúmero de elementos en la lista: " << lista.longitud() << endl;

    int eliminarValor;
    cout << "\nIngrese un valor a eliminar: ";
    cin >> eliminarValor;

    if (lista.eliminar(eliminarValor)) {
        cout << "Valor eliminado exitosamente.\n";
    } else {
        cout << "Valor no encontrado.\n";
    }

    cout << "\nContenido de la lista tras eliminación:\n";
    lista.mostrar();

    lista.ordenarMergeSort();

    cout<<"Contenido luego de ordenar:"<<endl;

    lista.mostrar();

    return 0;
}
