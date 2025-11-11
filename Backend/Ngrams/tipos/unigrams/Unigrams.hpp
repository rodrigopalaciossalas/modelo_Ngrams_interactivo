#ifndef UNIGRAMS_HPP
#define UNIGRAMS_HPP

#include <iostream>
#include <string>
#include "../../../Utils/Tokenizador.hpp"
#include "../../Ngram_Base/NGramBase.hpp"

class Unigram : public NGramBase {
private:
    std::string* palabras;  
    Tokenizador tokenizador;  
    Unigram();  
    ~Unigram();

    void generar(const char* texto) override;  
    void contar() override;                    
    void guardar(const char* ruta) override;   

    void mostrar(); 
};

#endif