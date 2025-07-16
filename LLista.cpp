#include <iostream>
using namespace std;

void Assert(bool condicion, const char* mensaje) {
    if (!condicion) {
        cerr << mensaje << endl;
        exit(1);
    }
}
// Estructura de producto
struct Producto {
    string nombre;
    int cantidad;
    float precioUnitario;

    Producto(string n = "", int c = 0, float p = 0.0) {
        nombre = n;
        cantidad = c;
        precioUnitario = p;
    }

    float costoTotal() const {
        return cantidad * precioUnitario;
    }

    void mostrar() const {
        cout << "Producto: " << nombre
             << ", Cantidad: " << cantidad
             << ", Precio unitario: S/." << precioUnitario
             << ", Subtotal: S/." << costoTotal() << endl;
    }
};

//Lista basada en nodos
template<typename E>
class Nodo {
public:
    E elemento; // data
    Nodo* siguiente; // puntero
    // Nodo n1 = new Nodo<E>(10)
    Nodo(const E& elemval, Nodo* sig = nullptr) {
        elemento = elemval;
        siguiente = sig;
    }
};

template<typename E>
class LLista {
private:
    Nodo<E>* cabeza; // puntero al inicio
    Nodo<E>* cola; // puntero al final
    Nodo<E>* actual; // puntero que nos va a servir para navegar en la lista
    // l1 = [1,2,3,4] -> cabeza -> 1, cola -> 4 y actual -> 1
    // eliminar(3), actual1 -> 1 moverA(2) actual1 -> 3 delete 3
    int cantidad;
public:
    LLista() {
        actual = cola = cabeza = new Nodo<E>(E()); // E(): nodo vacío
        // l1 = [actual/cola/cabeza] -> []
        cantidad = 0;
    }
    // destructor: limpiar la lista o dejar la lista vacia
    ~LLista() {
        while (cabeza != nullptr) {
            Nodo<E>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
    void insertar(const E& elemento) {                                                          // actual
        actual->siguiente = new Nodo<E>(elemento, actual->siguiente); // [cabeza] -> [] -> [1] -> [2] -> [10] -> [3] -> [4] <- cola
        // actual 2, actual->siguiente 3                                                    // Queremos insertar 10: moverAPosicion(2), insertar(10) 
        if (cola == actual) cola = actual->siguiente; // cabeza/actual ->[1] -> [2] <- cola
        cantidad++; // operación de adicion
    }
    void agregar(const E& elemento) { // 1 2 3 4  6 7
        cola = cola->siguiente = new Nodo<E>(elemento, nullptr);
        cantidad++;
    }
    E eliminar() { // [cabeza] -> [] -> [1] -> [2] -> [10] -> [3] -> [4] <- cola. Queremos eliminar el 10
        Assert(actual->siguiente != nullptr, "No hay elemento");
        Nodo<E>* temp = actual->siguiente;
        E elemento = temp->elemento;
        if (cola == temp) cola = actual; // 1 2 <- cola/actual
        actual->siguiente = temp->siguiente; // 1 2 3 -> null
        delete temp;
        cantidad--;
        return elemento;
    }
    void moverAInicio() { actual = cabeza; } // actual/cabeza -> 1 2 3 
    void moverAlFinal () { actual = cola; } // cabeza -> 1 2 3 <- cola/actual
    void anterior() { // cabeza-> 1 2 3 4 5 6 7
    if(actual==cabeza) return;
    Nodo<E>* temp = cabeza;
    while(temp->siguiente!=actual) temp=temp->siguiente;
      actual = temp;
    }
    void siguiente () { if(actual != cola) actual=actual->siguiente; } // 1 2 3 null
    int longitud() const { return cantidad; } // devolver el valor del atributo cant

    void moverAPosicion(int pos) {
        Assert(pos >= 0 && pos <= cantidad, "Posición fuera de rango");
        actual = cabeza;
        for (int i = 0; i < pos; i++) actual = actual->siguiente;
    }
    // 1 2 3 4 5
    int posicionActual() const {
      Nodo<E>* temp = cabeza;
      int i;
      for(i=0; actual!=temp; i++)
      temp=temp->siguiente;
      return i;
    } // 1 2 3 4 5 6 7
    const E& valor () const {
      return actual->siguiente->elemento; // 1 2 3 4
    }
};

int main() {
    LLista<Producto> listaProductos;

    // Agregamos algunos productos al minimarket
    listaProductos.agregar(Producto("Arroz", 2, 3.50));     // 2kg x S/.3.50
    listaProductos.agregar(Producto("Leche", 4, 2.80));     // 4L x S/.2.80
    listaProductos.agregar(Producto("Huevos", 12, 0.60));   // 12u x S/.0.60
    listaProductos.agregar(Producto("Aceite", 1, 10.00));   // 1L x S/.10.00

    // Recorremos la lista para mostrar los productos y calcular el costo total
    float total = 0.0;
    listaProductos.moverAInicio();
    for (int i = 0; i < listaProductos.longitud(); i++) {
        listaProductos.moverAPosicion(i);
        Producto p = listaProductos.valor(); // obtenemos producto
        p.mostrar();
        total += p.costoTotal();
    }

    cout << "----------------------------------\n";
    cout << "Costo total de compra: S/." << total << endl;

    return 0;
}
