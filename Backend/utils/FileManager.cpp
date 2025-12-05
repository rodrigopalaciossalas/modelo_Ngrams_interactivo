#include "FileManager.hpp"
#include <fstream>

bool FileManager::guardarLista(const char* ruta, const char** lista, int cantidad) {
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) return false;

    for (int i = 0; i < cantidad; i++) {
        archivo << lista[i];
        if (i < cantidad - 1) archivo << "\n";
    }
    return true;
}

bool FileManager::guardarListaAppend(const char* ruta, const char** lista, int cantidad) {
    std::ofstream archivo(ruta, std::ios::app);
    if (!archivo.is_open()) return false;

    for (int i = 0; i < cantidad; i++) {
        archivo << lista[i] << "\n";
    }
    return true;
}

char** FileManager::cargarLista(const char* ruta, int& cantidad) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cantidad = 0;
        return nullptr;
    }

    cantidad = 0;
    char buffer[300];

    while (archivo.getline(buffer, 300)) cantidad++;

    archivo.clear();
    archivo.seekg(0);

    if (cantidad == 0) return nullptr;

    char** lista = new char*[cantidad];
    int idx = 0;

    while (archivo.getline(buffer, 300)) {

        int len = 0;
        while (buffer[len] != '\0') len++;

        lista[idx] = new char[len + 1];

        for (int i = 0; i < len; i++)
            lista[idx][i] = buffer[i];
        lista[idx][len] = '\0';

        idx++;
    }

    return lista;
}
