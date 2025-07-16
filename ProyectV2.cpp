// ProyectV.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "ProyectV2.h"

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

    LLista<string>* parsearLineaCSV(const string& linea) {
        LLista<string>* campos = new LLista<string>();
        string campo = "";
        bool dentroComillas = false;
        
        for (size_t i = 0; i < linea.length(); i++) {
            char c = linea[i];
            
            if (c == '"') {
                dentroComillas = !dentroComillas;
            } else if (c == ',' && !dentroComillas) {
                campos->insertar(campo);
                campo = "";
            } else {
                campo += c;
            }
        }
        
        if (!campo.empty() || linea.back() == ',') {
            campos->insertar(campo);
        }
        
        return campos;
    }

    string limpiarValorNumerico(const string& valor) {
        string valorLimpio = valor;
        if (valorLimpio.length() >= 2 && valorLimpio.front() == '"' && valorLimpio.back() == '"') {
            valorLimpio = valorLimpio.substr(1, valorLimpio.length() - 2);
        }
        for (char& c : valorLimpio) {
            if (c == ',') c = '.';
        }
        return valorLimpio;
    }

    bool esNumerico(const string& valor) {
        string valorLimpio = limpiarValorNumerico(valor);
        if (valorLimpio.empty()) return false;
        try {
            stod(valorLimpio);
            return true;
        } catch (const exception& e) {
            return false;
        }
    }

    int seleccionarVariable(const string& prompt) {
        cout << prompt << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (cin.fail() || opcion < 1 || opcion > numColumnas) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Opcion invalida." << endl;
            return -1;
        }
        return opcion - 1;
    }

    LLista<string>* obtenerCategoriasUnicas(int indiceColumna) {
        LLista<string>* categoriasUnicas = new LLista<string>();
        for (int i = 0; i < numFilas; i++) {
            string valor = datosFilas->obtener(i)->obtener(indiceColumna);
            if (!categoriasUnicas->buscar(valor)) {
                categoriasUnicas->insertar(valor);
            }
        }
        return categoriasUnicas;
    }

    int obtenerIndiceCategoria(LLista<string>* categorias, const string& valor) {
        for (int i = 0; i < categorias->longitud(); i++) {
            if (categorias->obtener(i) == valor) {
                return i;
            }
        }
        return -1;
    }

    // --- Helpers para Regresión Lineal ---
    double** crearMatriz(int filas, int cols) {
        double** mat = new double*[filas];
        for (int i = 0; i < filas; ++i) {
            mat[i] = new double[cols];
            for (int j = 0; j < cols; ++j) {
                mat[i][j] = 0.0;
            }
        }
        return mat;
    }

    void liberarMatriz(double** mat, int filas) {
        if (!mat) return;
        for (int i = 0; i < filas; ++i) {
            delete[] mat[i];
        }
        delete[] mat;
    }

    double** transponerMatriz(double** mat, int filas, int cols) {
        double** transpuesta = crearMatriz(cols, filas);
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < cols; ++j) {
                transpuesta[j][i] = mat[i][j];
            }
        }
        return transpuesta;
    }

    double** multiplicarMatrices(double** matA, int fA, int cA, double** matB, int fB, int cB) {
        if (cA != fB) return nullptr;
        double** resultado = crearMatriz(fA, cB);
        for (int i = 0; i < fA; ++i) {
            for (int j = 0; j < cB; ++j) {
                for (int k = 0; k < cA; ++k) {
                    resultado[i][j] += matA[i][k] * matB[k][j];
                }
            }
        }
        return resultado;
    }

    double determinante3x3(double** mat) {
        double det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
                     mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
                     mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
        return det;
    }

    double** invertirMatriz3x3(double** mat) {
        double det = determinante3x3(mat);
        if (abs(det) < 1e-9) return nullptr; // No invertible

        double** inversa = crearMatriz(3, 3);
        double invDet = 1.0 / det;

        inversa[0][0] = (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) * invDet;
        inversa[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * invDet;
        inversa[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invDet;
        inversa[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * invDet;
        inversa[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invDet;
        inversa[1][2] = (mat[1][0] * mat[0][2] - mat[0][0] * mat[1][2]) * invDet;
        inversa[2][0] = (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) * invDet;
        inversa[2][1] = (mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1]) * invDet;
        inversa[2][2] = (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) * invDet;

        return inversa;
    }


public:
    AnalizadorDataset() {
        nombreColumnas = new LLista<string>();
        datosFilas = new LLista<LLista<string>*>();
        numFilas = 0;
        numColumnas = 0;
    }

    ~AnalizadorDataset() {
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

        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            LLista<string>* fila = parsearLineaCSV(linea);

            if (primeraLinea) {
                for (int i = 0; i < fila->longitud(); i++) {
                    nombreColumnas->insertar(fila->obtener(i));
                }
                numColumnas = fila->longitud();
                delete fila;
                primeraLinea = false;
            } else {
                if (fila->longitud() == numColumnas) {
                    datosFilas->insertar(fila);
                    numFilas++;
                } else {
                    delete fila;
                }
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
        cout << "Numero de registros: " << numFilas << endl;
        cout << "Numero de variables: " << numColumnas << endl;
        
        cout << "\nVariables disponibles:" << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        cout << "\nPrimeras 5 filas:" << endl;
        for (int j = 0; j < nombreColumnas->longitud(); j++) {
            cout << nombreColumnas->obtener(j) << "\t";
        }
        cout << endl;
        
        int filasAMostrar = min(5, numFilas);
        for (int i = 0; i < filasAMostrar; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            for (int j = 0; j < fila->longitud(); j++) {
                cout << fila->obtener(j) << "\t";
            }
            cout << endl;
        }
        cout << "======================================================" << endl;
    }

    void calcularFrecuenciaAbsolutaRelativa() {
        cout << "\n --- Frecuencia absoluta y relativa ---" << endl;
        int col = seleccionarVariable("Seleccione la variable categorica:");
        if (col == -1) return;
        
        string nombreVariable = nombreColumnas->obtener(col);
        
        LLista<FrecuenciaCategoria>* frecuencias = new LLista<FrecuenciaCategoria>();
        
        for (int i = 0; i < numFilas; i++) {
            string valor = datosFilas->obtener(i)->obtener(col);
            
            bool encontrado = false;
            for (int j = 0; j < frecuencias->longitud(); j++) {
                FrecuenciaCategoria categoria = frecuencias->obtener(j);
                if (categoria.getCategoria() == valor) {
                    encontrado = true;
                    // No podemos modificar en LLista, reconstruimos
                    break;
                }
            }
            
            if (!encontrado) {
                frecuencias->insertar(FrecuenciaCategoria(valor));
            }
        }
        
        LLista<FrecuenciaCategoria>* frecuenciasFinales = new LLista<FrecuenciaCategoria>();
        for (int i = 0; i < frecuencias->longitud(); i++) {
            FrecuenciaCategoria categoria = frecuencias->obtener(i);
            int contador = 0;
            for (int j = 0; j < numFilas; j++) {
                if (datosFilas->obtener(j)->obtener(col) == categoria.getCategoria()) {
                    contador++;
                }
            }
            
            categoria.setFrecuenciaAbsoluta(contador);
            categoria.setFrecuenciaRelativa((double)contador / numFilas);
            frecuenciasFinales->insertar(categoria);
        }
        
        cout << "\nFrecuencias para la variable: " << nombreVariable << endl;
        cout << "Categoria\t\tFrec. Absoluta\tFrec. Relativa" << endl;
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
        cout << "\n--- Porcentaje de valores faltantes ---" << endl;
        
        for (int col = 0; col < numColumnas; col++) {
            ACola<string> colaValores(numFilas);
            
            for (int fila = 0; fila < numFilas; fila++) {
                colaValores.push(datosFilas->obtener(fila)->obtener(col));
            }
            
            int valoresFaltantes = 0;
            int totalValores = colaValores.longitud();
            
            while (!colaValores.estaVacia()) {
                string valor = colaValores.pop();
                if (valor.empty() || valor == "NA" || valor == "NULL" || valor == "\"\"") {
                    valoresFaltantes++;
                }
            }
            
            double porcentajeFaltantes = (totalValores > 0) ? (double)valoresFaltantes / totalValores * 100.0 : 0.0;
            
            cout << nombreColumnas->obtener(col) << ": " 
                 << valoresFaltantes << "/" << totalValores 
                 << " (" << porcentajeFaltantes << "%)" << endl;
        }
    }

    void calcularRangoIntercuartilico() {
        cout << "\n--- Rango intercuartilico (IQR) ---" << endl;
        int col = seleccionarVariable("Seleccione la variable numerica:");
        if (col == -1) return;
        
        APila<double> valores(numFilas);
        
        for (int i = 0; i < numFilas; i++) {
            string valorStr = datosFilas->obtener(i)->obtener(col);
            if (esNumerico(valorStr)) {
                valores.push(stod(limpiarValorNumerico(valorStr)));
            }
        }
        
        if (valores.longitud() == 0) {
            cout << "No se encontraron valores numericos validos." << endl;
            return;
        }
        
        valores.ordenarMergeSort();
        
        int n = valores.longitud();
        double q1 = valores.obtener(n / 4);
        double q3 = valores.obtener((3 * n) / 4);
        double iqr = q3 - q1;
        
        cout << "\nResultados para la variable: " << nombreColumnas->obtener(col) << endl;
        cout << "Q1 (Primer cuartil): " << q1 << endl;
        cout << "Q3 (Tercer cuartil): " << q3 << endl;
        cout << "IQR (Rango Intercuartilico): " << iqr << endl;
    }

    void calcularCoeficienteVariacion() {
        cout << "\n--- Coeficiente de variacion ---" << endl;
        int col = seleccionarVariable("Seleccione la variable numerica:");
        if (col == -1) return;

        double* valores = new double[numFilas];
        int countValores = 0;
        
        for (int i = 0; i < numFilas; i++) {
            string valorStr = datosFilas->obtener(i)->obtener(col);
            if (esNumerico(valorStr)) {
                valores[countValores++] = stod(limpiarValorNumerico(valorStr));
            }
        }
        
        if (countValores == 0) {
            cout << "No se encontraron valores numericos validos." << endl;
            delete[] valores;
            return;
        }
        
        double suma = 0.0;
        for (int i = 0; i < countValores; i++) suma += valores[i];
        double media = suma / countValores;
        
        double sumaCuadrados = 0.0;
        for (int i = 0; i < countValores; i++) sumaCuadrados += pow(valores[i] - media, 2);
        double desviacionEstandar = sqrt(sumaCuadrados / countValores);
        
        double cv = (media != 0) ? desviacionEstandar / abs(media) : 0;
        
        cout << "\nResultados para la variable: " << nombreColumnas->obtener(col) << endl;
        cout << "Media: " << media << endl;
        cout << "Desviacion Estandar: " << desviacionEstandar << endl;
        cout << "Coeficiente de Variacion: " << cv * 100 << "%" << endl;
        
        delete[] valores;
    }

    void calcularChiCuadrado() {
        cout << "\n--- Chi-cuadrado (Categórica vs. Categórica) ---" << endl;
        int col1 = seleccionarVariable("Seleccione la primera variable categorica:");
        if (col1 == -1) return;
        int col2 = seleccionarVariable("Seleccione la segunda variable categorica:");
        if (col2 == -1 || col1 == col2) {
            cout << "Seleccion invalida." << endl;
            return;
        }

        LLista<string>* cats1 = obtenerCategoriasUnicas(col1);
        LLista<string>* cats2 = obtenerCategoriasUnicas(col2);

        int n_cats1 = cats1->longitud();
        int n_cats2 = cats2->longitud();

        int** observado = new int*[n_cats1];
        for(int i = 0; i < n_cats1; ++i) observado[i] = new int[n_cats2]();

        for (int i = 0; i < numFilas; ++i) {
            string val1 = datosFilas->obtener(i)->obtener(col1);
            string val2 = datosFilas->obtener(i)->obtener(col2);
            int idx1 = obtenerIndiceCategoria(cats1, val1);
            int idx2 = obtenerIndiceCategoria(cats2, val2);
            if (idx1 != -1 && idx2 != -1) {
                observado[idx1][idx2]++;
            }
        }

        int* totalFilas = new int[n_cats1]();
        int* totalCols = new int[n_cats2]();
        int granTotal = 0;

        for (int i = 0; i < n_cats1; ++i) {
            for (int j = 0; j < n_cats2; ++j) {
                totalFilas[i] += observado[i][j];
                totalCols[j] += observado[i][j];
                granTotal += observado[i][j];
            }
        }

        double chi2 = 0.0;
        for (int i = 0; i < n_cats1; ++i) {
            for (int j = 0; j < n_cats2; ++j) {
                double esperado = (double)totalFilas[i] * totalCols[j] / granTotal;
                if (esperado > 0) {
                    chi2 += pow(observado[i][j] - esperado, 2) / esperado;
                }
            }
        }

        cout << "\nEstadistico Chi-cuadrado: " << chi2 << endl;
        cout << "Grados de libertad: " << (n_cats1 - 1) * (n_cats2 - 1) << endl;

        delete cats1;
        delete cats2;
        for(int i = 0; i < n_cats1; ++i) delete[] observado[i];
        delete[] observado;
        delete[] totalFilas;
        delete[] totalCols;
    }

    void calcularCorrelacionPearson() {
        cout << "\n--- Correlacion de Pearson (Numerica vs. Numerica) ---" << endl;
        int col1 = seleccionarVariable("Seleccione la primera variable numerica:");
        if (col1 == -1 || !esNumerico(datosFilas->obtener(0)->obtener(col1))) { cout << "No es numerica." << endl; return; }
        int col2 = seleccionarVariable("Seleccione la segunda variable numerica:");
        if (col2 == -1 || !esNumerico(datosFilas->obtener(0)->obtener(col2)) || col1 == col2) { cout << "Seleccion invalida." << endl; return; }

        double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
        int n = 0;

        for (int i = 0; i < numFilas; ++i) {
            string val1_str = datosFilas->obtener(i)->obtener(col1);
            string val2_str = datosFilas->obtener(i)->obtener(col2);
            if (esNumerico(val1_str) && esNumerico(val2_str)) {
                double x = stod(limpiarValorNumerico(val1_str));
                double y = stod(limpiarValorNumerico(val2_str));
                sum_x += x;
                sum_y += y;
                sum_xy += x * y;
                sum_x2 += x * x;
                sum_y2 += y * y;
                n++;
            }
        }

        if (n < 2) {
            cout << "No hay suficientes datos validos." << endl;
            return;
        }

        double numerador = n * sum_xy - sum_x * sum_y;
        double denominador = sqrt((n * sum_x2 - pow(sum_x, 2)) * (n * sum_y2 - pow(sum_y, 2)));
        
        if (denominador == 0) {
            cout << "La correlacion no se puede calcular (division por cero)." << endl;
            return;
        }
        
        double r = numerador / denominador;
        cout << "\nCoeficiente de correlacion de Pearson (r): " << r << endl;
    }

    void calcularANOVA() {
        cout << "\n--- ANOVA (Categorica vs. Numerica) ---" << endl;
        int col_cat = seleccionarVariable("Seleccione la variable categorica (grupos):");
        if (col_cat == -1) return;
        int col_num = seleccionarVariable("Seleccione la variable numerica:");
        if (col_num == -1 || !esNumerico(datosFilas->obtener(0)->obtener(col_num))) { cout << "No es numerica." << endl; return; }

        LLista<string>* categorias = obtenerCategoriasUnicas(col_cat);
        int k = categorias->longitud();
        if (k < 2) {
            cout << "Se necesitan al menos 2 grupos para ANOVA." << endl;
            delete categorias;
            return;
        }

        LLista<LLista<double>*>* grupos = new LLista<LLista<double>*>();
        for (int i = 0; i < k; ++i) grupos->insertar(new LLista<double>());

        for (int i = 0; i < numFilas; ++i) {
            string val_cat = datosFilas->obtener(i)->obtener(col_cat);
            string val_num_str = datosFilas->obtener(i)->obtener(col_num);
            if (esNumerico(val_num_str)) {
                int idx = obtenerIndiceCategoria(categorias, val_cat);
                if (idx != -1) {
                    grupos->obtener(idx)->insertar(stod(limpiarValorNumerico(val_num_str)));
                }
            }
        }

        double gran_suma = 0;
        int N = 0;
        double* medias_grupos = new double[k]();

        for (int i = 0; i < k; ++i) {
            LLista<double>* grupo_actual = grupos->obtener(i);
            double suma_grupo = 0;
            for (int j = 0; j < grupo_actual->longitud(); ++j) {
                suma_grupo += grupo_actual->obtener(j);
            }
            if (grupo_actual->longitud() > 0) {
                medias_grupos[i] = suma_grupo / grupo_actual->longitud();
            }
            gran_suma += suma_grupo;
            N += grupo_actual->longitud();
        }

        if (N == 0) { cout << "No hay datos." << endl; return; }
        double gran_media = gran_suma / N;

        double ssb = 0; // Sum of squares between
        for (int i = 0; i < k; ++i) {
            ssb += grupos->obtener(i)->longitud() * pow(medias_grupos[i] - gran_media, 2);
        }

        double ssw = 0; // Sum of squares within
        for (int i = 0; i < k; ++i) {
            LLista<double>* grupo_actual = grupos->obtener(i);
            for (int j = 0; j < grupo_actual->longitud(); ++j) {
                ssw += pow(grupo_actual->obtener(j) - medias_grupos[i], 2);
            }
        }

        int df_b = k - 1;
        int df_w = N - k;
        if (df_w <= 0) {
            cout << "No hay suficientes datos para calcular el estadistico F." << endl;
        } else {
            double msb = ssb / df_b;
            double msw = ssw / df_w;
            double f_stat = (msw > 0) ? msb / msw : 0;

            cout << "\nEstadistico F: " << f_stat << endl;
            cout << "Grados de libertad (entre, dentro): " << df_b << ", " << df_w << endl;
        }

        delete categorias;
        for (int i = 0; i < k; ++i) delete grupos->obtener(i);
        delete grupos;
        delete[] medias_grupos;
    }

    void calcularRegresionLinealMultiple() {
        cout << "\n--- Regresion Lineal Multiple (2 variables independientes) ---" << endl;
        int col_y = seleccionarVariable("Seleccione la variable dependiente (Y) numerica:");
        if (col_y == -1 || !esNumerico(datosFilas->obtener(0)->obtener(col_y))) { cout << "No es numerica." << endl; return; }
        int col_x1 = seleccionarVariable("Seleccione la primera variable independiente (X1) numerica:");
        if (col_x1 == -1 || !esNumerico(datosFilas->obtener(0)->obtener(col_x1))) { cout << "No es numerica." << endl; return; }
        int col_x2 = seleccionarVariable("Seleccione la segunda variable independiente (X2) numerica:");
        if (col_x2 == -1 || !esNumerico(datosFilas->obtener(0)->obtener(col_x2)) || col_y == col_x1 || col_y == col_x2 || col_x1 == col_x2) {
            cout << "Seleccion invalida." << endl; return;
        }
        
        LLista<double> y_list, x1_list, x2_list;
        for(int i = 0; i < numFilas; ++i) {
            if(esNumerico(datosFilas->obtener(i)->obtener(col_y)) && esNumerico(datosFilas->obtener(i)->obtener(col_x1)) && esNumerico(datosFilas->obtener(i)->obtener(col_x2))) {
                y_list.insertar(stod(limpiarValorNumerico(datosFilas->obtener(i)->obtener(col_y))));
                x1_list.insertar(stod(limpiarValorNumerico(datosFilas->obtener(i)->obtener(col_x1))));
                x2_list.insertar(stod(limpiarValorNumerico(datosFilas->obtener(i)->obtener(col_x2))));
            }
        }

        int n = y_list.longitud();
        if (n < 3) { cout << "No hay suficientes datos." << endl; return; }
        
        double** X = crearMatriz(n, 3);
        double** Y = crearMatriz(n, 1);
        for(int i = 0; i < n; ++i) {
            X[i][0] = 1; // Intercepto
            X[i][1] = x1_list.obtener(i);
            X[i][2] = x2_list.obtener(i);
            Y[i][0] = y_list.obtener(i);
        }

        double** Xt = transponerMatriz(X, n, 3);
        double** XtX = multiplicarMatrices(Xt, 3, n, X, n, 3);
        double** XtX_inv = invertirMatriz3x3(XtX);
        if(!XtX_inv) {
            cout << "Error: La matriz no es invertible (posible multicolinealidad)." << endl;
            liberarMatriz(X, n); liberarMatriz(Y, n); liberarMatriz(Xt, 3); liberarMatriz(XtX, 3);
            return;
        }
        double** XtY = multiplicarMatrices(Xt, 3, n, Y, n, 1);
        double** beta = multiplicarMatrices(XtX_inv, 3, 3, XtY, 3, 1);

        cout << "\nEcuacion de Regresion:" << endl;
        cout << nombreColumnas->obtener(col_y) << " = " << beta[0][0] 
             << " + (" << beta[1][0] << " * " << nombreColumnas->obtener(col_x1) << ")"
             << " + (" << beta[2][0] << " * " << nombreColumnas->obtener(col_x2) << ")" << endl;

        liberarMatriz(X, n); liberarMatriz(Y, n); liberarMatriz(Xt, 3);
        liberarMatriz(XtX, 3); liberarMatriz(XtX_inv, 3); liberarMatriz(XtY, 3);
        liberarMatriz(beta, 3);
    }
};

void mostrarMenu() {
    cout << "\n======================== MENU ========================" << endl;
    cout << "--- Tecnicas Univariadas ---" << endl;
    cout << "1. Vista previa del dataset" << endl;
    cout << "2. Frecuencia absoluta y relativa" << endl;
    cout << "3. Porcentaje de valores faltantes" << endl;
    cout << "4. Rango intercuartilico (IQR)" << endl;
    cout << "5. Coeficiente de variacion" << endl;
    cout << "--- Tecnicas Bivariadas ---" << endl;
    cout << "6. Chi-cuadrado (Categorica vs. Categorica)" << endl;
    cout << "7. Correlacion de Pearson (Numerica vs. Numerica)" << endl;
    cout << "8. ANOVA (Categorica vs. Numerica)" << endl;
    cout << "--- Tecnicas Multivariadas ---" << endl;
    cout << "9. Regresion Lineal Multiple" << endl;
    cout << "10. Salir" << endl;
    cout << "======================================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    cout << "--- AED aplicado a Datasets ---" << endl;
    
    AnalizadorDataset analizador;
    string rutaArchivo;
    
    cout << "Ingrese la ruta del archivo CSV: ";
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
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
        }
        
        if (opcion != 10) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 10);
    
    return 0;
}