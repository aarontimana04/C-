#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "ProyectV1.h"

using namespace std;

// Clase para frecuencias categóricas (cambiado de struct a class)
class FrecuenciaCategoria {
private:
    string categoria;
    int frecuenciaAbsoluta;
    double frecuenciaRelativa;

public:
    // Constructores
    FrecuenciaCategoria() : categoria(""), frecuenciaAbsoluta(0), frecuenciaRelativa(0.0) {}
    FrecuenciaCategoria(string cat) : categoria(cat), frecuenciaAbsoluta(1), frecuenciaRelativa(0.0) {}
    
    // Getters
    string getCategoria() const { return categoria; }
    int getFrecuenciaAbsoluta() const { return frecuenciaAbsoluta; }
    double getFrecuenciaRelativa() const { return frecuenciaRelativa; }
    
    // Setters
    void setCategoria(const string& cat) { categoria = cat; }
    void setFrecuenciaAbsoluta(int freq) { frecuenciaAbsoluta = freq; }
    void setFrecuenciaRelativa(double freq) { frecuenciaRelativa = freq; }
    
    // Incrementar frecuencia
    void incrementarFrecuencia() { frecuenciaAbsoluta++; }
    
    // Operador de comparación con string
    bool operator==(const string& cat) const {
        return categoria == cat;
    }
    
    // Operador de comparación con otra FrecuenciaCategoria
    bool operator==(const FrecuenciaCategoria& other) const {
        return categoria == other.categoria;
    }
};

class AnalizadorDataset {
private:
    LLista<string>* nombreColumnas;
    LLista<LLista<string>*>* datosFilas;
    int numFilas;
    int numColumnas;

    // Función auxiliar para parsear CSV correctamente (aqui habia un problema xd los numeros flotantes en los csv tienen comas y los separaba en 2 numeros)
    // para parsear correctamente usamos un metodo aux
    LLista<string>* parsearLineaCSV(const string& linea) {
        LLista<string>* campos = new LLista<string>();
        string campo = "";
        bool dentroComillas = false;
        
        for (size_t i = 0; i < linea.length(); i++) {
            char c = linea[i];
            
            if (c == '"') {
                dentroComillas = !dentroComillas;
            } else if (c == ',' && !dentroComillas) {
                // Fin del campo
                campos->insertar(campo);
                campo = "";
            } else {
                campo += c;
            }
        }
        
        // Agregar el último campo
        if (!campo.empty() || linea.back() == ',') {
            campos->insertar(campo);
        }
        
        return campos;
    }

    // Función para limpiar y convertir valores numéricos
    string limpiarValorNumerico(const string& valor) {
        string valorLimpio = valor;
        
        // Remover comillas al inicio y final si existen
        if (valorLimpio.length() >= 2 && valorLimpio.front() == '"' && valorLimpio.back() == '"') {
            valorLimpio = valorLimpio.substr(1, valorLimpio.length() - 2);
        }
        
        // Reemplazar comas por puntos para decimales
        for (char& c : valorLimpio) {
            if (c == ',') c = '.';
        }
        
        return valorLimpio;
    }

    // Función para verificar si un valor es numérico
    bool esNumerico(const string& valor) {
        string valorLimpio = limpiarValorNumerico(valor);
        if (valorLimpio.empty()) return false;
        
        try { // esto es un prueba error, pertenece a iostream, no te preocupes
            // quiza no lo has visto, es como un if, else pero para verificar errores
            stod(valorLimpio);
            return true;
        } catch (const exception& e) {
            return false;
        }
    }

    // segunda forma (seguro mas viable para el profe :D)
    /*a
    bool esNumerico(const string& valor) {
    string v = limpiarValorNumerico(valor);
    if (v.empty()) return false;
    bool punto = false;
    for (int i = 0; i < (int)v.size(); i++) {
        if (isdigit(v[i])) continue;
        if (v[i] == '-' && i == 0) continue;
        if (v[i] == '.' && !punto) { punto = true; continue; }
        return false;
    }
    return true;
    }
    */

public:
    AnalizadorDataset() {
        nombreColumnas = new LLista<string>();
        datosFilas = new LLista<LLista<string>*>();
        numFilas = 0;
        numColumnas = 0;
    }

    ~AnalizadorDataset() {
        // Limpiar datos
        for (int i = 0; i < datosFilas->longitud(); i++) {
            delete datosFilas->obtener(i);
        }
        delete datosFilas;
        delete nombreColumnas;
    }

