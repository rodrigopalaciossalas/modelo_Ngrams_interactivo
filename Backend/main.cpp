#include <iostream>
#include "utils/FileManager.hpp"
#include "Ngrams/manager/NGramManager.hpp"
#include "Ngrams/manager/PredictionSession.hpp"
#include "Ngrams/tipos/bigrams/Bigram.hpp"

using namespace std;

int main() {
    const char* texto = "the quick brown fox jumps over the lazy dog";

    Bigram modelo;
    NGramManager manager;
    const char* rutaDB = "data/2grams_english.txt";

    manager.inicializarModelo(&modelo, rutaDB);

    cout << "--- demo del predictor ---" << endl;
    cout << endl;

    PredictionSession sesion;
    if (!sesion.iniciar(&manager, &modelo, "the", 3)) {
        cout << "error: no se pudo cargar el archivo data/2grams_english.txt" << endl;
        return 1;
    }
    
    cout << "palabra inicial: 'the'" << endl;
    cout << "texto actual: " << sesion.obtenerTextoCompleto() << endl;
    cout << endl;

    for (int ronda = 0; ronda < 3; ronda++) {
        int cantOps = 0;
        char** opciones = sesion.obtenerOpciones(cantOps);

        cout << "ronda " << (ronda + 1) << " - opciones:" << endl;
        if (opciones) {
            for (int i = 0; i < cantOps; i++) {
                cout << "  [" << i << "] " << opciones[i] << endl;
            }
            cout << "seleccionando la opcion [0]" << endl;
            if (sesion.seleccionarOpcion(0)) {
                cout << "palabra actual: '" << sesion.obtenerPalabraActual() << "'" << endl;
                cout << "texto completo: " << sesion.obtenerTextoCompleto() << endl;
            } else {
                cout << "ya no hay mas opciones" << endl;
                break;
            }

            for (int i = 0; i < cantOps; i++) delete[] opciones[i];
            delete[] opciones;
        } else {
            cout << "no hay predicciones" << endl;
            break;
        }

        cout << endl;
    }

    sesion.finalizar();
    cout << "fin de la demo" << endl;

    return 0;
}
