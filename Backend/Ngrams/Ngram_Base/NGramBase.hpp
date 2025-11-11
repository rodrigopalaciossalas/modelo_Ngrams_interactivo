#include <iostream>
#include <string>

#ifndef NGramBase_HPP
#define NGramBase_HPP

class NGramBase {
    protected:
        int n; 
        int cant;
    public:
        NGramBase(int valor);
        virtual ~NGramBase();

        virtual void generar(const char* texto) = 0;   
        virtual void contar() = 0;                     
        virtual void guardar(const char* ruta) = 0;  


        int obN() const;         
        int obCant() const;
};
#endif
