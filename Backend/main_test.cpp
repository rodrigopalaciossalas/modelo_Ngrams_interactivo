#include <iostream>
#include "Ngrams/manager/NGramManager.hpp"
#include "Ngrams/manager/PredictionSession.hpp"
#include "Ngrams/tipos/bigrams/Bigram.hpp"

int main() {
    Bigram modelo;
    NGramManager manager;

    const char* rutaDB = "data/2grams_english.txt";

    manager.inicializarModelo(&modelo, rutaDB);

    std::cout << "=== SESION DE PREDICCION CON BD GRANDE ===" << std::endl;
    std::cout << std::endl;

    PredictionSession sesion;
    if (!sesion.iniciar(&manager, &modelo, "the", 5, rutaDB)) {
        std::cout << "Error al iniciar sesion" << std::endl;
        return 1;
    }

    std::cout << "Palabra inicial: 'the'" << std::endl;
    std::cout << "Texto actual: " << sesion.obtenerTextoCompleto() << std::endl;
    std::cout << std::endl;

    for (int ronda = 0; ronda < 5; ronda++) {
        int cantOps = 0;
        char** opciones = sesion.obtenerOpciones(cantOps);

        std::cout << "--- Ronda " << (ronda + 1) << " ---" << std::endl;
        std::cout << "Palabra actual: '" << sesion.obtenerPalabraActual() << "'" << std::endl;
        std::cout << "Opciones disponibles (" << cantOps << "):" << std::endl;

        if (opciones && cantOps > 0) {
            for (int i = 0; i < cantOps; i++) {
                std::cout << "  [" << i << "] " << opciones[i] << std::endl;
            }

            std::cout << "Seleccionando opcion [0]" << std::endl;
            if (sesion.seleccionarOpcion(0)) {
                std::cout << "Texto: " << sesion.obtenerTextoCompleto() << std::endl;
            } else {
                std::cout << "No se pudo seleccionar opcion" << std::endl;
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
