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
        if (rutaGeneral != nullptr) {
            int lineasDB = 0;
            char** dbContenido = FileManager::cargarLista(rutaGeneral, lineasDB);
            
            char** lineasNuevas = new char*[cantidadOut];
            int cantidadNuevas = 0;
            
            for (int i = 0; i < cantidadOut; i++) {
                bool yaExiste = false;
                for (int j = 0; j < lineasDB; j++) {

                    int k = 0;
                    bool igual = true;
                    while (lines[i][k] != '\0' || dbContenido[j][k] != '\0') {
                        if (lines[i][k] != dbContenido[j][k]) { igual = false; break; }
                        k++;
                    }
                    if (igual) { yaExiste = true; break; }
                }
                if (!yaExiste) {
                    lineasNuevas[cantidadNuevas++] = lines[i];
                }
            }
            

            if (cantidadNuevas > 0) {
                FileManager::guardarListaAppend(rutaGeneral, (const char**)lineasNuevas, cantidadNuevas);
            }
            

            delete[] lineasNuevas;
            if (dbContenido) {
                for (int i = 0; i < lineasDB; i++) delete[] dbContenido[i];
                delete[] dbContenido;
            }
        }
    }

    modelo->limpiar();
    tz.limpiar();
}
void NGramManager::inicializarModelo(NGramBase* modelo, const char* rutaArchivo) {
    if (modelo == nullptr || rutaArchivo == nullptr) return;

    modelo->cargarDatos(rutaArchivo);
}

void NGramManager::construirModeloDesdeArchivo(NGramBase* modelo, const char* rutaTexto) {
    if (modelo == nullptr || rutaTexto == nullptr) return;

    int lineas = 0;
    char** lista = FileManager::cargarLista(rutaTexto, lineas);
    if (lista == nullptr || lineas == 0) return;


    int totalLen = 0;
    for (int i = 0; i < lineas; i++) {
        int l = 0; while (lista[i][l] != '\0') l++;
        totalLen += l + 1;
    }

    char* todo = new char[totalLen + 1];
    int pos = 0;
    for (int i = 0; i < lineas; i++) {
        int j = 0;
        while (lista[i][j] != '\0') todo[pos++] = lista[i][j++];
        if (i < lineas - 1) todo[pos++] = ' ';
    }
    todo[pos] = '\0';


    for (int i = 0; i < lineas; i++) delete[] lista[i];
    delete[] lista;

    Tokenizador tz;
    tz.tokenizar(todo);
    char** tokens = tz.obtenerTokens();
    int nTokens = tz.obtenerCantidad();

    modelo->procesarTokens(tokens, nTokens);

    tz.limpiar();
    delete[] todo;
}

const char* NGramManager::predecir(NGramBase* modelo, const char* ultimaPalabra) {
    if (modelo == nullptr || ultimaPalabra == nullptr) return "";
    return modelo->predecir(ultimaPalabra);
}

char** NGramManager::predecirTop(NGramBase* modelo, const char* ultimaPalabra, int cantidadOpciones) {
    if (modelo == nullptr || ultimaPalabra == nullptr || cantidadOpciones <= 0) return nullptr;
    

    int totalNgrams = 0;
    char** todosNgrams = modelo->obtenerNgrams(totalNgrams);
    if (todosNgrams == nullptr || totalNgrams == 0) return nullptr;
    

    char** candidatos = new char*[totalNgrams];
    int* frecuenciaCandidatos = new int[totalNgrams];
    int cantidadCandidatos = 0;
    
    int lenPalabra = 0; while (ultimaPalabra[lenPalabra] != '\0') lenPalabra++;
    
    for (int i = 0; i < totalNgrams; i++) {
        char* bigram = todosNgrams[i];
        


        int lenBigram = 0; while (bigram[lenBigram] != '\0' && bigram[lenBigram] != ' ' && bigram[lenBigram] != '-') lenBigram++;
        

        int posGuion = -1;
        for (int j = 0; bigram[j] != '\0'; j++) {
            if (bigram[j] == '-' && j > 0 && bigram[j-1] == ' ') { posGuion = j - 1; break; }
        }
        
        if (posGuion > 0) {

            char* textoCompleto = new char[posGuion + 1];
            for (int k = 0; k < posGuion; k++) textoCompleto[k] = bigram[k];
            textoCompleto[posGuion] = '\0';
            

            bool coincide = true;
            for (int j = 0; j < lenPalabra; j++) {
                if (textoCompleto[j] != ultimaPalabra[j]) { coincide = false; break; }
            }
            

            if (coincide && textoCompleto[lenPalabra] == ' ') {

                int freq = 0;
                for (int j = posGuion + 1; bigram[j] != '\0'; j++) {
                    if (bigram[j] >= '0' && bigram[j] <= '9') {
                        freq = freq * 10 + (bigram[j] - '0');
                    }
                }
                
                candidatos[cantidadCandidatos] = textoCompleto;
                frecuenciaCandidatos[cantidadCandidatos] = freq;
                cantidadCandidatos++;
            } else {
                delete[] textoCompleto;
            }
        }
    }
    

    for (int i = 0; i < cantidadCandidatos - 1; i++) {
        for (int j = i + 1; j < cantidadCandidatos; j++) {
            if (frecuenciaCandidatos[j] > frecuenciaCandidatos[i]) {

                int tmpFreq = frecuenciaCandidatos[i];
                frecuenciaCandidatos[i] = frecuenciaCandidatos[j];
                frecuenciaCandidatos[j] = tmpFreq;
                

                char* tmpCand = candidatos[i];
                candidatos[i] = candidatos[j];
                candidatos[j] = tmpCand;
            }
        }
    }
    

    int cantidadResultado = (cantidadCandidatos < cantidadOpciones) ? cantidadCandidatos : cantidadOpciones;
    char** resultado = new char*[cantidadResultado];
    for (int i = 0; i < cantidadResultado; i++) {
        resultado[i] = candidatos[i];
    }
    

    for (int i = cantidadResultado; i < cantidadCandidatos; i++) {
        delete[] candidatos[i];
    }
    delete[] candidatos;
    delete[] frecuenciaCandidatos;
    
    return resultado;
}

