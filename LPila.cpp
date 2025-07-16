#include <iostream>
namespace paso1 {
    template<typename E>
    class Pila {
    public: 
        Pila() {}
        virtual ~Pila() {}
        virtual void push(const E& it) = 0;
        virtual E pop() = 0;
        virtual void limpiar() = 0;
        virtual const E& valorTop() const = 0;
        virtual int longitud() const = 0;
    };

    template <typename E>
    struct Nodo {
        E data;
        Nodo* next;
        Nodo(const E& valor, Nodo* sig = nullptr) {
            data = valor;
            next = sig;
        }
    };
    
    template<typename E>
    class LPila : public Pila<E> {
    private:
        Nodo<E>* top;
        int tam;
        // Divide la lista en dos mitades
        void dividirLista(Nodo<E>* fuente, Nodo<E>** frente, Nodo<E>** atras) {
            Nodo<E>* rapido;
            Nodo<E>* lento;
            if (fuente == nullptr || fuente->next == nullptr) {
                *frente = fuente;
                *atras = nullptr;
                return;
            }
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

        // Mezcla dos listas ordenadas
        Nodo<E>* mezclarListas(Nodo<E>* a, Nodo<E>* b) {
            if (a == nullptr) return b;
            if (b == nullptr) return a;

            Nodo<E>* resultado = nullptr;

            if (a->data >= b->data) { // change
                resultado = a;
                resultado->next = mezclarListas(a->next, b);
            } else {
                resultado = b;
                resultado->next = mezclarListas(a, b->next);
            }

            return resultado;
        }

        // MergeSort recursivo
        void mergeSort(Nodo<E>** cabezaRef) {
            Nodo<E>* cabeza = *cabezaRef;
            if (cabeza == nullptr || cabeza->next == nullptr) return;

            Nodo<E>* a;
            Nodo<E>* b;

            dividirLista(cabeza, &a, &b);
            mergeSort(&a);
            mergeSort(&b);
            *cabezaRef = mezclarListas(a, b);
        }

    public:
        LPila() {
            top = nullptr;
            tam = 0;
        }

        ~LPila() {
            limpiar();
        }

        void push(const E& valor) override {
            Nodo<E>* nuevoNodo = new Nodo<E>(valor, top);
            top = nuevoNodo;
            tam++;
        }

        E pop() override {
            if (top == nullptr) {
                std::cout << "Pila vacía" << std::endl;
                exit(1); // error fatal
            }
            E valor = top->data;
            Nodo<E>* nodoAEliminar = top;
            top = top->next;
            delete nodoAEliminar;
            tam--;
            return valor;
        }

        void limpiar() override {
            while (top != nullptr) {
                pop();
            }
        }

        const E& valorTop() const override {
            if (top == nullptr) {
                std::cout << "Pila vacía" << std::endl;
                exit(1);
            }
            return top->data;
        }

        int longitud() const override {
            return tam;
        }

        void ordenarMergeSort() {
            mergeSort(&top);
            // Recalcular tamaño
            tam = 0;
            Nodo<E>* temp = top;
            while (temp != nullptr) {
                tam++;
                temp = temp->next;
            }

            // Invertir para mantener orden LIFO con mayor arriba
            Nodo<E>* prev = nullptr;
            Nodo<E>* curr = top;
            while (curr != nullptr) {
                Nodo<E>* next = curr->next;
                curr->next = prev;
                prev = curr;
                curr = next;
            }
            top = prev;
        }

        // Método auxiliar para mostrar pila (solo para pruebas)
        void mostrar() const {
            Nodo<E>* temp = top;
            while (temp != nullptr) {
                std::cout << temp->data << " ";
                temp = temp->next;
            }
            std::cout << std::endl;
        }
    };
}

using namespace std;
int main() {
    paso1::LPila<int> pila;
    int nvalores;
    cout << "Ingrese el número de datos: ";
    cin >> nvalores;

    for (int i = 0; i < nvalores; ++i) {
        int valor;
        cout << "Ingrese el valor " << i + 1 << ": ";
        cin >> valor;
        pila.push(valor);
    }

    cout << "Pila original:\n";
    pila.mostrar();

    pila.ordenarMergeSort();

    cout << "Pila ordenada (cima = mayor):\n";
    pila.mostrar();

    return 0;
}