    bool cargarCSV(const string& rutaArchivo) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << rutaArchivo << endl;
            return false;
        }

        string linea;
        bool primeraLinea = true;

        while (getline(archivo, linea)) { // getline lee una línea completa del archivo y la almacena en la variable linea. Retorna el stream del archivo.
            LLista<string>* fila = parsearLineaCSV(linea);

            if (primeraLinea) {
                // Guardar nombres de columnas
                for (int i = 0; i < fila->longitud(); i++) {
                    nombreColumnas->insertar(fila->obtener(i));
                }
                numColumnas = fila->longitud();
                delete fila; // No guardamos la fila de encabezados
                primeraLinea = false;
            } else {
                datosFilas->insertar(fila);
                numFilas++;
            }
        }

        archivo.close();
        cout << "Dataset cargado exitosamente!" << endl;
        cout << "Filas de datos: " << numFilas << endl;
        cout << "Columnas: " << numColumnas << endl;
        return true;
    }

    void mostrarVistaPrevia() {
        cout << "\n=============== VISTA PREVIA DEL DATASET ===============" << endl;
        cout << "Número de registros: " << numFilas << endl;
        cout << "Número de variables: " << numColumnas << endl;
        cout << "Total de celdas: " << (numFilas * numColumnas) << endl;
        
        cout << "\nVariables disponibles:" << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        cout << "\nPrimeras 5 filas:" << endl;
        // Mostrar encabezados
        for (int j = 0; j < nombreColumnas->longitud(); j++) {
            cout << nombreColumnas->obtener(j);
            if (j < nombreColumnas->longitud() - 1) cout << "\t";
        }
        cout << endl;
        
        // Mostrar datos
        int filasAMostrar = min(5, numFilas);
        for (int i = 0; i < filasAMostrar; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            for (int j = 0; j < fila->longitud(); j++) {
                string valor = fila->obtener(j);
                // Mostrar valor limpio para números
                if (esNumerico(valor)) {
                    cout << limpiarValorNumerico(valor);
                } else {
                    cout << valor;
                }
                if (j < fila->longitud() - 1) cout << "\t";
            }
            cout << endl;
        }
        cout << "======================================================" << endl;
    }

    void calcularFrecuenciaAbsolutaRelativa() {
        cout << "\n --- Frecuencia absoluta y relativa --" << endl;
        cout << "Seleccione la variable categorica:" << endl;
        
        // Mostrar variables disponibles
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        int columnaSeleccionada = opcion - 1;
        string nombreVariable = nombreColumnas->obtener(columnaSeleccionada);
        
        // Usar LLista para contar frecuencias
        LLista<FrecuenciaCategoria>* frecuencias = new LLista<FrecuenciaCategoria>();
        
        // Procesar cada fila
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valor = fila->obtener(columnaSeleccionada);
            
            // Buscar si la categoría ya existe
            bool encontrado = false;
            for (int j = 0; j < frecuencias->longitud(); j++) {
                FrecuenciaCategoria categoria = frecuencias->obtener(j);
                if (categoria == valor) {
                    encontrado = true;
                    break;
                }
            }
            
            // Si no existe, crear nueva categoría
            if (!encontrado) {
                frecuencias->insertar(FrecuenciaCategoria(valor));
            }
        }
        
        // Recalcular frecuencias correctamente
        LLista<FrecuenciaCategoria>* frecuenciasFinales = new LLista<FrecuenciaCategoria>();
        
        for (int i = 0; i < frecuencias->longitud(); i++) {
            FrecuenciaCategoria categoria = frecuencias->obtener(i);
            int contador = 0;
            
            // Contar todas las ocurrencias de esta categoría
            for (int j = 0; j < numFilas; j++) {
                LLista<string>* fila = datosFilas->obtener(j);
                string valor = fila->obtener(columnaSeleccionada);
                if (valor == categoria.getCategoria()) {
                    contador++;
                }
            }
            
            categoria.setFrecuenciaAbsoluta(contador);
            categoria.setFrecuenciaRelativa((double)contador / numFilas);
            frecuenciasFinales->insertar(categoria);
        }
        
        // Mostrar resultados
        cout << "\nFrecuencias para la variable: " << nombreVariable << endl;
        cout << "Categoriia\t\tFrec. Absoluta\tFrec. Relativa" << endl;
        cout << "--------------------------------------------------------" << endl;
        
        for (int i = 0; i < frecuenciasFinales->longitud(); i++) {
            FrecuenciaCategoria categoria = frecuenciasFinales->obtener(i);
            cout << categoria.getCategoria() << "\t\t\t" 
                 << categoria.getFrecuenciaAbsoluta() << "\t\t" 
                 << categoria.getFrecuenciaRelativa() << endl;
        }
        
        delete frecuencias;
        delete frecuenciasFinales;
    }

    void calcularPorcentajeValoresFaltantes() {
        cout << "\n--- Valores faltantes ---" << endl;
        
        // AColita para procesar cada columna
        for (int col = 0; col < numColumnas; col++) {
            string nombreVariable = nombreColumnas->obtener(col);
            
            // Crear cola para almacenar todos los valores de la columna
            ACola<string>* colaValores = new ACola<string>(numFilas);
            
            // Llenar la cola con todos los valores de la columna
            for (int fila = 0; fila < numFilas; fila++) {
                LLista<string>* filaActual = datosFilas->obtener(fila);
                string valor = filaActual->obtener(col);
                colaValores->enqueue(valor);
            }
            
            // Procesar la cola para contar valores faltantes
            int valoresFaltantes = 0;
            int totalValores = colaValores->longitud();
            
            // Procesar todos los elementos de la cola
            while (!colaValores->estaVacia()) {
                string valor = colaValores->dequeue();
                
                // Verificar si el valor está faltante
                if (valor.empty() || valor == "NA" || valor == "NULL" || valor == "\"\"" || valor == "\"") {
                    valoresFaltantes++;
                }
            }
            
            double porcentajeFaltantes = (double)valoresFaltantes / totalValores * 100.0;
            
            cout << nombreVariable << ": " 
                 << valoresFaltantes << "/" << totalValores 
                 << " (" << porcentajeFaltantes << "%)" << endl;
            
            delete colaValores;
        }
    }

    void calcularRangoIntercuartilico() {
        cout << "\n--- Rango intercuartilico (IQR) ---" << endl;
        cout << "Seleccione la variable numerica:" << endl;
        
        // Mostrar variables disponibles
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        int columnaSeleccionada = opcion - 1;
        string nombreVariable = nombreColumnas->obtener(columnaSeleccionada);
        
        // Usar APila para almacenar valores numéricos y ordenarlos
        APila<double>* valores = new APila<double>(numFilas);
        
        // Convertir valores a números y almacenar en pila
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valorStr = fila->obtener(columnaSeleccionada);
            
            if (esNumerico(valorStr)) {
                try {
                    string valorLimpio = limpiarValorNumerico(valorStr);
                    double valor = stod(valorLimpio);
                    valores->push(valor);
                } catch (const exception& e) {
                    cout << "Advertencia: Error al convertir valor: " << valorStr << endl;
                }
            }
        }
        
        if (valores->longitud() == 0) {
            cout << "No se encontraron valores numericos validos." << endl;
            delete valores;
            return;
        }
        
        // Ordenar valores usando mergeSort
        valores->ordenarMergeSort();
        
        // Calcular posiciones de cuartiles
        int n = valores->longitud();
        int posQ1 = n / 4;
        int posQ3 = (3 * n) / 4;
        
        double Q1 = valores->obtener(posQ1);
        double Q3 = valores->obtener(posQ3);
        double IQR = Q3 - Q1;
        
        cout << "\nResultados para la variable: " << nombreVariable << endl;
        cout << "Numero de valores validos: " << n << endl;
        cout << "Q1 (Primer cuartil): " << Q1 << endl;
        cout << "Q3 (Tercer cuartil): " << Q3 << endl;
        cout << "IQR (Rango Intercuartilico): " << IQR << endl;
        
        delete valores;
    }

    void calcularCoeficienteVariacion() {
        cout << "\n--- Coef. de variacion ---" << endl;
        cout << "Seleccione la variable numerica:" << endl;
        
        // Mostrar variables disponibles
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        int columnaSeleccionada = opcion - 1;
        string nombreVariable = nombreColumnas->obtener(columnaSeleccionada);
        
        // Usar arreglo para múltiples pasadas
        double* valores = new double[numFilas];
        int countValores = 0;
        
        // Primera pasada: recopilar valores válidos
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valorStr = fila->obtener(columnaSeleccionada);
            
            if (esNumerico(valorStr)) {
                try {
                    string valorLimpio = limpiarValorNumerico(valorStr);
                    double valor = stod(valorLimpio);
                    valores[countValores++] = valor;
                } catch (const exception& e) {
                    // Ignorar valores no convertibles
                }
            }
        }
        
        if (countValores == 0) {
            cout << "No se encontraron valores numericos validos." << endl;
            delete[] valores;
            return;
        }
        
        // Segunda pasada: calcular media
        double suma = 0.0;
        for (int i = 0; i < countValores; i++) {
            suma += valores[i];
        }
        double media = suma / countValores;
        
        // Tercera pasada: calcular desviación estándar
        double sumaCuadrados = 0.0;
        for (int i = 0; i < countValores; i++) {
            double diferencia = valores[i] - media;
            sumaCuadrados += diferencia * diferencia;
        }
        double varianza = sumaCuadrados / countValores;
        double desviacionEstandar = sqrt(varianza); // aqui el uso de la libreria cmath para calcular la raiz
        
        // Calcular coeficiente de variación
        double coeficienteVariacion = (media != 0) ? desviacionEstandar / abs(media) : 0;
        
        cout << "\nResultados para la variable: " << nombreVariable << endl;
        cout << "Numero de valores validos: " << countValores << endl;
        cout << "Media: " << media << endl;
        cout << "Desviacion Estandar: " << desviacionEstandar << endl;
        cout << "Coeficiente de Variacion: " << coeficienteVariacion << endl;
        cout << "CV como porcentaje: " << (coeficienteVariacion * 100) << "%" << endl;
        
        delete[] valores;
    }
        void calcularChiCuadrado() {
        cout << "\n--- Chi-cuadrado (Categorica vs Categorica) ---" << endl;
        cout << "Seleccione la primera variable categorica:" << endl;
        
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion1;
        cout << "Ingrese su opcion: ";
        cin >> opcion1;
        
        if (opcion1 < 1 || opcion1 > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        cout << "Seleccione la segunda variable categorica:" << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion2;
        cout << "Ingrese su opcion: ";
        cin >> opcion2;
        
        if (opcion2 < 1 || opcion2 > numColumnas || opcion2 == opcion1) {
            cout << "Opcion invalida o igual a la primera variable." << endl;
            return;
        }
        
        int col1 = opcion1 - 1;
        int col2 = opcion2 - 1;
        string nombreVar1 = nombreColumnas->obtener(col1);
        string nombreVar2 = nombreColumnas->obtener(col2);
        
        // Usar LLista para construir tabla de contingencia
        LLista<string>* categorias1 = new LLista<string>();
        LLista<string>* categorias2 = new LLista<string>();
        
        // Obtener categorías únicas para ambas variables
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string val1 = fila->obtener(col1);
            string val2 = fila->obtener(col2);
            
            if (!categorias1->buscar(val1)) categorias1->insertar(val1);
            if (!categorias2->buscar(val2)) categorias2->insertar(val2);
        }
        
        int numCat1 = categorias1->longitud();
        int numCat2 = categorias2->longitud();
        
        // Crear tabla de contingencia usando LLista
        LLista<LLista<int>*>* tablaContingencia = new LLista<LLista<int>*>();
        
        for (int i = 0; i < numCat1; i++) {
            LLista<int>* fila = new LLista<int>();
            for (int j = 0; j < numCat2; j++) {
                fila->insertar(0);
            }
            tablaContingencia->insertar(fila);
        }
        
        // Llenar tabla de contingencia
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string val1 = fila->obtener(col1);
            string val2 = fila->obtener(col2);
            
            int idx1 = -1, idx2 = -1;
            for (int j = 0; j < numCat1; j++) {
                if (categorias1->obtener(j) == val1) {
                    idx1 = j;
                    break;
                }
            }
            for (int j = 0; j < numCat2; j++) {
                if (categorias2->obtener(j) == val2) {
                    idx2 = j;
                    break;
                }
            }
            
            if (idx1 != -1 && idx2 != -1) {
                LLista<int>* filaTabla = tablaContingencia->obtener(idx1);
                int valorActual = filaTabla->obtener(idx2);
                // Como no podemos modificar directamente, reconstruimos la fila
                LLista<int>* nuevaFila = new LLista<int>();
                for (int k = 0; k < numCat2; k++) {
                    if (k == idx2) {
                        nuevaFila->insertar(valorActual + 1);
                    } else {
                        nuevaFila->insertar(filaTabla->obtener(k));
                    }
                }
                delete filaTabla;
                // Reemplazar en la posición idx1
                tablaContingencia->obtener(idx1) = nuevaFila;
            }
        }
        
        // Calcular Chi-cuadrado
        double chiCuadrado = 0.0;
        
        // Calcular totales
        ALista<int>* totalesFila = new ALista<int>();
        ALista<int>* totalesColumna = new ALista<int>();
        
        for (int i = 0; i < numCat1; i++) totalesFila->insertar(0);
        for (int j = 0; j < numCat2; j++) totalesColumna->insertar(0);
        
        int totalGeneral = 0;
        
        for (int i = 0; i < numCat1; i++) {
            LLista<int>* fila = tablaContingencia->obtener(i);
            int sumaFila = 0;
            for (int j = 0; j < numCat2; j++) {
                int valor = fila->obtener(j);
                sumaFila += valor;
                totalesColumna->operator[](j) += valor;
            }
            totalesFila->operator[](i) = sumaFila;
            totalGeneral += sumaFila;
        }
        
        // Calcular estadístico Chi-cuadrado
        for (int i = 0; i < numCat1; i++) {
            LLista<int>* fila = tablaContingencia->obtener(i);
            for (int j = 0; j < numCat2; j++) {
                int observado = fila->obtener(j);
                double esperado = (double)(totalesFila->obtener(i) * totalesColumna->obtener(j)) / totalGeneral;
                
                if (esperado > 0) {
                    chiCuadrado += pow(observado - esperado, 2) / esperado;
                }
            }
        }
        
        cout << "\nResultados Chi-cuadrado:" << endl;
        cout << "Variables: " << nombreVar1 << " vs " << nombreVar2 << endl;
        cout << "Estadistico Chi-cuadrado: " << chiCuadrado << endl;
        cout << "Grados de libertad: " << (numCat1 - 1) * (numCat2 - 1) << endl;
        
        // Limpiar memoria
        for (int i = 0; i < numCat1; i++) {
            delete tablaContingencia->obtener(i);
        }
        delete tablaContingencia;
        delete categorias1;
        delete categorias2;
        delete totalesFila;
        delete totalesColumna;
    }
    
    void calcularCorrelacionPearson() {
        cout << "\n--- Correlacion de Pearson (Numerica vs Numerica) ---" << endl;
        cout << "Seleccione la primera variable numerica:" << endl;
        
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion1;
        cout << "Ingrese su opcion: ";
        cin >> opcion1;
        
        if (opcion1 < 1 || opcion1 > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        cout << "Seleccione la segunda variable numerica:" << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion2;
        cout << "Ingrese su opcion: ";
        cin >> opcion2;
        
        if (opcion2 < 1 || opcion2 > numColumnas || opcion2 == opcion1) {
            cout << "Opcion invalida o igual a la primera variable." << endl;
            return;
        }
        
        int col1 = opcion1 - 1;
        int col2 = opcion2 - 1;
        string nombreVar1 = nombreColumnas->obtener(col1);
        string nombreVar2 = nombreColumnas->obtener(col2);
        
        // Usar ALista para almacenar pares (xi, yi) alineados
        ALista<double>* valoresX = new ALista<double>();
        ALista<double>* valoresY = new ALista<double>();
        
        // Recopilar valores válidos
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valStr1 = fila->obtener(col1);
            string valStr2 = fila->obtener(col2);
            
            if (esNumerico(valStr1) && esNumerico(valStr2)) {
                try {
                    double val1 = stod(limpiarValorNumerico(valStr1));
                    double val2 = stod(limpiarValorNumerico(valStr2));
                    valoresX->insertar(val1);
                    valoresY->insertar(val2);
                } catch (const exception& e) {
                    // Ignorar valores no convertibles
                }
            }
        }
        
        int n = valoresX->longitud();
        if (n < 2) {
            cout << "No hay suficientes pares de valores validos para calcular correlacion." << endl;
            delete valoresX;
            delete valoresY;
            return;
        }
        
        // Calcular medias
        double sumaX = 0, sumaY = 0;
        for (int i = 0; i < n; i++) {
            sumaX += valoresX->obtener(i);
            sumaY += valoresY->obtener(i);
        }
        double mediaX = sumaX / n;
        double mediaY = sumaY / n;
        
        // Calcular covarianza y varianzas
        double covarianza = 0, varianzaX = 0, varianzaY = 0;
        for (int i = 0; i < n; i++) {
            double difX = valoresX->obtener(i) - mediaX;
            double difY = valoresY->obtener(i) - mediaY;
            covarianza += difX * difY;
            varianzaX += difX * difX;
            varianzaY += difY * difY;
        }
        
        double desviacionX = sqrt(varianzaX / (n - 1));
        double desviacionY = sqrt(varianzaY / (n - 1));
        
        // Calcular correlación de Pearson
        double correlacion = 0;
        if (desviacionX > 0 && desviacionY > 0) {
            correlacion = (covarianza / (n - 1)) / (desviacionX * desviacionY);
        }
        
        cout << "\nResultados Correlacion de Pearson:" << endl;
        cout << "Variables: " << nombreVar1 << " vs " << nombreVar2 << endl;
        cout << "Numero de pares validos: " << n << endl;
        cout << "Media " << nombreVar1 << ": " << mediaX << endl;
        cout << "Media " << nombreVar2 << ": " << mediaY << endl;
        cout << "Correlacion de Pearson: " << correlacion << endl;
        
        delete valoresX;
        delete valoresY;
    }
    
    void calcularANOVA() {
        cout << "\n--- ANOVA (Categorica vs Numerica) ---" << endl;
        cout << "Seleccione la variable categorica (factor):" << endl;
        
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcionCat;
        cout << "Ingrese su opcion: ";
        cin >> opcionCat;
        
        if (opcionCat < 1 || opcionCat > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        cout << "Seleccione la variable numerica (respuesta):" << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcionNum;
        cout << "Ingrese su opcion: ";
        cin >> opcionNum;
        
        if (opcionNum < 1 || opcionNum > numColumnas || opcionNum == opcionCat) {
            cout << "Opcion invalida o igual a la variable categorica." << endl;
            return;
        }
        
        int colCat = opcionCat - 1;
        int colNum = opcionNum - 1;
        string nombreVarCat = nombreColumnas->obtener(colCat);
        string nombreVarNum = nombreColumnas->obtener(colNum);
        
        // Usar LCola para procesar datos por grupos en orden FIFO
        LLista<string>* categorias = new LLista<string>();
        LLista<ALista<double>*>* gruposDatos = new LLista<ALista<double>*>();
        
        // Identificar categorías únicas
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string categoria = fila->obtener(colCat);
            
            if (!categorias->buscar(categoria)) {
                categorias->insertar(categoria);
                gruposDatos->insertar(new ALista<double>());
            }
        }
        
        // Agrupar datos numéricos por categoría
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string categoria = fila->obtener(colCat);
            string valorNumStr = fila->obtener(colNum);
            
            if (esNumerico(valorNumStr)) {
                try {
                    double valorNum = stod(limpiarValorNumerico(valorNumStr));
                    
                    // Encontrar índice de la categoría
                    for (int j = 0; j < categorias->longitud(); j++) {
                        if (categorias->obtener(j) == categoria) {
                            gruposDatos->obtener(j)->insertar(valorNum);
                            break;
                        }
                    }
                } catch (const exception& e) {
                    // Ignorar valores no convertibles
                }
            }
        }
        
        // Calcular medias de cada grupo y media general
        ALista<double>* mediasGrupos = new ALista<double>();
        ALista<int>* tamañosGrupos = new ALista<int>();
        double sumaTotal = 0;
        int nTotal = 0;
        
        for (int i = 0; i < categorias->longitud(); i++) {
            ALista<double>* grupo = gruposDatos->obtener(i);
            double sumaGrupo = 0;
            int nGrupo = grupo->longitud();
            
            for (int j = 0; j < nGrupo; j++) {
                sumaGrupo += grupo->obtener(j);
            }
            
            double mediaGrupo = (nGrupo > 0) ? sumaGrupo / nGrupo : 0;
            mediasGrupos->insertar(mediaGrupo);
            tamañosGrupos->insertar(nGrupo);
            
            sumaTotal += sumaGrupo;
            nTotal += nGrupo;
        }
        
        double mediaGeneral = (nTotal > 0) ? sumaTotal / nTotal : 0;
        
        // Calcular suma de cuadrados entre grupos (SSB)
        double SSB = 0;
        for (int i = 0; i < categorias->longitud(); i++) {
            int ni = tamañosGrupos->obtener(i);
            double mediaGrupo = mediasGrupos->obtener(i);
            SSB += ni * pow(mediaGrupo - mediaGeneral, 2);
        }
        
        // Calcular suma de cuadrados dentro de grupos (SSW)
        double SSW = 0;
        for (int i = 0; i < categorias->longitud(); i++) {
            ALista<double>* grupo = gruposDatos->obtener(i);
            double mediaGrupo = mediasGrupos->obtener(i);
            
            for (int j = 0; j < grupo->longitud(); j++) {
                SSW += pow(grupo->obtener(j) - mediaGrupo, 2);
            }
        }
        
        // Calcular grados de libertad
        int k = categorias->longitud(); // número de grupos
        int dfB = k - 1; // grados de libertad entre grupos
        int dfW = nTotal - k; // grados de libertad dentro de grupos
        
        // Calcular cuadrados medios
        double MSB = (dfB > 0) ? SSB / dfB : 0;
        double MSW = (dfW > 0) ? SSW / dfW : 0;
        
        // Calcular estadístico F
        double F = (MSW > 0) ? MSB / MSW : 0;
        
        cout << "\nResultados ANOVA:" << endl;
        cout << "Variable categorica: " << nombreVarCat << endl;
        cout << "Variable numerica: " << nombreVarNum << endl;
        cout << "Numero de grupos: " << k << endl;
        cout << "Numero total de observaciones: " << nTotal << endl;
        cout << "Media general: " << mediaGeneral << endl;
        cout << "\nMedias por grupo:" << endl;
        for (int i = 0; i < categorias->longitud(); i++) {
            cout << categorias->obtener(i) << ": " << mediasGrupos->obtener(i) 
                 << " (n=" << tamañosGrupos->obtener(i) << ")" << endl;
        }
        cout << "\nSuma de cuadrados entre grupos (SSB): " << SSB << endl;
        cout << "Suma de cuadrados dentro de grupos (SSW): " << SSW << endl;
        cout << "Cuadrado medio entre grupos (MSB): " << MSB << endl;
        cout << "Cuadrado medio dentro de grupos (MSW): " << MSW << endl;
        cout << "Estadistico F: " << F << endl;
        cout << "Grados de libertad: " << dfB << ", " << dfW << endl;
        
        // Limpiar memoria
        for (int i = 0; i < gruposDatos->longitud(); i++) {
            delete gruposDatos->obtener(i);
        }
        delete categorias;
        delete gruposDatos;
        delete mediasGrupos;
        delete tamañosGrupos;
    }
    
    void calcularRegresionLinealMultiple() {
        cout << "\n--- Regresion Lineal Multiple ---" << endl;
        cout << "Seleccione la variable dependiente (Y):" << endl;
        
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcionY;
        cout << "Ingrese su opcion: ";
        cin >> opcionY;
        
        if (opcionY < 1 || opcionY > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        cout << "Cuantas variables independientes desea incluir? ";
        int numVarsX;
        cin >> numVarsX;
        
        if (numVarsX < 1 || numVarsX >= numColumnas) {
            cout << "Numero invalido de variables independientes." << endl;
            return;
        }
        
        ALista<int>* columnasX = new ALista<int>();
        ALista<string>* nombresX = new ALista<string>();
        
        cout << "Seleccione las variables independientes:" << endl;
        for (int v = 0; v < numVarsX; v++) {
            cout << "Variable " << (v + 1) << ":" << endl;
            for (int i = 0; i < nombreColumnas->longitud(); i++) {
                cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
            }
            
            int opcion;
            cout << "Ingrese su opcion: ";
            cin >> opcion;
            
            if (opcion < 1 || opcion > numColumnas || opcion == opcionY) {
                cout << "Opcion invalida o igual a la variable dependiente." << endl;
                delete columnasX;
                delete nombresX;
                return;
            }
            
            columnasX->insertar(opcion - 1);
            nombresX->insertar(nombreColumnas->obtener(opcion - 1));
        }
        
        int colY = opcionY - 1;
        string nombreY = nombreColumnas->obtener(colY);
        
        // Recopilar datos válidos usando ALista
        ALista<ALista<double>*>* matrizX = new ALista<ALista<double>*>();
        ALista<double>* vectorY = new ALista<double>();
        
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valorYStr = fila->obtener(colY);
            
            bool filaValida = esNumerico(valorYStr);
            ALista<double>* filaX = new ALista<double>();
            filaX->insertar(1.0); // Término independiente
            
            for (int j = 0; j < numVarsX && filaValida; j++) {
                string valorXStr = fila->obtener(columnasX->obtener(j));
                if (esNumerico(valorXStr)) {
                    try {
                        double valorX = stod(limpiarValorNumerico(valorXStr));
                        filaX->insertar(valorX);
                    } catch (const exception& e) {
                        filaValida = false;
                    }
                } else {
                    filaValida = false;
                }
            }
            
            if (filaValida) {
                try {
                    double valorY = stod(limpiarValorNumerico(valorYStr));
                    matrizX->insertar(filaX);
                    vectorY->insertar(valorY);
                } catch (const exception& e) {
                    delete filaX;
                }
            } else {
                delete filaX;
            }
        }
        
        int n = vectorY->longitud();
        int p = numVarsX + 1; // +1 para el término independiente
        
        if (n < p) {
            cout << "No hay suficientes observaciones validas para el modelo." << endl;
            // Limpiar memoria
            for (int i = 0; i < matrizX->longitud(); i++) {
                delete matrizX->obtener(i);
            }
            delete matrizX;
            delete vectorY;
            delete columnasX;
            delete nombresX;
            return;
        }
        
        // Calcular X'X (simplificado para el ejemplo)
        ALista<ALista<double>*>* XtX = new ALista<ALista<double>*>();
        for (int i = 0; i < p; i++) {
            ALista<double>* fila = new ALista<double>();
            for (int j = 0; j < p; j++) {
                double suma = 0;
                for (int k = 0; k < n; k++) {
                    suma += matrizX->obtener(k)->obtener(i) * matrizX->obtener(k)->obtener(j);
                }
                fila->insertar(suma);
            }
            XtX->insertar(fila);
        }
        
        // Calcular X'Y
        ALista<double>* XtY = new ALista<double>();
        for (int i = 0; i < p; i++) {
            double suma = 0;
            for (int j = 0; j < n; j++) {
                suma += matrizX->obtener(j)->obtener(i) * vectorY->obtener(j);
            }
            XtY->insertar(suma);
        }
        
        // Para simplicidad, calcular solo algunos estadísticos básicos
        double sumaY = 0, sumaY2 = 0;
        for (int i = 0; i < n; i++) {
            double y = vectorY->obtener(i);
            sumaY += y;
            sumaY2 += y * y;
        }
        double mediaY = sumaY / n;
        double SST = sumaY2 - n * mediaY * mediaY;
        
        cout << "\nResultados Regresion Lineal Multiple:" << endl;
        cout << "Variable dependiente: " << nombreY << endl;
        cout << "Variables independientes: ";
        for (int i = 0; i < numVarsX; i++) {
            cout << nombresX->obtener(i);
            if (i < numVarsX - 1) cout << ", ";
        }
        cout << endl;
        cout << "Numero de observaciones: " << n << endl;
        cout << "Numero de parametros: " << p << endl;
        cout << "Media de Y: " << mediaY << endl;
        cout << "Suma total de cuadrados (SST): " << SST << endl;
        
        cout << "\nNota: Calculo completo de coeficientes requiere inversion de matriz." << endl;
        cout << "X'Y (lado derecho del sistema):" << endl;
        for (int i = 0; i < p; i++) {
            cout << "Parametro " << i << ": " << XtY->obtener(i) << endl;
        }
        
        // Limpiar memoria
        for (int i = 0; i < matrizX->longitud(); i++) {
            delete matrizX->obtener(i);
        }
        delete matrizX;
        delete vectorY;
        delete columnasX;
        delete nombresX;
        for (int i = 0; i < XtX->longitud(); i++) {
            delete XtX->obtener(i);
        }
        delete XtX;
        delete XtY;
    }
};

