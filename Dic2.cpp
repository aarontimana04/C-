#include <iostream>
#include <string>
using namespace std;


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
        if(tamLista >= tamMax){
        	cout<<"Capacidad de la lista excedida";
        	return;
		}
        for (int i = tamLista; i > actual; i--)
            arregloLista[i] = arregloLista[i - 1]; 
        arregloLista[actual] = elemento;
        tamLista++;
    }

    void agregar(const E& elemento) {
        if(tamLista >= tamMax){
        	cout<<"Capacidad de lista excedida";
        	return;
		}
        arregloLista[tamLista] = elemento;
        tamLista++;
    }

    E eliminar() {
        if(!((actual >= 0) && (actual < tamLista))){
        	cout<<"No hay elemento";
        	return E();
		}
        E elemento = arregloLista[actual];
        for (int i = actual; i < tamLista - 1; i++)
            arregloLista[i] = arregloLista[i + 1];
        tamLista--;
        return elemento;
    }

    void limpiar() {
        tamLista = actual = 0;
    }

    void moverAInicio() { actual = 0; }
    void moverAlFinal() { actual = tamLista; }
    void anterior() { if (actual != 0) actual--; }
    void siguiente() { if (actual < tamLista) actual++; }
    int longitud() const { return tamLista; }
    int posicionActual() const { return actual; }

    void moverAPosicion(int pos) {
        if(!((pos >= 0) && (pos <= tamLista))){
        	cout<<"Posicion fuera de rango";
        	return;
		}
        actual = pos;
    }

    // "int pos" nos sirve como un auxiliar de actual para recoger un elemento en x posicion, sin alterar el valor de actual
    // que nos sirve para realizar operaciones (NO TOCAR)
    E& getElemento(int pos) {
        if(!(pos >= 0 && pos < tamLista)){
        	cout<<"Indice fuera de rango";
        	// Retornar una referencia válida para evitar segmentation fault
        	static E elementoVacio;
        	return elementoVacio;
		}
        return arregloLista[pos];
    }

    E getValor() {
        if(!((actual >= 0) && (actual < tamLista))){
        	cout<<"No hay elemento en la posicion actual";
        	return E();
        }
        return arregloLista[actual];
    }
};

// Clase MergeSort
template<typename T>
class MergeSort {
public:
    // Mezcla dos subarreglos ordenados en uno
    void merge(T* A, int n, T* A1, int n1, T* A2, int n2) {
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

    // Algoritmo MergeSort 
    void mergeSort(T* A, int n) {
        if (n <= 1) return;

        int n1 = n / 2;
        int n2 = n - n1;

        T* A1 = new T[n1];
        T* A2 = new T[n2];

        for (int i = 0; i < n1; i++) A1[i] = A[i];
        for (int i = 0; i < n2; i++) A2[i] = A[n1 + i];

        mergeSort(A1, n1);
        mergeSort(A2, n2);

        merge(A, n, A1, n1, A2, n2);

        delete[] A1;
        delete[] A2;
    }
};

// Clase Contacto
class Contacto {
private:
    string nombres;
    string apellidos;
    string celular;

public:
    Contacto() {}
    
    Contacto(string nombres, string apellidos, string celular) {
        this->nombres = nombres;
        this->apellidos = apellidos;
        this->celular = celular;
    }
/* SON EQUIVALENTES
    Contacto(string nombres_, string apellidos_, string celular_) {
        nombres = nombres_;
        apellidos = apellidos_;
        celular = celular_;
    }
*/
    // Constructor de copia
    Contacto(const Contacto& otro) {
        this->nombres = otro.nombres;
        this->apellidos = otro.apellidos;
        this->celular = otro.celular;
    }

    // Operador de asignacion
    void operator=(const Contacto& otro) {
        this->nombres = otro.nombres;
        this->apellidos = otro.apellidos;
        this->celular = otro.celular;
    }
    // Contacto c1, c2
    // c1 == c2 no check | 5 == 5 check 
    // operator= -> c1.nombres = c2.nombres | aaron = alvaro | 1 -> 27 |  

    // Operador de comparacion para ordenamiento alfabetico
    bool operator<(const Contacto& otro) const {
        return nombres < otro.nombres; // Contacto otro -> nombres, apellidos, telef -> otro.nombres
    }

    // Getters
    string getNombres() const { return nombres; }
    string getApellidos() const { return apellidos; }
    string getCelular() const { return celular; }

    // Setters
    void setNombres(const string& nombres) { this->nombres = nombres; }
    void setApellidos(const string& apellidos) { this->apellidos = apellidos; }
    void setCelular(const string& celular) { this->celular = celular; }

