#ifndef TOKENIZADOR_HPP
#define TOKENIZADOR_HPP

#include <iostream>
#include <cstring>

class Tokenizador {
private:
    char** tokens;   
    int cantidad;   

public:
    Tokenizador();
    ~Tokenizador();

    void tokenizar(const char* texto);

    char** obtenerTokens();

    int obtenerCantidad();

    void limpiar();
};

#endif
