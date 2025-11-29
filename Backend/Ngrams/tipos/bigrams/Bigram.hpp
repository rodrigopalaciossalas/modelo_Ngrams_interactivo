#ifndef BIGRAM_HPP
#define BIGRAM_HPP

#include "../../Ngram_Base/NGramBase.hpp"
#include "../../../utils/Tokenizador.hpp"
#include "../../../utils/FileManager.hpp"

class Bigram : public NGramBase {
private:
    char** bigrams;    
    int* frecuencias;  
    int cantidad;      

    int comparar(const char* a, const char* b);
    void agregarBigrama(const char* b1, const char* b2);

public:
    Bigram();
    ~Bigram();

    void procesarTokens(char** tokens, int cantidadTokens) override;
    char** obtenerNgrams(int& cantidadOut) override;
    void limpiar() override;
    const char* predecir(const char* ultimaPalabra) override;
    void cargarDatos(const char* rutaArchivo) override;
};

#endif
