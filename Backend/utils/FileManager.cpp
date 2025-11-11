#include "FileManager.hpp"

const char* FileManager::DATA_PATH = "data/";

bool FileManager::existeArchivo(const char* nombreArchivo) {
    char ruta[100];
    int i = 0, j = 0;
    while (DATA_PATH[i] != '\0') {
        ruta[i] = DATA_PATH[i];
        i++;
    }
    while (nombreArchivo[j] != '\0') {
        ruta[i++] = nombreArchivo[j++];
    }
    ruta[i] = '\0';

    std::ifstream archivo(ruta);
    return archivo.good();
}

void FileManager::guardarLista(const char* nombreArchivo, const char** datos, int cantidad) {
    char ruta[100];
    int i = 0, j = 0;
    while (DATA_PATH[i] != '\0') {
        ruta[i] = DATA_PATH[i];
        i++;
    }
    while (nombreArchivo[j] != '\0') {
        ruta[i++] = nombreArchivo[j++];
    }
    ruta[i] = '\0';

    std::ofstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir " << ruta << " para escritura.\n";
        return;
    }

    for (int k = 0; k < cantidad; k++) {
        archivo << datos[k] << "\n";
    }

    archivo.close();
}

char** FileManager::cargarLista(const char* nombreArchivo, int& cantidad) {
    char ruta[100];
    int i = 0, j = 0;
    while (DATA_PATH[i] != '\0') {
        ruta[i] = DATA_PATH[i];
        i++;
    }
    while (nombreArchivo[j] != '\0') {
        ruta[i++] = nombreArchivo[j++];
    }
    ruta[i] = '\0';

    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir " << ruta << " para lectura" << std::endl;
        cantidad = 0;
        return nullptr;
    }

    cantidad = 0;
    char linea[256];
    while (archivo.getline(linea, 256)) {
        cantidad++;
    }

    archivo.clear();
    archivo.seekg(0, std::ios::beg);

    char** lista = new char*[cantidad];
    for (int k = 0; k < cantidad; k++) {
        lista[k] = new char[256];
    }

    int idx = 0;
    while (archivo.getline(linea, 256)) {
        int l = 0;
        while (linea[l] != '\0') {
            lista[idx][l] = linea[l];
            l++;
        }
        lista[idx][l] = '\0';
        idx++;
    }

    archivo.close();
    return lista;
}

void FileManager::eliminarArchivo(const char* nombreArchivo) {
    char ruta[100];
    int i = 0, j = 0;
    while (DATA_PATH[i] != '\0') {
        ruta[i] = DATA_PATH[i];
        i++;
    }
    while (nombreArchivo[j] != '\0') {
        ruta[i++] = nombreArchivo[j++];
    }
    ruta[i] = '\0';

    std::remove(ruta);
}
