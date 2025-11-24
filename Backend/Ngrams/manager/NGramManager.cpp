#include "NGramManager.hpp"
#include "../../utils/Tokenizador.hpp"
#include "../../utils/FileManager.hpp"

void NGramManager::ejecutarYGuardar(NGramBase* modelo, const char* texto,
                                    const char* rutaEspecifica, const char* rutaGeneral) {
    if (modelo == nullptr || texto == nullptr) return;

    Tokenizador tz;
    tz.tokenizar(texto);
    char** tokens = tz.obtenerTokens();
    int nTokens = tz.obtenerCantidad();

    modelo->procesarTokens(tokens, nTokens);

    int cantidadOut = 0;
    char** lines = modelo->obtenerNgrams(cantidadOut);
    if (lines != nullptr && cantidadOut > 0) {
        FileManager::guardarLista(rutaEspecifica, (const char**)lines, cantidadOut);
        FileManager::guardarLista(rutaGeneral, (const char**)lines, cantidadOut);
    }

    modelo->limpiar();
    tz.limpiar();
}
