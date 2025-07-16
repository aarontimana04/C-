#include <iostream>
#include <string>
using namespace std;

// CONTROL DE LA CANTIDAD

void Assert(bool condicion, const char* mensaje) {
    if (!condicion) {
        cerr << mensaje << endl;
        exit(1);
    }
}

// Estructura Estudiante

struct Estudiante {
    string nombre;
    float notas[5];  // Máximo 5 notas
    int cantidadNotas;

    Estudiante(string n = "", float* nArray = NULL, int cant = 0) {
        nombre = n;
        cantidadNotas = cant;
        for (int i = 0; i < cant; i++) {
            notas[i] = nArray[i];
        }
    }

    float promedio() const {
        if (cantidadNotas == 0) return 0;
        float suma = 0;
        for (int i = 0; i < cantidadNotas; i++)
            suma += notas[i];
        return suma / cantidadNotas;
    }

    void mostrar() const {
        cout << "Nombre: " << nombre << ", Notas: ";
        for (int i = 0; i < cantidadNotas; i++)
            cout << notas[i] << " ";
        cout << "=> Promedio: " << promedio() << endl;
    }
};
/* 
struct Estudiante {
	string nombre;
	int cantidad;
	float notas; // Search: no pointer
	
    Estudiante(string _nombre = ""){
        nombre = _nombre;
        cantidad = 0;
        notas = ;
    }

    ~Estudiante(){
        delete[] notas;
    }

    Estudiante(const Estudiante& otro){
        nombre = otro.nombre;
        cantidad = otro.cantidad;
        notas = new float[cantidad];
        for(int i = 0; i < cantidad; i++){
            notas[i] = otro.notas[i];
        }
    }

	void registroDatos(){
		cout<<"Ingrese el nombre del estudiante: ";
		cin>>nombre;
		cout<<"Ingrese la cantidad de notas: ";
		cin>>cantidad;
		
		for(int i=0; i<cantidad;i++){
			cout<<"Ingrese la nota #"<<i+1<<endl;
			cin>>notas[i];
		}
	}
	
	float promedio(){
		float suma=0;
		float promedio=0;
		for(int i=0;i<cantidad;i++){
			suma=suma+notas[i];
		}
		promedio= suma/cantidad;
		return promedio;
	}
	
	void mostrar(){
		cout<<"Nombre: "<<nombre<<", Notas: ";
		for(int i=0; i<cantidad;i++){
			cout<<notas[i]<<" ";
			cout<<endl<<"El promedio es: "<<promedio()<<endl;
		}
	}
};
*/

template<typename E> class Lista{
private:
	void operator =(const Lista&) {}
	Lista(const Lista&) {}
public:
	Lista() {} //Constructor por defecto
	virtual ~Lista() {} //Desctructor base
	//Limpiar contenidos de la lista para vaciarla.
	virtual void limpiar() = 0;
	// Insertar un elemento a la ubicaci�n actual
	// elemento: el elemento a ser insertado
	virtual void insertar(const E& elemento) = 0;
	//Agregar un elemento al final de la lista
	// elemento: El elemento a ser a�adido
	virtual void agregar(const E& elemento) = 0;
	//Eliminar y retornar el elemento actual.
	//Retornar el elemento que fue eliminado.
	virtual E eliminar() = 0;
	// Cambiar la posici�n actual al inicio de la lista
	virtual void moverAInicio() = 0;
	// Cambiar la posici�n actual al final de la lista
	virtual void moverAlFinal() = 0;
	// Mover la posici�n actual un paso a la izquierda. No cambiar si est� al inicio.
	virtual void anterior() = 0;
	// Mover la posici�n actual un paso a la derecha. No cambiar si est� al final.
	virtual void siguiente() = 0;
	// Retorna: el n�mero de elementos en la lista
	virtual int longitud() const = 0;
	// Retorna: la posici�n del elemento actual
	virtual int posicionActual() const = 0;
	// Cambia la posici�n actual, donde �pos� es la nueva posici�n.
	virtual void moverAPosicion(int pos) = 0;
	//Retornar el elemento actual.
	virtual const E& valor() const = 0;
};

// Lista basada en arreglo
// int lista[5];
// for() lista[i] -> lista = [1,2,3,4,5,6]
const int tamPorDefecto = 15;
template<typename E>
class ALista : public Lista<E>{
private:
    int tamMax, tamLista, actual;
    E* arregloLista; // l1 =[1,2,3]
public:
    ALista(int tam = tamPorDefecto) {
        tamMax = tam;
        tamLista = actual = 0;
        arregloLista = new E[tamMax];
    }
    ~ALista() { delete[] arregloLista; }
    // actual = 3 // actual indica la posicion
    // tamMax = 10
    // arregloLista[2] = 3
    // arregloLista = [1,2,3,4,5,6]
    // i = tamLista = 6
    // actual = 3
    // iteraciones cuando i sea igual a 6, 5 y 4
    // arregloLista[6] = arregloLista[5]
    // inicial: arregloLista = [1,2,3,4,5,6]
    // final: arregloLista = [1,2,3,4,5,6,6]
    // tamaño de l2 = 5
    // l2 = [?,?,?,?,?]
    // l2 = [1,2,3]
    // si actual = 1, esto representa el valor 2
    // insertar(5) l2 = [1,5,2,3]
    void limpiar () override {
        delete [] arregloLista;
    }
    void insertar (const E& elemento) override { 
        Assert(tamLista < tamMax, "Capacidad de la lista excedida");
        for (int i = tamLista; i > actual; i--)
            arregloLista[i] = arregloLista[i - 1]; 
        arregloLista[actual] = elemento;
        tamLista++;
    }

