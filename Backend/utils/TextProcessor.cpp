#include "TextProcessor.hpp"

char* TextProcessor::limpiarEspacios(const char* texto) {
    if (!texto) return nullptr;

    int len = 0;
    while (texto[len] != '\0') len++;

    char* limpio = new char[len + 1];

    int j = 0;
    bool espacioPrevio = false;

    for (int i = 0; i < len; i++) {
        char c = texto[i];

        if (c == ' ') {
            if (!espacioPrevio) {
                limpio[j++] = ' ';
                espacioPrevio = true;
            }
        } else {
            limpio[j++] = c;
            espacioPrevio = false;
        }
    }

    limpio[j] = '\0';
    return limpio;
}
