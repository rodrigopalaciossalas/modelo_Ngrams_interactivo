#include "Tokenizador.hpp"

Tokenizador::Tokenizador() {
    tokens = nullptr;
    cantidad = 0;
}

Tokenizador::~Tokenizador() {
    limpiar();
}

void Tokenizador::tokenizar(const char* texto) {
    if (tokens != nullptr) limpiar();
    if (!texto) return;

    int espacios = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ' ') espacios++;
    }
    cantidad = espacios + 1;

    tokens = new char*[cantidad];
    for (int i = 0; i < cantidad; i++) {
        tokens[i] = new char[50];
        for (int j = 0; j < 50; j++) tokens[i][j] = '\0';
    }

    int indice = 0, pos = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] != ' ' && texto[i] != '\n' && texto[i] != '\t') {
            tokens[indice][pos++] = texto[i];
        } else if (pos > 0) { 
            tokens[indice][pos] = '\0';
            indice++;
            pos = 0;
        }
    }
    if (pos > 0) tokens[indice][pos] = '\0';
    else cantidad = indice;
}

char** Tokenizador::obtenerTokens() {
    return tokens;
}

int Tokenizador::obtenerCantidad() {
    return cantidad;
}

void Tokenizador::limpiar() {
    if (tokens != nullptr) {
        for (int i = 0; i < cantidad; i++) delete[] tokens[i];
        delete[] tokens;
        tokens = nullptr;
        cantidad = 0;
    }
}
