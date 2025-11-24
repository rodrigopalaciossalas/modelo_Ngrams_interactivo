#include "Tokenizador.hpp"
#include <iostream>

Tokenizador::Tokenizador() {
    capacidadTokens = 10;
    cantidad = 0;
    tokens = new char*[capacidadTokens];
}

Tokenizador::~Tokenizador() {
    limpiar();
    delete[] tokens;
}

bool Tokenizador::esLetra(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9');
}

char Tokenizador::aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

void Tokenizador::agregarToken(const char* palabra, int longitud) {
    if (cantidad == capacidadTokens) {
        capacidadTokens *= 2;
        char** nuevo = new char*[capacidadTokens];
        for (int i = 0; i < cantidad; i++) nuevo[i] = tokens[i];
        delete[] tokens;
        tokens = nuevo;
    }

    char* nuevo = new char[longitud + 1];
    for (int i = 0; i < longitud; i++)
        nuevo[i] = palabra[i];

    nuevo[longitud] = '\0';
    tokens[cantidad++] = nuevo;
}

void Tokenizador::tokenizar(const char* texto) {
    limpiar();

    if (!texto) return;

    int len = 0;
    while (texto[len] != '\0') len++;

    char palabra[200];
    int pos = 0;

    for (int i = 0; i <= len; i++) {
        char c = texto[i];

        if (esLetra(c)) {
            palabra[pos++] = aMinuscula(c);
        } else {
            if (pos > 0) {
                agregarToken(palabra, pos);
                pos = 0;
            }
        }
    }
}

char** Tokenizador::obtenerTokens() {
    return tokens;
}

int Tokenizador::obtenerCantidad() {
    return cantidad;
}

void Tokenizador::limpiar() {
    for (int i = 0; i < cantidad; i++)
        delete[] tokens[i];
    cantidad = 0;
}
