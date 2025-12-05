#include "TextBuilder.hpp"

TextBuilder::TextBuilder() {
    capacidad = 50;
    cantidad = 0;
    palabras = new char*[capacidad];
}

TextBuilder::~TextBuilder() {
    limpiar();
    delete[] palabras;
}

void TextBuilder::expandirCapacidad() {
    int nuevaCap = capacidad * 2;
    char** nuevo = new char*[nuevaCap];
    for (int i = 0; i < cantidad; i++) {
        nuevo[i] = palabras[i];
    }
    delete[] palabras;
    palabras = nuevo;
    capacidad = nuevaCap;
}

void TextBuilder::agregarPalabra(const char* palabra) {
    if (!palabra) return;

    if (cantidad >= capacidad) expandirCapacidad();

    int len = 0;
    while (palabra[len] != '\0') len++;

    char* copia = new char[len + 1];
    for (int i = 0; i < len; i++) copia[i] = palabra[i];
    copia[len] = '\0';

    palabras[cantidad++] = copia;
}

char* TextBuilder::obtenerTextoCompleto() {
    if (cantidad == 0) {
        char* vacio = new char[1];
        vacio[0] = '\0';
        return vacio;
    }

    int totalLen = 0;
    for (int i = 0; i < cantidad; i++) {
        int len = 0;
        while (palabras[i][len] != '\0') len++;
        totalLen += len + 1;
    }

    char* resultado = new char[totalLen];
    int pos = 0;

    for (int i = 0; i < cantidad; i++) {
        int j = 0;
        while (palabras[i][j] != '\0') resultado[pos++] = palabras[i][j++];
        if (i < cantidad - 1) resultado[pos++] = ' ';
    }

    resultado[pos] = '\0';
    return resultado;
}

char** TextBuilder::obtenerPalabras(int& cantidadOut) {
    cantidadOut = cantidad;
    if (cantidad == 0) return nullptr;

    char** copia = new char*[cantidad];
    for (int i = 0; i < cantidad; i++) {
        int len = 0;
        while (palabras[i][len] != '\0') len++;

        copia[i] = new char[len + 1];
        for (int j = 0; j < len; j++) copia[i][j] = palabras[i][j];
        copia[i][len] = '\0';
    }

    return copia;
}

const char* TextBuilder::obtenerUltimaPalabra() {
    if (cantidad == 0) return "";
    return palabras[cantidad - 1];
}

void TextBuilder::limpiar() {
    for (int i = 0; i < cantidad; i++) {
        delete[] palabras[i];
    }
    cantidad = 0;
}

bool TextBuilder::removerUltima() {
    if (cantidad == 0) return false;
    delete[] palabras[cantidad - 1];
    cantidad--;
    return true;
}