char** NGramManager::predecirSiguiente(const char* ultimaPalabra, int cantidadOpciones) {
    if (ultimaPalabra == nullptr || cantidadOpciones <= 0) return nullptr;

    return nullptr;
}

void NGramManager::registrarPrediccion(const char* palabra1, const char* palabra2, const char* rutaDB) {
    if (!palabra1 || !palabra2 || !rutaDB) return;

    int lineasDB = 0;
    char** dbContenido = FileManager::cargarLista(rutaDB, lineasDB);

    int lenP1 = 0; while (palabra1[lenP1] != '\0') lenP1++;
    int lenP2 = 0; while (palabra2[lenP2] != '\0') lenP2++;

    bool encontrado = false;
    for (int i = 0; i < lineasDB; i++) {
        int lenLinea = 0;
        while (dbContenido[i][lenLinea] != '\0' && dbContenido[i][lenLinea] != ' ' && 
               dbContenido[i][lenLinea] != '-') lenLinea++;

        bool coincide = true;
        for (int j = 0; j < lenP1; j++) {
            if (dbContenido[i][j] != palabra1[j]) { coincide = false; break; }
        }

        if (coincide && dbContenido[i][lenP1] == ' ') {
            int posEspacio2 = lenP1 + 1;
            int lenP2Actual = 0;
            while (dbContenido[i][posEspacio2 + lenP2Actual] != '\0' && 
                   dbContenido[i][posEspacio2 + lenP2Actual] != ' ') lenP2Actual++;

            if (lenP2Actual == lenP2) {
                bool iguales = true;
                for (int j = 0; j < lenP2; j++) {
                    if (dbContenido[i][posEspacio2 + j] != palabra2[j]) { iguales = false; break; }
                }

                if (iguales) {
                    int posGuion = -1;
                    for (int j = 0; dbContenido[i][j] != '\0'; j++) {
                        if (dbContenido[i][j] == '-' && j > 0 && dbContenido[i][j-1] == ' ') {
                            posGuion = j - 1;
                            break;
                        }
                    }

                    if (posGuion > 0) {
                        int frecuencia = 0;
                        for (int j = posGuion + 1; dbContenido[i][j] != '\0'; j++) {
                            if (dbContenido[i][j] >= '0' && dbContenido[i][j] <= '9') {
                                frecuencia = frecuencia * 10 + (dbContenido[i][j] - '0');
                            }
                        }

                        frecuencia++;

                        char buffer[256];
                        int pos = 0;
                        for (int j = 0; palabra1[j] != '\0'; j++) buffer[pos++] = palabra1[j];
                        buffer[pos++] = ' ';
                        for (int j = 0; palabra2[j] != '\0'; j++) buffer[pos++] = palabra2[j];
                        buffer[pos++] = ' ';
                        buffer[pos++] = '-';
                        buffer[pos++] = ' ';

                        int freqLen = 0;
                        int tmpFreq = frecuencia;
                        while (tmpFreq > 0) { freqLen++; tmpFreq /= 10; }
                        if (freqLen == 0) freqLen = 1;
                        tmpFreq = frecuencia;
                        for (int j = freqLen - 1; j >= 0; j--) {
                            buffer[pos + j] = '0' + (tmpFreq % 10);
                            tmpFreq /= 10;
                        }
                        pos += freqLen;
                        buffer[pos] = '\0';

                        delete[] dbContenido[i];
                        dbContenido[i] = new char[pos + 1];
                        for (int j = 0; buffer[j] != '\0'; j++) dbContenido[i][j] = buffer[j];
                        dbContenido[i][pos] = '\0';

                        encontrado = true;
                        break;
                    }
                }
            }
        }
    }

    if (!encontrado) {
        char buffer[256];
        int pos = 0;
        for (int j = 0; palabra1[j] != '\0'; j++) buffer[pos++] = palabra1[j];
        buffer[pos++] = ' ';
        for (int j = 0; palabra2[j] != '\0'; j++) buffer[pos++] = palabra2[j];
        buffer[pos++] = ' ';
        buffer[pos++] = '-';
        buffer[pos++] = ' ';
        buffer[pos++] = '1';
        buffer[pos] = '\0';

        const char* nuevoEntry = buffer;
        FileManager::guardarListaAppend(rutaDB, &nuevoEntry, 1);
    } else {
        FileManager::guardarLista(rutaDB, (const char**)dbContenido, lineasDB);
    }

    if (dbContenido) {
        for (int i = 0; i < lineasDB; i++) delete[] dbContenido[i];
        delete[] dbContenido;
    }
}
