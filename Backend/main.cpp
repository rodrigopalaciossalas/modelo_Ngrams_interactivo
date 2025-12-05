#include <iostream>
#include "utils/FileManager.hpp"
#include "Ngrams/manager/NGramManager.hpp"
#include "Ngrams/manager/PredictionSession.hpp"
#include "Ngrams/tipos/bigrams/Bigram.hpp"
#include "Ngrams/tipos/unigrams/Unigrams.hpp"

int main() {
    const char* texto = "Hola como estas hola como te fue";

    Bigram modelo;
    NGramManager manager;


    const char* rutaDB = "data/2grams_english.txt";
    manager.ejecutarYGuardar(&modelo, texto, nullptr, rutaDB);


    manager.inicializarModelo(&modelo, rutaDB);

    std::cout << "=== DEMOSTRACION: SESION DE PREDICCION ===" << std::endl;
    std::cout << std::endl;


    PredictionSession sesion;
    if (!sesion.iniciar(&manager, &modelo, "hola", 3)) {
        std::cout << "Error al iniciar sesion" << std::endl;
        return 1;
    }

    std::cout << "Palabra inicial: 'hola'" << std::endl;
    std::cout << "Texto actual: " << sesion.obtenerTextoCompleto() << std::endl;
    std::cout << std::endl;


    for (int ronda = 0; ronda < 3; ronda++) {
        int cantOps = 0;
        char** opciones = sesion.obtenerOpciones(cantOps);

        std::cout << "Ronda " << (ronda + 1) << " - Opciones disponibles:" << std::endl;
        if (opciones) {
            for (int i = 0; i < cantOps; i++) {
                std::cout << "  [" << i << "] " << opciones[i] << std::endl;
            }


            std::cout << "Seleccionando opcion [0]" << std::endl;
            if (sesion.seleccionarOpcion(0)) {
                std::cout << "Palabra actual: '" << sesion.obtenerPalabraActual() << "'" << std::endl;
                std::cout << "Texto completo: " << sesion.obtenerTextoCompleto() << std::endl;
            } else {
                std::cout << "No hay mas opciones para continuar" << std::endl;
                break;
            }


            for (int i = 0; i < cantOps; i++) delete[] opciones[i];
            delete[] opciones;
        } else {
            std::cout << "Sin opciones para continuar" << std::endl;
            break;
        }

        std::cout << std::endl;
    }

    sesion.finalizar();
    std::cout << "Sesion finalizada" << std::endl;

    return 0;
}