    // Metodo para mostrar el contacto
    void mostrar() const {
        cout << "Nombre: " << nombres << " " << apellidos << " - Celular: " << celular << endl;
    }
};

// Clase Clave-valor
template<typename C, typename E>
class ClaveValor {
private:
    C clave;
    E valor;
public:
    ClaveValor(){}
    
    ClaveValor(C clave, E valor){
        this->clave = clave;
        this->valor = valor;
    }

    ClaveValor(const ClaveValor& o) {
        this->clave = o.clave;
        this->valor = o.valor;
    }

    // Contacto1
    // Contacto2

    // if(ClaveValor2 == ClaveValor1)
    // 10 == 10

    // Compara dos pares de ClaveValor basandose en sus valores, no en sus claves. 
    // Permite ordenar los contactos de forma alfabetica.
    void operator=(const ClaveValor& o) {
        this->clave = o.clave;
        this->valor = o.valor;
    }

    // Operador de comparacion para ordenamiento
    bool operator<(const ClaveValor& otro) const { // ClaveValor otro -> (nombre, contacto) (C, E) -> otro.contacto  
        return valor < otro.valor;  // Compara por valor (Contacto)
    }

    C getClave() const { return clave; }
    void setClave(const C &clave) { this->clave = clave; }
    E getValor() const { return valor; }
    void setValor(const E &valor) { this->valor = valor; }
};

// Clase abstracta Diccionario
template<typename C, typename E>
class Diccionario {
public:
    Diccionario(){}
    virtual ~Diccionario(){}
    virtual void limpiar() = 0;
    virtual void insertar(C k, E v) = 0;
    virtual E remover(C k) = 0;
    virtual E removerCualquiera() = 0;
    virtual E encontrar(C k) = 0;
    virtual int longitud() = 0;
};

// Clase concreta DiccionarioAL
template<typename C, typename E>
class DiccionarioAL : public Diccionario<C, E> {
private:
    ALista<ClaveValor<C, E> >* lista;
    MergeSort<ClaveValor<C, E> > ordenador; // Instancia de MergeSort
    // Diccionario (ClaveValor(nombreContacto, Contacto))
public:
    DiccionarioAL(int tam = 1000) {
        lista = new ALista<ClaveValor<C, E> >(tam);
        // Se crea la lista de forma dinamica porque se trata de un atributo de tipo puntero
        // se le asigna un tam inicial al tratarse de una lista basada en arreglo
    }

    ~DiccionarioAL() {
        delete lista;
    }

    void limpiar() {
        lista->limpiar();
    }

    void insertar(C k, E v) {
        ClaveValor<C,E> temp(k,v);
        lista->agregar(temp);
    }

    E encontrar(C k) {
        for (lista->moverAInicio(); lista->posicionActual() < lista->longitud(); lista->siguiente()) {
            ClaveValor<C,E> temp = lista->getValor();
            if (k == temp.getClave()) {
                return temp.getValor();
            }
        }
        return E(); // Retorna un objeto vacio del tipo E

        // 1. Recorre toda la lista desde el inicio
        // 2. Para cada elemento, obtiene el par clavevalor
        // 3. Si la clave coincide con la busqueda, retorna el valor
        // 4. Caso contrario, retorna un objeto vacia (mensaje de eror)
        
    }

    E remover(C k) {
        for (lista->moverAInicio(); lista->posicionActual() < lista->longitud(); lista->siguiente()) {
            ClaveValor<C,E> temp = lista->getValor();
            if (k == temp.getClave()) {
                E valor = temp.getValor();
                lista->eliminar();
                return valor;
            }
        }
        return E(); // No encontrado

        // Similar al metodo encontrar, pero ademas elimina el elemento de la lista cuando lo encuentra
    }

    E removerCualquiera() {
        if (lista->longitud() == 0) {
            return E(); // Lista vacia
        }
        lista->moverAlFinal();
        lista->anterior(); // Mover al ultimo elemento válido
        ClaveValor<C,E> temp = lista->eliminar();
        return temp.getValor();
        // remueve el elemento segun la posicion del valor de actual
    }

    int longitud() {
        return lista->longitud();
    }

    // Metodo adicional para obtener todos los contactos ordenados
    void obtenerTodosOrdenados() {
        if (lista->longitud() == 0) {
            cout << "No hay contactos en la agenda." << endl;
            return;
        }

        // Crear arreglo temporal
        ClaveValor<C, E>* arreglo = new ClaveValor<C, E>[lista->longitud()];
        
        // Copiar elementos de la lista al arreglo
        for (int i = 0; i < lista->longitud(); i++) {
            arreglo[i] = lista->getElemento(i);
        }

        // Ordenar usando MergeSort (ahora usando la instancia ordenador)
        ordenador.mergeSort(arreglo, lista->longitud());

        // Mostrar contactos ordenados
        cout << "\n=== CONTACTOS ORDENADOS ALFABÉTICAMENTE ===" << endl;
        for (int i = 0; i < lista->longitud(); i++) {
            cout << i + 1 << ". ";
            arreglo[i].getValor().mostrar();
        }

        delete[] arreglo;
    }
};

// Clase Agenda de Contactos (llevarlo al MAIN como funciones)
class AgendaContactos {
private:
    DiccionarioAL<string, Contacto>* diccionario; // Usar puntero

public:
    // Constructor sin lista de inicialización
    AgendaContactos() {
        diccionario = new DiccionarioAL<string, Contacto>(1000); // Crear dinámicamente
    }
    
