#include "Bigram.hpp"

Bigram::Bigram() : NGramBase(2) {
    bigrams = nullptr;
    frecuencias = nullptr;
    cantidad = 0;
}

Bigram::~Bigram() {
    limpiar();
}

void Bigram::limpiar() {
    if (bigrams != nullptr) {
        for (int i = 0; i < cantidad; i++) {
            delete[] bigrams[i];
        }
        delete[] bigrams;
    }
    if (frecuencias != nullptr)
        delete[] frecuencias;

    bigrams = nullptr;
    frecuencias = nullptr;
    cantidad = 0;
}

int Bigram::comparar(const char* a, const char* b) {
    int i = 0;
    while (a[i] != 0 || b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return 1;
}

void Bigram::agregarBigrama(const char* w1, const char* w2) {
    int t1 = 0; while (w1[t1] != 0) t1++;
    int t2 = 0; while (w2[t2] != 0) t2++;

    int total = t1 + t2 + 2; 
    char* nuevo = new char[total];

    int k = 0;
    for (int i = 0; i < t1; i++) nuevo[k++] = w1[i];
    nuevo[k++] = ' ';
    for (int i = 0; i < t2; i++) nuevo[k++] = w2[i];
    nuevo[k] = '\0';

    bigrams[cantidad] = nuevo;
    frecuencias[cantidad] = 1;
    cantidad++;
}

void Bigram::procesarTokens(char** tokens, int cantidadTokens) {
    limpiar();

    if (cantidadTokens < 2) return;

    bigrams = new char*[cantidadTokens - 1];
    frecuencias = new int[cantidadTokens - 1];
    cantidad = 0;

    for (int i = 0; i < cantidadTokens - 1; i++) {
        agregarBigrama(tokens[i], tokens[i + 1]);
    }

    for (int i = 0; i < cantidad; i++) {
        for (int j = i + 1; j < cantidad; j++) {
            if (frecuencias[j] == 0) continue;

            if (comparar(bigrams[i], bigrams[j])) {
                frecuencias[i]++;
                frecuencias[j] = 0;
            }
        }
    }
}

char** Bigram::obtenerNgrams(int& cantidadOut) {
    int reales = 0;
    for (int i = 0; i < cantidad; i++)
        if (frecuencias[i] > 0)
            reales++;

    cantidadOut = reales;

    if (reales == 0) return nullptr;

    char** salida = new char*[reales];
    int idx = 0;

    for (int i = 0; i < cantidad; i++) {
        if (frecuencias[i] == 0) continue;

        const char* bg = bigrams[i];

        int t = 0; while (bg[t] != 0) t++;

        char num[12];
        int pos = 0;
        int f = frecuencias[i];

        if (f == 0) f = 1;

        int temp = f;
        while (temp > 0) {
            num[pos++] = (temp % 10) + '0';
            temp /= 10;
        }

        num[pos] = 0;

        char numFinal[12];
        int k2 = 0;
        for (int p = pos - 1; p >= 0; p--)
            numFinal[k2++] = num[p];
        numFinal[k2] = '\0';

        int total = t + k2 + 4;
        char* linea = new char[total];
        int k = 0;

        for (int j = 0; j < t; j++) linea[k++] = bg[j];
        linea[k++] = ' ';
        linea[k++] = '-';
        linea[k++] = ' ';

        int p = 0;
        while (numFinal[p] != 0)
            linea[k++] = numFinal[p++];
        linea[k] = '\0';

        salida[idx++] = linea;
    }

    return salida;
}
