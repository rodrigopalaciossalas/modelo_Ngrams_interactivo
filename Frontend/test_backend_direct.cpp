#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../Backend/Ngrams/manager/NGramManager.hpp"
#include "../Backend/Ngrams/manager/PredictionSession.hpp"
#include "../Backend/Ngrams/tipos/bigrams/Bigram.hpp"

int main(int argc, char* argv[]) {
    // Hardcode full path to data file
    const char* dbPath = "D:\\programacion\\cc1\\modelo_Ngrams_interactivo\\Backend\\data\\2grams_english.txt";
    
    std::cerr << "[TEST] Testing backend directly with: " << dbPath << std::endl;
    
    Bigram modelo;
    NGramManager manager;
    
    // Initialize model
    manager.inicializarModelo(&modelo, dbPath);
    std::cerr << "[TEST] Model initialized" << std::endl;
    
    // Create session
    PredictionSession sesion;
    if (!sesion.iniciar(&manager, &modelo, "the", 5, dbPath)) {
        std::cerr << "[TEST] FAIL: Could not start session" << std::endl;
        return 1;
    }
    std::cerr << "[TEST] Session started with 'the'" << std::endl;
    
    // Get options
    int cantOps = 0;
    char** opciones = sesion.obtenerOpciones(cantOps);
    std::cerr << "[TEST] obtenerOpciones returned " << cantOps << " options" << std::endl;
    
    if (opciones && cantOps > 0) {
        for (int i = 0; i < cantOps; i++) {
            std::cerr << "[TEST] Option " << i << ": " << opciones[i] << std::endl;
        }
        for (int i = 0; i < cantOps; i++) delete[] opciones[i];
        delete[] opciones;
        std::cerr << "[TEST] SUCCESS: Got predictions!" << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST] FAIL: No options returned" << std::endl;
        return 1;
    }
}
