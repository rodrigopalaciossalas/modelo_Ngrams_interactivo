#ifndef TOKENIZADOR_HPP
#define TOKENIZADOR_HPP

class Tokenizador {
private:
    char** tokens;
    int cantidad;
    int capacidadTokens;

    bool esLetra(char c);
    char aMinuscula(char c);

    void agregarToken(const char* palabra, int longitud);

public:
    Tokenizador();
    ~Tokenizador();

    void tokenizar(const char* texto);
    char** obtenerTokens();
    int obtenerCantidad();
    void limpiar();
};

#endif
