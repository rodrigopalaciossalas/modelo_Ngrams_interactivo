#include "Bigram.hpp"


static int cadenaAEntero(const char* s) {
    if (!s) return 0;
    int i = 0;
    while (s[i] == ' ') i++;
    int val = 0;
    while (s[i] >= '0' && s[i] <= '9') {
        val = val * 10 + (s[i] - '0');
        i++;
    }
    return val;
}

static void copiarCadena(char* dest, const char* src, int len) {
    for (int i = 0; i < len; i++) dest[i] = src[i];
    dest[len] = '\0';
}

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

const char* Bigram::predecir(const char* ultimaPalabra) {
    if (bigrams == nullptr || cantidad == 0 || ultimaPalabra == nullptr) {
        return "";
    }
    int len = 0;
    while (ultimaPalabra[len] != '\0') {
        char c = ultimaPalabra[len];
        if (c >= '0' && c <= '9') return "";
        len++;
    }

    int mejorFrecuencia = -1;
    const char* mejorCandidato = ""; 

    for (int i = 0; i < cantidad; i++) {
        if (frecuencias[i] == 0) continue;

        const char* bg = bigrams[i]; 

        int j = 0;
        bool coincide = true;
        
        while (j < len) {
            char cBase = bg[j];              
            char cInput = ultimaPalabra[j];  
            if (cInput >= 'A' && cInput <= 'Z') {
                cInput = cInput + 32; 
            }

            if (cBase != cInput) {
                coincide = false;
                break;
            }
            j++;
        }
        if (coincide) {
            if (bg[j] == ' ') {
                if (frecuencias[i] > mejorFrecuencia) {
                    mejorFrecuencia = frecuencias[i];
                    mejorCandidato = bg + j + 1; 
                }
            }
        }
    }
    
    return mejorCandidato;
}

void Bigram::cargarDatos(const char* rutaArchivo) {
    int lineasLeidas = 0;
    
    char** listaCruda = FileManager::cargarLista(rutaArchivo, lineasLeidas);
    
    if (listaCruda == nullptr || lineasLeidas == 0) return;

    int totalCapacidad = cantidad + lineasLeidas;
    char** nuevosBigrams = new char*[totalCapacidad];
    int* nuevasFrecuencias = new int[totalCapacidad];

    for (int i = 0; i < cantidad; i++) {
        nuevosBigrams[i] = bigrams[i];
        nuevasFrecuencias[i] = frecuencias[i];
    }

    if (bigrams) delete[] bigrams;
    if (frecuencias) delete[] frecuencias;

    bigrams = nuevosBigrams;
    frecuencias = nuevasFrecuencias;

    for (int i = 0; i < lineasLeidas; i++) {
        char* linea = listaCruda[i];

        int len = 0;
        int posGuion = -1;
        while (linea[len] != '\0') {
            if (linea[len] == '-' && posGuion == -1) {
                posGuion = len;
            }
            len++;
        }

        if (posGuion != -1) {
            int freq = cadenaAEntero(linea + posGuion + 2);

            int lenTexto = posGuion - 1;
            
            if (lenTexto > 0) {
                char* nuevoTexto = new char[lenTexto + 1];
                copiarCadena(nuevoTexto, linea, lenTexto);


                int encontrado = -1;
                for (int k = 0; k < cantidad; k++) {
                    if (comparar(nuevosBigrams[k], nuevoTexto)) { encontrado = k; break; }
                }

                if (encontrado != -1) {
                    nuevasFrecuencias[encontrado] += freq;
                    delete[] nuevoTexto; // ya no se necesita
                } else {
                    nuevosBigrams[cantidad] = nuevoTexto;
                    nuevasFrecuencias[cantidad] = freq;
                    cantidad++;
                }
            }
        }
        delete[] linea;
    }
    delete[] listaCruda;
}
