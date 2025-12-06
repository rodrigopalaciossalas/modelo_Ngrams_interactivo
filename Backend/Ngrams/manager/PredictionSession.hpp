#ifndef PREDICTIONSESSION_HPP
#define PREDICTIONSESSION_HPP

#include "../Ngram_Base/NGramBase.hpp"
#include "NGramManager.hpp"
#include "../../utils/TextBuilder.hpp"

class PredictionSession {
private:
    NGramManager* manager;
    NGramBase* modelo;
    TextBuilder textoActual;
    const char* palabraActual;
    int cantidadOpciones;
    const char* rutaBD;
    int* ultimasFrecuencias;
    int ultimasFrecuenciasCount;

public:
    PredictionSession();
    ~PredictionSession();

    bool iniciar(NGramManager* mgr, NGramBase* mdl, const char* palabraInicial, int numOpciones = 3, const char* ruta = nullptr);
    char** obtenerOpciones(int& cantidadOut);
    bool seleccionarOpcion(int indiceOpcion);
    char* obtenerTextoCompleto();
    const char* obtenerPalabraActual();
    bool retroceder();
    void finalizar();
    int obtenerCantidadPalabras() const;
    
    // Nuevas funcionalidades
    void setCantidadOpciones(int n);
    bool seleccionarPalabra(const char* palabra);
    int* obtenerUltimasFrecuencias(int& cantOut);
};

#endif
