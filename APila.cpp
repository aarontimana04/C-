#include <iostream>
using namespace std;


//MergeSort (T [ ] a, entero n)
    //MergeSort(a1, n/2);

void merge(int* a, int* a1, int n1, int* a2, int n2, int* resultado) {
    int i = 0, j = 0, k = 0;
    // n1 y n2 presenta el tamaño de lista1 y lista2. Recuerda que a1 y a2
    // son las mitades de a
    while (i < n1 && j < n2) {
        if (a1[i] < a2[j]) { // cambiar para ordenar de forma ascendente o descendente
            resultado[k++] = a1[i++];
            // a1 = [5,6,7]
            // a2 = [4,3,1]
            // 5 > 4

            // resultado = [5]
        } else {
            resultado[k++] = a2[j++];
            // resultado = [4]
        }
    }
    // Estos bucles te sirven en caso a1 tenga valores aún mientras a2 
    // haya llegado al final
    // a2 = [9,2,3] a1 = [5,6,7,1,2]
    while (i < n1) resultado[k++] = a1[i++];
    while (j < n2) resultado[k++] = a2[j++];
}

void mergeSort(int* a, int n) {

    if (n <= 1) return;
    // creas las 2 sublistas
    int n1 = n / 2; // primera mitad
    int n2 = n - n1; // segunda mitad

    int* a1 = new int[n1]; // creas un puntero a la primera mitad con n1 espacios
    int* a2 = new int[n2]; // creas el 2do puntero a la segunda mitad

    // estos bucles te van a ayudar a ingresar los valores de la lista
    // original a las 2 sublistas
    for (int i = 0; i < n1; ++i) a1[i] = a[i];
    for (int i = 0; i < n2; ++i) a2[i] = a[n1 + i];
    // a = [1,2,3,4]
    // a1 = [1,2] -> a3[1], a4 [2]
    // a2 = [3,4]

    mergeSort(a1, n1);
    mergeSort(a2, n2);

    merge(a, a1, n1, a2, n2, a);

    delete[] a1;
    delete[] a2;
}
template<typename E> 
class Pila{
  public:
  Pila() {} //Constructor por defecto
  virtual ~Pila(){} //Destructor base

  //elemento: el elemento que ser� colocado encima de la pila.
  virtual void push(const E& it) = 0;

  //Eliminamos el elemento que est� en la cima de la pila.
  // Retornar: el elemento de la cima de la pila y la retiramos de esa estructura de datos
  virtual E pop() = 0;

  //Reinicializar la pila. El usuario es responsable por solicitar almacenamiento usado para los elementos de la pila.
  virtual void limpiar() = 0;

  //Retornar: una copia del elemento que se encuentra en el "top" o "cima".
  virtual const E& valorTop() const = 0;

  //Retornar: el n�mero de elementos en la pila
  virtual int longitud() const = 0;
};

template<typename E>
class APila : public Pila<E> {
private:
    int tamMax;
    int top;
    int tam;
    E* arreglo;

public:
    APila(int _tamMax) {
      tamMax = _tamMax; //tama�o maximo del arreglo
      tam=0; // atributo del tama�o actual del arreglo
      top = 0; //elemento que esta en la cima
      arreglo = new E[tamMax]; //arreglo de elementos

    }

    ~APila() {
        delete[] arreglo;
    }

    void push(const E& elemento) {
    if(tam==tamMax){
      cout<<"La pila esta llena";
      return;
    }
        arreglo[top] = elemento;
        top=(top+1)%tamMax; //buscar el indice del elemento en lo más alto (top - 1)
        tam++;
    }
/* implementación
    E pop() {
      if(tam==0){
        cout<<"La cola esta vacia";
    }
    E valor=arreglo[top];
    top=(top-1)%tamMax;
    top--;
    tam--;
        return valor;
    }
*/
// la corrección
    E pop() {
      // verificamos que la cola este o no vacía
      if(tam==0){
        cout<<"La cola esta vacia";
      }
      // lo primero es retroceder el indice de top antes de eliminar el valor,
      // para eso hacemos (top - 1). Luego, para evitar que el indice caiga en valor negativo,
      // por ejemplo, cuando top es 0 y restamos 1, debemos sumar el valor del tamaño maximo del arreglo (tamMax)
      // (top - 1 + tamMax) para asegurar siempre un indice posttivo. Lo ultimo que se hace es obtener el modulo
      // con tamMax para asegurar que top no sobrepase el valor del tamaño del arreglo -> top = (top - 1 +       //tamMax)%tamMax
      top=(top-1+tamMax)%tamMax;
      E valor=arreglo[top];
      tam--;
      return valor;

      // por ejemplo, tienes l1 = [1,2,3,4]
      // tamMax 6
      // si quieres hacer un pop, debes acceder al elemento 4. Entonces debes calcular top, recuerda que 
      // cuando hiciste push para cada elemento de la lista, top terminó con indice 4, por qué 4? por cada
      // inserción se aumentaba en 1 su valor. Inició top = 0 y al hacer push(1) entonces top = 1.
      // cuando hiciste push(3) entonces top = 4. Teniendo en cuenta eso, si quieres eliminar el elemento en 
      // lo más alto, debes calcular su indice, que sería top - 1, pero como te contaba en el parrafo de arriba
      // si fuera top = 0 habria un problema y ya sabes la razon porque se calcula el indice como
      // top = (top - 1 + tamMax) % tamMax
    }

    void limpiar(){
      top=0;
      tam=0;
  }

  const E& valorTop() const{
    if (tam==0){
      cout<<"Pila vacia: ";
    }
    return arreglo[(top - 1 + tamMax) % tamMax];
    // aqui es similar, si sabes calcular el indice del elemento mas alto que quieres eliminar
    // solo debes retornar ello
  }

  int longitud()const{
    return tam;
  }

  void ordenarMergeSort() {
    mergeSort(arreglo, tam);
    top = tam;
}

};


int main() {
    int numero;
    APila<int> pila(5);

    //MergeSort<Apila> m1(pila, 5);

    //m1.ordenarMergeSort();

    // Ingresar 4 n�meros con push
    for (int i = 0; i < 5; i++) {
        cout << "Ingrese el numero "<<i+1<<": ";
        cin >> numero;
        pila.push(numero);
    }

    cout<<"El primer valor de la pila es: "<<pila.valorTop()<<endl;
    // Eliminar el �ltimo ingresado con pop
    int eliminado = pila.pop();
    cout << "Elemento eliminado: " << eliminado << endl;


    //Mostar el ultimo valor del arreglo

    cout<<"El primer valor de la pila es: "<<pila.valorTop()<<endl;

    cout<<"El tama�o actual de la pila es: "<<pila.longitud()<<endl;

    pila.ordenarMergeSort();

    cout << "\nPila ordenada (menor en base, mayor en la cima):\n";
    int longitud = pila.longitud();
    for (int i = 0; i < longitud; ++i) {
        cout << pila.valorTop() << " ";
        pila.pop();
    }
    cout << endl;

    return 0;
}
