#include "Tokenizador.hpp"

Tokenizador::Tokenizador() {
    tokens = nullptr;
    cantidad = 0;
    capacidadTokens = 0;
}

Tokenizador::~Tokenizador() {
    limpiar();
}

bool Tokenizador::esLetra(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

char Tokenizador::aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

void Tokenizador::agregarToken(const char* palabra, int longitud) {
    if (longitud == 0) return;

    if (cantidad == capacidadTokens) {
        int nuevaCap = (capacidadTokens == 0 ? 4 : capacidadTokens * 2);
        char** nuevo = new char*[nuevaCap];

        for (int i = 0; i < cantidad; i++)
            nuevo[i] = tokens[i];

        delete[] tokens;
        tokens = nuevo;
        capacidadTokens = nuevaCap;
    }

    char* nuevoToken = new char[longitud + 1];
    for (int i = 0; i < longitud; i++)
        nuevoToken[i] = palabra[i];
    nuevoToken[longitud] = '\0';

    tokens[cantidad] = nuevoToken;
    cantidad++;
}

void Tokenizador::tokenizar(const char* texto) {
    limpiar();

    int tamTemp = 32;
    int lenTemp = 0;
    char* temp = new char[tamTemp];

    for (int i = 0; texto[i] != 0; i++) {
        char c = texto[i];

        if (esLetra(c)) {
            if (lenTemp + 1 >= tamTemp) {
                int nuevoTam = tamTemp * 2;
                char* nuevo = new char[nuevoTam];
                for (int j = 0; j < lenTemp; j++)
                    nuevo[j] = temp[j];
                delete[] temp;
                temp = nuevo;
                tamTemp = nuevoTam;
            }
            temp[lenTemp++] = aMinuscula(c);
        } else {
            if (lenTemp > 0) {
                agregarToken(temp, lenTemp);
                lenTemp = 0;
            }
        }
    }

    if (lenTemp > 0)
        agregarToken(temp, lenTemp);

    delete[] temp;
}

char** Tokenizador::obtenerTokens() {
    return tokens;
}

int Tokenizador::obtenerCantidad() {
    return cantidad;
}

void Tokenizador::limpiar() {
    if (tokens != nullptr) {
        for (int i = 0; i < cantidad; i++)
            delete[] tokens[i];
        delete[] tokens;
    }

    tokens = nullptr;
    cantidad = 0;
    capacidadTokens = 0;
}
