#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>

class FileManager {
public:
    static bool existeArchivo(const char* ruta);
    static bool guardarTexto(const char* ruta, const char* contenido);
    static bool guardarLista(const char* ruta, const char** lista, int cantidad);
    static char** cargarLista(const char* ruta, int& cantidad);
    static void eliminarArchivo(const char* ruta);
};

#endif
