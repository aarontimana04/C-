// Este documento omite tildes y caracteres no alfanumericos restringidos
#include<iostream>
using namespace std;

// Clase abstracta de nodo de arbol binario 
template<typename E> class NodoBinario {
  public:
     virtual ~NodoBinario() {} //Base destructor
     //Retorna el valor del nodo
     virtual E& getElemento() = 0;
     //Modifica el valor del nodo
     virtual void setElemento(const E&) = 0;
     //Retorna el hijo izquierdo del nodo
     virtual NodoBinario* getIzquierdo() const = 0;
     //Modifica el hijo izquierdo del nodo
     virtual void setIzquierdo(NodoBinario* ) = 0;
     //Retorna el hijo derecho del nodo
     virtual NodoBinario* getDerecho() const = 0;
     //Modifica el hijo derecho del nodo
     virtual void setDerecho(NodoBinario* )=0;
     //Retorna si el nodo es un nodo hoja o no
     virtual bool esHoja() = 0;
};

// Implementacion de un nodo de arbol binario
template<typename C, typename E>
class NodoArbolBinarioBusqueda : public NodoBinario<E>{
   private:
	   C clave;  // La clave del nodo
	   E elemento; // El valor del nodo
	   NodoArbolBinarioBusqueda* izquierdo; //Puntero al hijo izquierdo
	   NodoArbolBinarioBusqueda* derecho; //Puntero al hijo derecho
   public:
	   //Constructor sin parametros iniciales
	   NodoArbolBinarioBusqueda() {izquierdo=derecho=nullptr;}
	   //Constructor con parametros iniciales
	   NodoArbolBinarioBusqueda(C clave, E elemento, NodoArbolBinarioBusqueda* izquierdo=nullptr, NodoArbolBinarioBusqueda* derecho=nullptr){
               this->clave = clave;
               this->elemento = elemento;
	       this->izquierdo = izquierdo;
	       this->derecho = derecho;
	   }
	   //Destructor
	   ~NodoArbolBinarioBusqueda() {}

           //Retornar elemento
	   E& getElemento() {return this->elemento;}
	   //Modificar elemento
	   void setElemento(const E& e) {this->elemento = e;}

	   //Get-set para la clave
	   C& getClave() {return clave;}
	   void setClave(const C& k) {this->clave=k;}
           
	   //Nodos hijo
	   NodoArbolBinarioBusqueda* getIzquierdo() const {return this->izquierdo;}
	   NodoArbolBinarioBusqueda* getDerecho() const {return this->derecho;}
           void setIzquierdo(NodoBinario<E>* b) {this->izquierdo = (NodoArbolBinarioBusqueda*)b;}
           void setDerecho(NodoBinario<E>* b) {this->derecho = (NodoArbolBinarioBusqueda*)b;}

	   //Retornar true si es hoja
	   bool esHoja() {return (this->izquierdo==nullptr)&&(this->derecho==nullptr);}
};



//Arbol binario de busqueda: implementacion basada en Diccionario
template<typename C, typename E>
class ArbolBinarioBusqueda : public Diccionario<C,E> {
  private:
	  NodoArbolBinarioBusqueda<C, E>* raiz; //Raiz del arbol binario de busqueda
	  int num_nodos; //Numero de nodos en el Arbol Binario de Busqueda

	  //Funciones de apoyo como metodo privado
	  NodoArbolBinarioBusqueda<C, E>* apoyoInsercion(ArbolBinarioBusqueda<C,E>*, const C&, const E&);
          void apoyoLimpiar(NodoArbolBinarioBusqueda<C,E>*);
	  //Funciones de apoyo como meteodo privado QUE FALTAN IMPLEMENTAR
	  NodoArbolBinarioBusqueda<C,E>* eliminarMin(NodoArbolBinarioBusqueda<C,E>*);
	  NodoArbolBinarioBusqueda<C,E>* getMin(NodoArbolBinarioBusqueda<C,E>*);
	  NodoArbolBinarioBusqueda<C,E>* apoyoEliminar(NodoArbolBinarioBusqueda<C,E>*, const C&);
	  E apoyoEncontrar(NodoArbolBinarioBusqueda<C,E>*, const C&) const;
	  void apoyoPrint(NodoArbolBinarioBusqueda<C,E>* , int) const;


  public:
	  ArbolBinarioBusqueda() {raiz=nullptr; num_nodos=0;} //Constructor
	  ~ArbolBinarioBusqueda(){ apoyoLimpiar(this->raiz); } //Debemos eliminar todos los nodos del arbol
	  
          //Insertar un registro en el arbol
	  //C Clave Valor del registro a insertar
	  //E El registro a insertar
	  void insertar(const C& clave, const E& e){
             raiz = apoyoInsercion(this->raiz, clave, e);
	     num_nodos++;
	  }


	  //Limpiar: reinicializar el arbol
	  void limpiar() //Reinicializar el arbol
          { apoyoLimpiar(this->raiz); this->raiz=nullptr; num_nodos=0; }

          //Eliminar un registro a partir del arbol
	  //C Valor clave de registro a eliminar
	  //Retorno: el registro eliminado o NULL si no hay nada
	  E eliminar(const C& k) {
		E temp = apoyoEncontrar(this->raiz, k); //Primero encontrarlo
		if(temp!=nullptr){
		   this->raiz = apoyoEliminar(this->raiz, k);
		   num_nodos--;
		}
		return temp;
	  }