    // Destructor para liberar memoria
    ~AgendaContactos() {
        delete diccionario;
    }

    void agregarContacto() {
        string nombres, apellidos, celular;
        
        cout << "\n=== AGREGAR NUEVO CONTACTO ===" << endl;
        cout << "Ingrese nombres: ";
        cin.ignore();
        getline(cin, nombres); // ingresar datos: 11 12 // cin<<n1<<n2
        // nombres: aaron victor manuel -> nombres
        // cin >> nombre
        cout << "Ingrese apellidos: ";
        getline(cin, apellidos);
        cout << "Ingrese celular: ";
        getline(cin, celular);

        // creamos un contacto con los valores ingresados anteriormente
        // en este caso se llama NUEVOCONTACTO
        Contacto nuevoContacto(nombres, apellidos, celular);
        // con el contacto creado, pasamos a guardarlo (insertar) en el diccionario (como clavevalor)
        diccionario->insertar(nombres, nuevoContacto);
        
        cout << "Contacto agregado exitosamente!" << endl;
    }

    void buscarContacto() {
        string nombre;
        cout << "\n=== BUSCAR CONTACTO ===" << endl;
        cout << "Ingrese el nombre a buscar: ";
        // ignore y getline van juntos (primero se ignora cin, luego se llama a get)
        cin.ignore();
        // cin --> getline
        getline(cin, nombre);

        Contacto contacto = diccionario->encontrar(nombre);
        if (contacto.getNombres().empty()) {
            cout << "Contacto no encontrado." << endl;
        } else {
            cout << "Contacto encontrado:" << endl;
            contacto.mostrar();
        }
    }

    void eliminarContacto() {
        string nombre;
        cout << "\n=== ELIMINAR CONTACTO ===" << endl;
        cout << "Ingrese el nombre del contacto a eliminar: ";
        cin.ignore();
        getline(cin, nombre);

        Contacto contacto = diccionario->remover(nombre);
        if (contacto.getNombres().empty()) {
            cout << "Contacto no encontrado." << endl;
        } else {
            cout << "Contacto eliminado:" << endl;
            contacto.mostrar();
        }
    }

    void mostrarTodosOrdenados() {
        cout << "\n=== MOSTRAR TODOS LOS CONTACTOS ===" << endl;
        diccionario->obtenerTodosOrdenados();
    }

    void mostrarMenu() {
        cout << "\n========================================" << endl;
        cout << "         AGENDA DE CONTACTOS" << endl;
        cout << "========================================" << endl;
        cout << "1. Agregar nuevo contacto" << endl;
        cout << "2. Buscar contacto por nombre" << endl;
        cout << "3. Eliminar contacto" << endl;
        cout << "4. Mostrar todos los contactos ordenados" << endl;
        cout << "5. Salir" << endl;
        cout << "========================================" << endl;
        cout << "Seleccione una opcion: ";
    }

    void ejecutar() {
        int opcion;
        
        do {
            mostrarMenu();
            cin >> opcion;
            switch (opcion) {
                case 1:
                    agregarContacto();
                    break;
                case 2:
                    buscarContacto();
                    break;
                case 3:
                    eliminarContacto();
                    break;
                case 4:
                    mostrarTodosOrdenados();
                    break;
                case 5:
                    cout << "\n¡Gracias por usar la agenda de contactos!" << endl;
                    break;
                default:
                    cout << "\nOpcion invalida. Por favor, seleccione una opcion del 1 al 5." << endl;
            }
        } while (opcion != 5);
    }
};

int main() {
    AgendaContactos agenda;
    agenda.ejecutar();
    return 0;

    // metodo no estatico ( no static)
    //MergeSort m1;
    //m1.mergesort()

    // metodo estatico (static)
    // MergeSort::merge(int* a, int*b)
}

// CREAR UNA FUNCION DE ORDENAMIENTO EN EL MAIN, SIMILAR A LA CLASE MERGESORT PARA VALORES REALES (LETRAS)
// valor1 < valor2
// aaron < fabricio
// INSERCION EN ORDEN ALFABETICO
// temporaldiccionario[pos(aaron)]
// diccionariotemp = [contacto(aaron)]