#include <iostream>
#include "Ngrams/manager/NGramManager.hpp"
#include "Ngrams/manager/PredictionSession.hpp"
#include "Ngrams/tipos/bigrams/Bigram.hpp"
#include "utils/FileManager.hpp"

int main() {
    std::cout << "=== TEST DE INCREMENTO DE FRECUENCIAS ===" << std::endl;
    
    const char* rutaDB = "data/2grams_english.txt";
    
    std::cout << "\n1. Cargando BD inicial..." << std::endl;
    int lineas1 = 0;
    char** db1 = FileManager::cargarLista(rutaDB, lineas1);
    
    if (db1 && lineas1 > 0) {
        std::cout << "Total lineas en BD: " << lineas1 << std::endl;
        
        bool encontrado = false;
        for (int i = 0; i < lineas1; i++) {
            if (db1[i][0] == 't' && db1[i][1] == 'h' && db1[i][2] == 'e' && db1[i][3] == ' ') {
                std::cout << "Linea encontrada: " << db1[i] << std::endl;
                if (i == 0) {
                    encontrado = true;
                    break;
                }
            }
        }
        
        std::cout << "\n2. Leyendo frecuencia inicial de 'the same'..." << std::endl;
        int freqAntes = 0;
        for (int i = 0; i < lineas1; i++) {
            int j = 0;
            if (db1[i][0] == 't' && db1[i][1] == 'h' && db1[i][2] == 'e' && db1[i][3] == ' ' &&
                db1[i][4] == 's' && db1[i][5] == 'a' && db1[i][6] == 'm' && db1[i][7] == 'e') {
                
                while (db1[i][j] != '\0') j++;
                j--;
                int freq = 0;
                int multiplier = 1;
                while (j >= 0 && db1[i][j] >= '0' && db1[i][j] <= '9') {
                    freq += (db1[i][j] - '0') * multiplier;
                    multiplier *= 10;
                    j--;
                }
                freqAntes = freq;
                std::cout << "Frecuencia inicial 'the same': " << freqAntes << std::endl;
                break;
            }
        }
        
        std::cout << "\n3. Registrando predicción 'the' -> 'same'..." << std::endl;
        NGramManager manager;
        manager.registrarPrediccion("the", "same", rutaDB);
        std::cout << "Prediccion registrada." << std::endl;
        
        std::cout << "\n4. Leyendo frecuencia DESPUÉS del registro..." << std::endl;
        int lineas2 = 0;
        char** db2 = FileManager::cargarLista(rutaDB, lineas2);
        
        int freqDespues = 0;
        for (int i = 0; i < lineas2; i++) {
            if (db2[i][0] == 't' && db2[i][1] == 'h' && db2[i][2] == 'e' && db2[i][3] == ' ' &&
                db2[i][4] == 's' && db2[i][5] == 'a' && db2[i][6] == 'm' && db2[i][7] == 'e') {
                
                int j = 0;
                while (db2[i][j] != '\0') j++;
                j--;
                int freq = 0;
                int multiplier = 1;
                while (j >= 0 && db2[i][j] >= '0' && db2[i][j] <= '9') {
                    freq += (db2[i][j] - '0') * multiplier;
                    multiplier *= 10;
                    j--;
                }
                freqDespues = freq;
                std::cout << "Frecuencia DESPUÉS 'the same': " << freqDespues << std::endl;
                std::cout << "Linea actual: " << db2[i] << std::endl;
                break;
            }
        }
        
        std::cout << "\n=== RESULTADO ===" << std::endl;
        if (freqDespues > freqAntes) {
            std::cout << "✅ EXITO: Frecuencia se incrementó de " << freqAntes << " a " << freqDespues << std::endl;
        } else {
            std::cout << "❌ FALLO: Frecuencia NO se incrementó. Antes: " << freqAntes << ", Después: " << freqDespues << std::endl;
        }
        
        std::cout << "\n5. Registrando otra predicción 'the' -> 'same'..." << std::endl;
        manager.registrarPrediccion("the", "same", rutaDB);
        
        std::cout << "\n6. Leyendo frecuencia tras segundo registro..." << std::endl;
        int lineas3 = 0;
        char** db3 = FileManager::cargarLista(rutaDB, lineas3);
        
        int freqFinal = 0;
        for (int i = 0; i < lineas3; i++) {
            if (db3[i][0] == 't' && db3[i][1] == 'h' && db3[i][2] == 'e' && db3[i][3] == ' ' &&
                db3[i][4] == 's' && db3[i][5] == 'a' && db3[i][6] == 'm' && db3[i][7] == 'e') {
                
                int j = 0;
                while (db3[i][j] != '\0') j++;
                j--;
                int freq = 0;
                int multiplier = 1;
                while (j >= 0 && db3[i][j] >= '0' && db3[i][j] <= '9') {
                    freq += (db3[i][j] - '0') * multiplier;
                    multiplier *= 10;
                    j--;
                }
                freqFinal = freq;
                std::cout << "Frecuencia FINAL 'the same': " << freqFinal << std::endl;
                std::cout << "Linea final: " << db3[i] << std::endl;
                break;
            }
        }
        
        std::cout << "\n=== RESULTADO FINAL ===" << std::endl;
        if (freqFinal == freqDespues + 1) {
            std::cout << "✅ EXITO: Segundo incremento funcionó. " << freqDespues << " -> " << freqFinal << std::endl;
        } else {
            std::cout << "❌ FALLO: Segundo incremento no funcionó. Esperado: " << (freqDespues + 1) << ", Obtenido: " << freqFinal << std::endl;
        }
        
        if (db1) {
            for (int i = 0; i < lineas1; i++) delete[] db1[i];
            delete[] db1;
        }
        if (db2) {
            for (int i = 0; i < lineas2; i++) delete[] db2[i];
            delete[] db2;
        }
        if (db3) {
            for (int i = 0; i < lineas3; i++) delete[] db3[i];
            delete[] db3;
        }
    }
    
    return 0;
}
