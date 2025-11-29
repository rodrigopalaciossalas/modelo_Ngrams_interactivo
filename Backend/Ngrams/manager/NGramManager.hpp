// NGramManager.hpp
#ifndef NGRAMMANAGER_HPP
#define NGRAMMANAGER_HPP

#include "../Ngram_Base/NGramBase.hpp"
class NGramManager {
public:
    void ejecutarYGuardar(NGramBase* modelo, const char* texto,
                          const char* rutaEspecifica, const char* rutaGeneral);
    void inicializarModelo(NGramBase* modelo, const char* rutaArchivo);
};

#endif