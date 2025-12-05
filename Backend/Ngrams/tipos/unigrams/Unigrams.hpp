#ifndef UNIGRAMS_HPP
#define UNIGRAMS_HPP

#include "../../../utils/Tokenizador.hpp"
#include "../../Ngram_Base/NGramBase.hpp"

class Unigram : public NGramBase {
private:
    char** palabras;    // array de palabras
    int* frecuencias;   // frecuencias correspondientes
    int cantidad;       // cantidad usada

    int buscarPalabra(const char* palabra);
    void agregarPalabra(const char* palabra);

public:
    Unigram();
    ~Unigram();

    void procesarTokens(char** tokens, int cantidadTokens) override;
    char** obtenerNgrams(int& cantidadOut) override;
    void limpiar() override;
    const char* predecir(const char* ultimaPalabra) override;
    void cargarDatos(const char* rutaArchivo) override;
};

#endif
