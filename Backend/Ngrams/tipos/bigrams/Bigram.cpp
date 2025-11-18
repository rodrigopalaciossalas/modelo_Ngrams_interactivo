#include "Bigram.hpp"
#include <iostream>

Bigram::Bigram() : NGramBase(2) {
    bigrams = nullptr;
    frecuencias = nullptr;
    cantidad = 0;
}

Bigram::~Bigram() {
    if (bigrams) {
        for (int i = 0; i < cantidad; i++) {
            delete[] bigrams[i];
        }
        delete[] bigrams;
    }
    if (frecuencias) delete[] frecuencias;
}

void Bigram::generar(const char* texto) {
    Tokenizador tok;
    tok.tokenizar(texto);

    int n = tok.obtenerCantidad();
    char** tokens = tok.obtenerTokens();

    if (n < 2) return;

    // Máximo número de bigramas posibles = n - 1
    bigrams = new char*[n - 1];
    frecuencias = new int[n - 1];
    cantidad = 0;

    // Generar cada bigrama
    for (int i = 0; i < n - 1; i++) {
        const char* p1 = tokens[i];
        const char* p2 = tokens[i + 1];

        // Obtener tamaños manualmente (sin strlen)
        int tam1 = 0; while (p1[tam1] != '\0') tam1++;
        int tam2 = 0; while (p2[tam2] != '\0') tam2++;

        int total = tam1 + tam2 + 2; // espacio + '\0'

        bigrams[cantidad] = new char[total];

        // Copiar palabra 1
        int k = 0;
        for (int j = 0; j < tam1; j++) {
            bigrams[cantidad][k++] = p1[j];
        }

        // espacio
        bigrams[cantidad][k++] = ' ';

        // copiar palabra 2
        for (int j = 0; j < tam2; j++) {
            bigrams[cantidad][k++] = p2[j];
        }

        // terminar cadena
        bigrams[cantidad][k] = '\0';

        frecuencias[cantidad] = 1;
        cantidad++;
    }
}

void Bigram::contar() {
    if (cantidad == 0) return;

    for (int i = 0; i < cantidad; i++) {
        for (int j = i + 1; j < cantidad; j++) {

            // Comparar cadenas sin strcmp
            int k = 0;
            bool iguales = true;

            while (bigrams[i][k] != '\0' || bigrams[j][k] != '\0') {
                if (bigrams[i][k] != bigrams[j][k]) {
                    iguales = false;
                    break;
                }
                k++;
            }

            if (iguales) {
                frecuencias[i]++;       
                frecuencias[j] = 0;     // marcar duplicado
            }
        }
    }
}

void Bigram::guardar(const char* ruta) {
    // Recuento final de bigramas únicos
    int reales = 0;
    for (int i = 0; i < cantidad; i++) {
        if (frecuencias[i] > 0)
            reales++;
    }

    // Crear un array temporal para guardar
    char** salida = new char*[reales];
    int idx = 0;

    for (int i = 0; i < cantidad; i++) {
        if (frecuencias[i] == 0) continue;

        // Crear cadena final "bigrama - frecuencia"
        int tam = 0;
        while (bigrams[i][tam] != '\0') tam++;

        char* linea = new char[tam + 10];
        
        int k = 0;
        for (int j = 0; j < tam; j++)
            linea[k++] = bigrams[i][j];

        linea[k++] = ' ';
        linea[k++] = '-';
        linea[k++] = ' ';

        // convertir frecuencia a char*
        int f = frecuencias[i];
        char num[10];
        int pos = 0;

        while (f > 0) {
            num[pos++] = (f % 10) + '0';
            f /= 10;
        }

        // invertir número
        for (int j = pos - 1; j >= 0; j--)
            linea[k++] = num[j];

        linea[k] = '\0';

        salida[idx++] = linea;
    }

    FileManager::guardarLista(ruta, (const char**)salida, reales);

    for (int i = 0; i < reales; i++)
        delete[] salida[i];

    delete[] salida;
}

char** Bigram::obtenerBigrams() { return bigrams; }
int* Bigram::obtenerFrecuencias() { return frecuencias; }
int Bigram::obtenerCantidadBigrams() { return cantidad; }
