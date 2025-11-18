#include "tokenizador.hpp"

Tokenizador::Tokenizador() {
    tokens = nullptr;
    cantidad = 0;
}

Tokenizador::~Tokenizador() {
    limpiar();
}

bool Tokenizador::esLetra(char c) {
    // A–Z o a–z
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

char Tokenizador::aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z') 
        return c + 32;
    return c;
}

void Tokenizador::tokenizar(const char* texto) {
    limpiar();

    tokens = new char*[1000];
    cantidad = 0;

    char temp[100];
    int pos = 0;

    for (int i = 0; texto[i] != 0; i++) {
        char c = texto[i];

        if (esLetra(c)) {
            temp[pos++] = aMinuscula(c);
        } else {
            if (pos > 0) {
                temp[pos] = '\0';

                tokens[cantidad] = new char[pos + 1];
                for (int j = 0; j <= pos; j++)
                    tokens[cantidad][j] = temp[j];

                cantidad++;
                pos = 0;
            }
        }
    }

    if (pos > 0) {
        temp[pos] = '\0';

        tokens[cantidad] = new char[pos + 1];
        for (int j = 0; j <= pos; j++)
            tokens[cantidad][j] = temp[j];

        cantidad++;
    }
}

char** Tokenizador::obtenerTokens() {
    return tokens;
}

int Tokenizador::obtenerCantidad() {
    return cantidad;
}

void Tokenizador::limpiar() {
    if (tokens != nullptr) {
        for (int i = 0; i < cantidad; i++) {
            delete[] tokens[i];
        }
        delete[] tokens;
    }
    tokens = nullptr;
    cantidad = 0;
}
