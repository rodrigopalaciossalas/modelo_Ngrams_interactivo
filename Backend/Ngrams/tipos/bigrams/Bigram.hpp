#ifndef BIGRAM_HPP
#define BIGRAM_HPP

#include "../../Ngram_Base/NGramBase.hpp"
#include "../../../utils/tokenizador.hpp"
#include "../../../utils/FileManager.hpp"

class Bigram : public NGramBase {
private:
    char** bigrams;      // Lista de bigramas generados
    int* frecuencias;    // Frecuencia de cada bigrama
    int cantidad;        // Cantidad de bigramas únicos

public:
    Bigram();
    ~Bigram();

    void generar(const char* texto) override;
    void contar() override;
    void guardar(const char* ruta) override;

    char** obtenerBigrams();
    int* obtenerFrecuencias();
    int obtenerCantidadBigrams();
};

#endif
