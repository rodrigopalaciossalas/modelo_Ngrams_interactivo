#ifndef NGRAMMANAGER_HPP
#define NGRAMMANAGER_HPP

#include "../Ngram_Base/NGramBase.hpp"
class NGramManager {
public:
    void ejecutarYGuardar(NGramBase* modelo, const char* texto,
                          const char* rutaEspecifica, const char* rutaGeneral);
    void inicializarModelo(NGramBase* modelo, const char* rutaArchivo);
    void construirModeloDesdeArchivo(NGramBase* modelo, const char* rutaTexto);
    const char* predecir(NGramBase* modelo, const char* ultimaPalabra);
    char** predecirTop(NGramBase* modelo, const char* ultimaPalabra, int cantidadOpciones);
    char** predecirSiguiente(const char* ultimaPalabra, int cantidadOpciones);
    void registrarPrediccion(const char* palabra1, const char* palabra2, const char* rutaDB);
};

#endif