    void agregar (const E& elemento) override{
        //Assert(tamLista < tamMax, "Capacidad de lista excedida");
        arregloLista[tamLista] = elemento;
        tamLista++;
        // l2 = [1,2,3,?,?], queremos agregar(10)
        // previo: tamLista = 3
        // l2[tamLista] = 10
        // l2 = [1,2,3,10,?]
    }
    // l2 = [1,3,?,?,?]
    // tamLista--;
    // return valor eliminado en la variable elemento
    E eliminar() override{
        Assert((actual >= 0) && (actual < tamLista), "No hay elemento");
        E elemento = arregloLista[actual];
        for (int i = actual; i < tamLista - 1; i++)
            arregloLista[i] = arregloLista[i + 1];
            // [1,2,3,4] ->   
        tamLista--;
        return elemento;
    }
    // Cambiar la posición actual al inicio de la lista
    void moverAInicio() override{ actual = 0; } //Resetea la posición
    // Cambiar la posición actual al final de la lista
    void moverAlFinal() override{actual = tamLista;} //Mover al final
    // Mover la posición actual un paso a la izquierda. No cambiar si está al inicio.
    void anterior() override{if(actual!=0) actual--;} //Regresa
    // l4 = [1,2,3,4,5]
    // si actual es 2, es decir, el valor de 3, su anterior. Para saber el indice del elemento 2, debemos restar una unidad a actual
    // Mover la posición actual un paso a la derecha. No cambiar si está al final.
    void siguiente() override{if(actual<tamLista) actual++;} //Siguiente
    // Retorna: el número de elementos en la lista
    int longitud() const override{return tamLista;}
    // Retorna: la posición del elemento actual

    // faltaba
    int posicionActual() const override { return actual; }
    // Cambia la posición actual, donde “pos” es la nueva posición.
    void moverAPosicion(int pos) override{
        Assert((pos >= 0) && (pos <= tamLista), "Posicion fuera de rango");
        actual = pos;
    }
    // faltaba
    const E& valor() const override {
        Assert((actual >= 0) && (actual < tamLista), "No hay elemento actual");
        return arregloLista[actual];
    }
};

// Principal funcion
int main() {
    //1. Crear instancias con new (Clase base)
    // int* ptr = new X<int>()

    // PASO 1: CREAR LISTA QUE ADMITE SOLO "<ESTUDIANTES>"
    Lista<Estudiante>* listaEstudiantes = new ALista<Estudiante>();

    // PASO 3: CREAR ARRAY DE NOTAS PARA EL PASO 2 DE LOS ESTUDIANTES
    float notasE1[] = {18.5, 17.0, 19.0, 18.0};
    float notasE2[] = {13.5, 14.0, 12.5, 15.0};
    float notasE3[] = {20.0, 19.5, 18.0, 19.0};

    // PASO 2: CREAR ESTUDIANTES PARA MI LISTA
    Estudiante E1("Ana",notasE1,4); // OBS: MIS ESTUDIANTES TIENEN UNA LISTA DE NOTAS, QUE PREVIAMENTE DEBE EXISTIR
    Estudiante E2("Luis",notasE2,4);
    Estudiante E3("Maria",notasE3,4);

    // PD: YA TENEMOS A LOS ESTUDIANTES CREADO
    // PASO 4: AGREGAR A LOS ESTUDIANTES A NUESTRA LISTA
    listaEstudiantes->agregar(E1);
    listaEstudiantes->agregar(E2);
    listaEstudiantes->agregar(E3);

    // PASO 5: ITERAR DENTRO DE LA LISTA DE ESTUDIANTES, Y POR CADA ESTUDIANTE, MOSTRAR SU INFO (NOMBRE, NOTAS, PROM)
    for(int i = 0; i < listaEstudiantes->longitud();i++){ // l1 = [E1, E2, E3]
        listaEstudiantes->moverAPosicion(i); // ACTUAL SE MUEVE A LA POSICION "i"
        listaEstudiantes->valor().mostrar();
        // listaEstudiantes->valor() == Ex. Devuelve un estudiante que esta en la posicion que indica "i", al cual llamaremos Ex
        // Ex.mostrar() == Accedemos a la info de Ex, se imprime
    }

    return 0;
}
