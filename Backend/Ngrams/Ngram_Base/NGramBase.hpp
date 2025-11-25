#ifndef NGRAMBASE_HPP
#define NGRAMBASE_HPP

class NGramBase {
protected:
    int n;

public:
    NGramBase(int n_) : n(n_) {}
    virtual ~NGramBase() {}

    virtual void procesarTokens(char** tokens, int cantidadTokens) = 0;
    virtual char** obtenerNgrams(int& cantidadOut) = 0;
    virtual void limpiar() = 0;
    int obtenerN() const { return n; }
};

#endif
