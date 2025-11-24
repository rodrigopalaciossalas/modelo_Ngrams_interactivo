#include <iostream>
#include "utils/Tokenizador.hpp"
#include "utils/TextProcessor.hpp"
#include "utils/FileManager.hpp"
#include "Ngrams/manager/NGramManager.hpp"
#include "Ngrams/tipos/bigrams/Bigram.hpp"

int main() {
    // Texto de prueba
    const char* texto = "Hola como estas hola como andas";

    // Crear modelo bigram
    Bigram modelo;

    // Crear manager
    NGramManager manager;

    // Ejecutar pipeline y guardar en rutas
    manager.ejecutarYGuardar(
        &modelo,
        texto,
        "data/bigrams.txt",
        "data/general.txt"
    );

    std::cout << "Bigramas generados y almacenados correctamente.\n";
    return 0;
}
