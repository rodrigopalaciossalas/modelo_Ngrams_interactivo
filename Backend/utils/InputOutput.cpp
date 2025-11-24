#include <iostream>
#include "InputOutput.hpp"

void InputOutput::mostrarLista(char** lista, int cantidad) {
    for (int i = 0; i < cantidad; i++)
        std::cout << lista[i] << std::endl;
}
