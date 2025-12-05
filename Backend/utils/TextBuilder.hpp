#ifndef TEXTBUILDER_HPP
#define TEXTBUILDER_HPP

class TextBuilder {
private:
    char** palabras;
    int cantidad;
    int capacidad;

    void expandirCapacidad();

public:
    TextBuilder();
    ~TextBuilder();

    void agregarPalabra(const char* palabra);
    char* obtenerTextoCompleto();
    char** obtenerPalabras(int& cantidadOut);
    const char* obtenerUltimaPalabra();
    int obtenerCantidad() const { return cantidad; }
    void limpiar();
    bool removerUltima();
};

#endif
