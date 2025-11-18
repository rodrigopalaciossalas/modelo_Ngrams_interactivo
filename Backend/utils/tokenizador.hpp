#ifndef TOKENIZADOR_HPP
#define TOKENIZADOR_HPP

#include <string>

class Tokenizador {
private:
    char** tokens;
    int cantidad;

    bool esLetra(char c);
    char aMinuscula(char c);

public:
    Tokenizador();
    ~Tokenizador();

    void tokenizar(const char* texto);

    char** obtenerTokens();
    int obtenerCantidad();

    void limpiar();
};

#endif
