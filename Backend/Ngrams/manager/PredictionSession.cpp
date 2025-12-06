#include "PredictionSession.hpp"

PredictionSession::PredictionSession() {
    manager = nullptr;
    modelo = nullptr;
    palabraActual = "";
    cantidadOpciones = 3;
    rutaBD = nullptr;
    ultimasFrecuencias = nullptr;
    ultimasFrecuenciasCount = 0;
}

PredictionSession::~PredictionSession() {
    finalizar();
}

bool PredictionSession::iniciar(NGramManager* mgr, NGramBase* mdl, const char* palabraInicial, int numOpciones, const char* ruta) {
    if (!mgr || !mdl || !palabraInicial) return false;

    manager = mgr;
    modelo = mdl;
    cantidadOpciones = numOpciones;
    rutaBD = ruta;

    textoActual.limpiar();
    textoActual.agregarPalabra(palabraInicial);
    palabraActual = textoActual.obtenerUltimaPalabra();

    return true;
}

char** PredictionSession::obtenerOpciones(int& cantidadOut) {
    if (!manager || !modelo) {
        cantidadOut = 0;
        return nullptr;
    }

    int totalNgrams = 0;
    char** todosNgrams = modelo->obtenerNgrams(totalNgrams);
    if (!todosNgrams || totalNgrams == 0) {
        cantidadOut = 0;
        return nullptr;
    }

    char** candidatos = new char*[totalNgrams];
    int* frecuencias = new int[totalNgrams];
    int cantidadCandidatos = 0;

    int lenPalabra = 0;
    while (palabraActual[lenPalabra] != '\0') lenPalabra++;

    for (int i = 0; i < totalNgrams; i++) {
        char* ngram = todosNgrams[i];

        int posGuion = -1;
        for (int j = 0; ngram[j] != '\0'; j++) {
            if (ngram[j] == '-' && j > 0 && ngram[j-1] == ' ') {
                posGuion = j - 1;
                break;
            }
        }

        if (posGuion <= 0) continue;

        char* bigram = new char[posGuion + 1];
        for (int k = 0; k < posGuion; k++) bigram[k] = ngram[k];
        bigram[posGuion] = '\0';

        bool coincide = true;
        for (int j = 0; j < lenPalabra; j++) {
            if (bigram[j] != palabraActual[j]) { coincide = false; break; }
        }

        if (coincide && bigram[lenPalabra] == ' ') {
            int inicioSegunda = lenPalabra + 1;
            int lenSegunda = 0;
            while (bigram[inicioSegunda + lenSegunda] != '\0') lenSegunda++;

            int freq = 0;
            for (int j = posGuion + 1; ngram[j] != '\0'; j++) {
                if (ngram[j] >= '0' && ngram[j] <= '9') {
                    freq = freq * 10 + (ngram[j] - '0');
                }
            }

            char* segundaPalabra = new char[lenSegunda + 1];
            for (int k = 0; k < lenSegunda; k++) {
                segundaPalabra[k] = bigram[inicioSegunda + k];
            }
            segundaPalabra[lenSegunda] = '\0';

            candidatos[cantidadCandidatos] = segundaPalabra;
            frecuencias[cantidadCandidatos] = freq;
            cantidadCandidatos++;
        }

        delete[] bigram;
    }

    for (int i = 0; i < cantidadCandidatos - 1; i++) {
        for (int j = i + 1; j < cantidadCandidatos; j++) {
            if (frecuencias[j] > frecuencias[i]) {
                int tmpFreq = frecuencias[i];
                frecuencias[i] = frecuencias[j];
                frecuencias[j] = tmpFreq;

                char* tmpCand = candidatos[i];
                candidatos[i] = candidatos[j];
                candidatos[j] = tmpCand;
            }
        }
    }

    int resultado_count = (cantidadCandidatos < cantidadOpciones) ? cantidadCandidatos : cantidadOpciones;

    if (ultimasFrecuencias) delete[] ultimasFrecuencias;
    ultimasFrecuenciasCount = resultado_count;
    ultimasFrecuencias = new int[ultimasFrecuenciasCount];
    
    char** resultado = new char*[resultado_count];
    for (int i = 0; i < resultado_count; i++) {
        resultado[i] = candidatos[i];
        ultimasFrecuencias[i] = frecuencias[i];
    }

    for (int i = resultado_count; i < cantidadCandidatos; i++) {
        delete[] candidatos[i];
    }
    delete[] candidatos;
    delete[] frecuencias;

    cantidadOut = resultado_count;
    return resultado;
}

bool PredictionSession::seleccionarOpcion(int indiceOpcion) {
    if (!manager || !modelo) return false;

    int cantOps = 0;
    char** opciones = obtenerOpciones(cantOps);

    if (!opciones || indiceOpcion < 0 || indiceOpcion >= cantOps) {
        if (opciones) {
            for (int i = 0; i < cantOps; i++) delete[] opciones[i];
            delete[] opciones;
        }
        return false;
    }

    const char* palabraEscogida = opciones[indiceOpcion];
    
    if (rutaBD != nullptr) {
        manager->registrarPrediccion(palabraActual, palabraEscogida, rutaBD);
    }
    
    textoActual.agregarPalabra(palabraEscogida);
    palabraActual = textoActual.obtenerUltimaPalabra();

    for (int i = 0; i < cantOps; i++) delete[] opciones[i];
    delete[] opciones;

    return true;
}

char* PredictionSession::obtenerTextoCompleto() {
    return textoActual.obtenerTextoCompleto();
}

const char* PredictionSession::obtenerPalabraActual() {
    return palabraActual;
}

bool PredictionSession::retroceder() {
    bool resultado = textoActual.removerUltima();
    if (resultado) {
        palabraActual = textoActual.obtenerUltimaPalabra();
    }
    return resultado;
}

void PredictionSession::finalizar() {
    textoActual.limpiar();
    palabraActual = "";
    manager = nullptr;
    modelo = nullptr;
    if (ultimasFrecuencias) {
        delete[] ultimasFrecuencias;
        ultimasFrecuencias = nullptr;
    }
    ultimasFrecuenciasCount = 0;
}

int PredictionSession::obtenerCantidadPalabras() const {
    return textoActual.obtenerCantidad();
}

void PredictionSession::setCantidadOpciones(int n) {
    if (n > 0) cantidadOpciones = n;
}

bool PredictionSession::seleccionarPalabra(const char* palabra) {
    if (!manager || !palabra) return false;
    
    int cantOps = 0;
    if (rutaBD != nullptr) {
        manager->registrarPrediccion(palabraActual, palabra, rutaBD);
    }
    
    textoActual.agregarPalabra(palabra);
    palabraActual = textoActual.obtenerUltimaPalabra(); // Actualizar puntero
    return true;
}

int* PredictionSession::obtenerUltimasFrecuencias(int& cantOut) {
    cantOut = ultimasFrecuenciasCount;
    return ultimasFrecuencias;
}