/// funcion principal
void mostrarMenu() {
    cout << "\n=== ANALISIS ESTADISTICO DE DATASETS ===" << endl;
    cout << "--- Tecnicas Univariadas ---" << endl;
    cout << "1. Vista previa del dataset" << endl;
    cout << "2. Frecuencia absoluta y relativa (Variables Categoricas)" << endl;
    cout << "3. Porcentaje de valores faltantes" << endl;
    cout << "4. Rango intercuartilico - IQR (Variables Numericas)" << endl;
    cout << "5. Coeficiente de variacion (Variables Numericas)" << endl;
    cout << "\n--- Tecnicas Bivariadas ---" << endl;
    cout << "6. Chi-cuadrado (Categorica vs Categorica)" << endl;
    cout << "7. Correlacion de Pearson (Numerica vs Numerica)" << endl;
    cout << "8. ANOVA (Categorica vs Numerica)" << endl;
    cout << "\n--- Tecnicas Multivariadas ---" << endl;
    cout << "9. Regresion Lineal Multiple" << endl;
    cout << "\n10. Salir" << endl;
    cout << "===============================================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    cout << "--- AED aplicado a Datasets ---" << endl;
    
    AnalizadorDataset analizador;
    string rutaArchivo;
    
    cout << "Ingrese la ruta del archivo CSV: "; // /Users/usuario/Documents/C++/dataset01.csv
    getline(cin, rutaArchivo);
    
    if (!analizador.cargarCSV(rutaArchivo)) {
        return 1;
    }
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                analizador.mostrarVistaPrevia();
                break;
            case 2:
                analizador.calcularFrecuenciaAbsolutaRelativa();
                break;
            case 3:
                analizador.calcularPorcentajeValoresFaltantes();
                break;
            case 4:
                analizador.calcularRangoIntercuartilico();
                break;
            case 5:
                analizador.calcularCoeficienteVariacion();
                break;
            case 6:
                analizador.calcularChiCuadrado();
                break;
            case 7:
                analizador.calcularCorrelacionPearson();
                break;
            case 8:
                analizador.calcularANOVA();
                break;
            case 9:
                analizador.calcularRegresionLinealMultiple();
                break;
            case 10:
                cout << "¡Gracias por usar el programa!" << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
        
        if (opcion != 10) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 10);
    
    return 0;
}