	  //Eliminar y retornar el nodo raiz a partir del diccionario.
	  //Retornar: El registro eliminado, null si el arbol esta vacio.
	  E eliminarCualquiera() {//Eliminar valor minimo
	     if(this->raiz != nullptr) {
	        E temp = this->raiz->getElemento();
		this->raiz = apoyoEliminar(this->raiz, this->raiz->getClave());
		num_nodos--;
		return temp;
	     }
	     else return nullptr;
	  }

	  //Retornar registro con valor clave k, NULL si no existe
	  //k: el valor clave a encontrar
	  //Retorna un registro con quien se asocie "k".
	  //Retorna true si existe, false en caso contrario.
	  //Si hay multiples registros "k", retorna uno arbitrario
	  E encontrar(const C& k) const {return apoyoEncontrar(this->raiz, k);}

          //Retorna el numero de registros en el Diccionario
	  int tamanho() {return num_nodos;}

	  void print() const {
	     if(this->raiz == nullptr) cout<<"El arbol binario de busqueda esta vacio.\n";
	     else apoyoPrint(this->raiz, 0);
	  }
};



//Metodo privado de apoyoInsercion
template<typename C, typename E>
NodoArbolBinarioBusqueda<C,E>* ArbolBinarioBusqueda<C,E>::apoyoInsercion(NodoArbolBinarioBusqueda<C,E>* raiz, const C& k, const E& e){
   if(raiz==nullptr) //Arbol esta vacio: se requiere crear un nodo
      return new NodoArbolBinarioBusqueda<C,E>(k, e, nullptr, nullptr);
   if(k < raiz->getClave())
      raiz->setIzquierdo(apoyoInsercion(raiz->getIzquierdo(), k, e));
   else raiz->setDerecho(apoyoInsercion(raiz->getDerecho(), k, e));
   return raiz; //Retorna el arbol con el nodo insertado
}



//Metodo privado de apoyoEncontrar
template<typename C, typename E>
E ArbolBinarioBusqueda<C,E>::apoyoEncontrar(NodoArbolBinarioBusqueda<C,E>* raiz, const C& k) const {
   // AQUI VA TU IMPLEMENTACION
}


//Metodo privado eliminarMin
template<typename C, typename E>
NodoArbolBinarioBusqueda<C,E>* ArbolBinarioBusqueda<C,E>::eliminarMin(NodoArbolBinarioBusqueda<C,E>* rt) {
  // AQUI VA TU IMPLEMENTACION  
}

//Metodo privado getMin
template<typename C, typename E>
NodoArbolBinarioBusqueda<C,E>* ArbolBinarioBusqueda<C,E>::getMin(NodoArbolBinarioBusqueda<C,E>* rt){
  if(rt->getIzquierdo() == nullptr)
	  return rt;
  else return getMin(rt->getIzquierdo());
}

//Metodo apoyoEliminar
//Elimina un nodo con un valor clave k
//Retorna: el arbol con el nodo eliminado
template<typename C, typename E>
NodoArbolBinarioBusqueda<C,E>* ArbolBinarioBusqueda<C,E>::apoyoEliminar(NodoArbolBinarioBusqueda<C,E>* rt, const C& k){
   if(rt==nullptr) return nullptr; //k no se encuentra en el arbol
   else if(k < rt->getClave())
      rt->setIzquierdo(apoyoEliminar(rt->getIzquierdo(), k));
   else if(k > rt->getClave())
      rt->setDerecho(apoyoEliminar(rt->getDerecho(), k));
   else {
      NodoArbolBinarioBusqueda<C,E>* temp = rt;
      if(rt->getIzquierdo() == nullptr) { //Solo hacia el hijo derecho
	 rt = rt->getDerecho();
	 delete temp;
      }
      else if(rt->getDerecho() == nullptr){
         rt = rt->getIzquierdo();
	 delete temp;
      }
      else {
         NodoArbolBinarioBusqueda<C,E>* temp = getMin(rt->getDerecho());
	 rt->setElemento(temp->getElemento());
	 rt->setClave(temp->getClave());
	 rt->setDerecho(eliminarMin(rt->getDerecho());
	 delete temp;
      }
   }
}


//Metodo apoyoLimpiar
template<typename C, typename E>
void ArbolBinarioBusqueda<C,E>::apoyoLimpiar(NodoArbolBinarioBusqueda<C,E>* raiz){
    // AQUI VA TU CODIGO
}


//Metodo apoyoPrint
template<typename C, typename E>
void ArbolBinarioBusqueda<C,E>::apoyoPrint(NodoArbolBinarioBusqueda<C,E>* raiz, int nivel) const {
    if(raiz == nullptr) return; //Arbol esta vacio
    apoyoPrint(raiz->getIzquierdo(), nivel+1); 
    for(int i=0; i<nivel; i++)
       cout<<" ";
    cout<<raiz->getClave() << "\n"; //Imprimir el valor del nodo
    apoyoPrint(raiz->getDerecho(), nivel+1);
}



// Main: testing del ArbolBinario de Busqueda
int main(){
	
   return 0;
}