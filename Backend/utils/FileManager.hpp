#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>

class FileManager {
public:
    static bool guardarLista(const char* ruta, const char** lista, int cantidad);
    static bool guardarListaAppend(const char* ruta, const char** lista, int cantidad);
    static char** cargarLista(const char* ruta, int& cantidad);
};

#endif
