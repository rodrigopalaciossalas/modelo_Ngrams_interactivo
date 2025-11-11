#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <iostream>
#include <fstream>

class FileManager {
private:
    static const char* DATA_PATH;
public:
    static bool existeArchivo(const char* nombreArchivo);
    static void guardarLista(const char* nombreArchivo, const char** datos, int cantidad);
    static char** cargarLista(const char* nombreArchivo, int& cantidad);
    static void eliminarArchivo(const char* nombreArchivo);
};

#endif
