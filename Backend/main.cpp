#include <iostream>
#include "utils/FileManager.hpp"
#include "Ngrams/manager/NGramManager.hpp"
#include "Ngrams/tipos/bigrams/Bigram.hpp"

int main() {
    const char* texto = "Hola como estas hola como te fue";

    Bigram modelo;

    NGramManager manager;

    manager.ejecutarYGuardar(
        &modelo,
        texto,
        "data/bigrams.txt",
        "data/general.txt"
    );

    std::cout << "si";
    return 0;
}
