#include <iostream>

namespace paso4 {
    // Declaramos nuestra clase abstracta Cola
    template<typename E>
    class Cola {
    public:
        Cola() {}
        virtual ~Cola() {}

        // Método para colocar un elemento al final de la cola.
        virtual void encolar(const E& it) = 0;

        // Método para eliminar y retornar el elemento del frente de la cola.
        virtual E desencolar() = 0;

        // Método para reinicializar la cola.
        virtual void limpiar() = 0;

        // Método para retornar una copia del elemento frontal.
        virtual const E& valorFrontal() const = 0;

        // Método para retornar el número de elementos en la cola.
        virtual int longitud() const = 0;
    };

    // Implementación de la clase concreta ACola
    template<typename E>
    class ACola : public Cola<E> {
    private:
        E* arreglo; // Arreglo para almacenar los elementos
        int tamMax; // Tamaño máximo de la cola
        int frente; // Índice del frente de la cola
        int fin; // Índice del final de la cola
        int tam; // Número de elementos en la cola
    public:
        ACola(int maxSize) {
            tamMax = maxSize;
            frente = fin = 0;
            tam = 0;
            arreglo = new E[tamMax];
        }

        ~ACola() {
            delete[] arreglo; // Liberar memoria al destruir la cola
        }

        void encolar(const E& valor) override {
            if (tam == tamMax) {
                std::cout<<"Cola llena"; // Manejo de error
            }
            arreglo[fin] = valor; // Colocar el elemento en el final
            fin = (fin + 1) % tamMax; // Actualizar el índice del final
            tam++; // Incrementar el tamaño
        }

        E desencolar() override {
            if (tam == 0) {
                std::cout<<"Cola vacía"; // Manejo de error
            }
            E valor = arreglo[frente]; // Obtener el valor del frente
            frente = (frente + 1) % tamMax; // Actualizar el índice del frente
            tam--; // Decrementar el tamaño
            return valor; // Retornar el valor
        }

        void limpiar() override {
            frente = 0; // Reiniciar el frente
            fin = 0; // Reiniciar el fin
            tam = 0; // Reiniciar el tamaño
        }

        const E& valorFrontal() const override {
            if (tam == 0) {
                std::cout<<"Cola vacía"; // Manejo de error
            }
            return arreglo[frente]; // Retornar el valor en el frente
        }

        int longitud() const override {
            return tam; // Retornar el tamaño de la cola
        }
    };
}

using namespace std;

int main() {
    int maxSize;
    cout << "Ingrese la cantidad maxima de elementos:" << endl;
    cin >> maxSize;

    paso4::ACola<int> cola(maxSize); // Crear una instancia de la cola

    for (int i = 0; i < maxSize; i++) {
        int valor;
        cout << "Ingrese el valor " << 1 + i << ": ";
        cin >> valor;
        if(valor == 0){
            break;
        }
        cola.encolar(valor);
    }

    // Mostrar el valor en el frente
    cout << "Valor en el frente de la cola: " << cola.valorFrontal() << endl;

    // Probar la longitud de la cola
    cout << "Número de elementos en la cola: " << cola.longitud() << endl;

    // Probar la eliminación de elementos
    cout << "Eliminando elementos de la cola:" << endl;
    while (cola.longitud() > 1) {
        cout << "Elemento eliminado: " << cola.desencolar() << endl;
    }

    return 0;
}