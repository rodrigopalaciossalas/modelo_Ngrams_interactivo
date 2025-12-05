#include "Unigrams.hpp"
#include "../../../utils/FileManager.hpp"


static int cadenaAEntero_local(const char* s) {
	if (!s) return 0;
	int i = 0;
	while (s[i] == ' ') i++;
	int val = 0;
	while (s[i] >= '0' && s[i] <= '9') {
		val = val * 10 + (s[i] - '0');
		i++;
	}
	return val;
}

static void copiarCadena_local(char* dest, const char* src, int len) {
	for (int i = 0; i < len; i++) dest[i] = src[i];
	dest[len] = '\0';
}

Unigram::Unigram() : NGramBase(1) {
	palabras = nullptr;
	frecuencias = nullptr;
	cantidad = 0;
}

Unigram::~Unigram() {
	limpiar();
}

int Unigram::buscarPalabra(const char* palabra) {
	if (!palabras) return -1;
	int i = 0;
	while (i < cantidad) {
		int j = 0;
		bool igual = true;
		while (palabra[j] != '\0' || palabras[i][j] != '\0') {
			if (palabra[j] != palabras[i][j]) { igual = false; break; }
			j++;
		}
		if (igual) return i;
		i++;
	}
	return -1;
}

void Unigram::agregarPalabra(const char* palabra) {
	int len = 0; while (palabra[len] != '\0') len++;
	if (palabras == nullptr) {
		palabras = new char*[10];
		frecuencias = new int[10];
		cantidad = 0;
	}




	static int capacidad = 10;
	if (cantidad >= capacidad) {
		int nuevaCap = capacidad * 2;
		char** np = new char*[nuevaCap];
		int* nf = new int[nuevaCap];
		for (int i = 0; i < cantidad; i++) { np[i] = palabras[i]; nf[i] = frecuencias[i]; }
		delete[] palabras; delete[] frecuencias;
		palabras = np; frecuencias = nf; capacidad = nuevaCap;
	}

	char* copia = new char[len + 1];
	for (int i = 0; i < len; i++) copia[i] = palabra[i];
	copia[len] = '\0';

	palabras[cantidad] = copia;
	frecuencias[cantidad] = 1;
	cantidad++;
}

void Unigram::procesarTokens(char** tokens, int cantidadTokens) {
	limpiar();
	if (!tokens || cantidadTokens <= 0) return;

	for (int i = 0; i < cantidadTokens; i++) {
		int idx = buscarPalabra(tokens[i]);
		if (idx >= 0) {
			frecuencias[idx]++;
		} else {
			agregarPalabra(tokens[i]);
		}
	}
}

char** Unigram::obtenerNgrams(int& cantidadOut) {
	if (cantidad == 0) { cantidadOut = 0; return nullptr; }
	char** salida = new char*[cantidad];
	for (int i = 0; i < cantidad; i++) {

		int t = 0; while (palabras[i][t] != '\0') t++;

		char num[12]; int pos = 0; int f = frecuencias[i]; if (f <= 0) f = 1;
		int temp = f; while (temp > 0) { num[pos++] = (temp % 10) + '0'; temp /= 10; }
		if (pos == 0) { num[pos++] = '0'; }
		num[pos] = '\0';
		char numFinal[12]; int k2 = 0; for (int p = pos - 1; p >= 0; p--) numFinal[k2++] = num[p]; numFinal[k2] = '\0';

		int total = t + k2 + 4;
		char* linea = new char[total];
		int k = 0;
		for (int j = 0; j < t; j++) linea[k++] = palabras[i][j];
		linea[k++] = ' '; linea[k++] = '-'; linea[k++] = ' ';
		int p = 0; while (numFinal[p] != '\0') linea[k++] = numFinal[p++]; linea[k] = '\0';

		salida[i] = linea;
	}
	cantidadOut = cantidad;
	return salida;
}

void Unigram::limpiar() {
	if (palabras) {
		for (int i = 0; i < cantidad; i++) delete[] palabras[i];
		delete[] palabras;
	}
	if (frecuencias) delete[] frecuencias;
	palabras = nullptr; frecuencias = nullptr; cantidad = 0;
}

const char* Unigram::predecir(const char* ultimaPalabra) {
	if (!palabras || cantidad == 0 || !ultimaPalabra) return "";
	int len = 0; while (ultimaPalabra[len] != '\0') len++;

	int mejorIdx = -1; int mejorFreq = -1;
	for (int i = 0; i < cantidad; i++) {
		int j = 0; bool coincide = true;
		while (j < len) {
			if (palabras[i][j] != ultimaPalabra[j]) { coincide = false; break; }
			j++;
		}
		if (coincide) {
			if (frecuencias[i] > mejorFreq) { mejorFreq = frecuencias[i]; mejorIdx = i; }
		}
	}
	if (mejorIdx == -1) return "";
	return palabras[mejorIdx];
}

void Unigram::cargarDatos(const char* rutaArchivo) {
	int lineas = 0;
	char** lista = FileManager::cargarLista(rutaArchivo, lineas);
	if (!lista || lineas == 0) return;

	for (int i = 0; i < lineas; i++) {
		char* linea = lista[i];
		int len = 0; while (linea[len] != '\0') len++;
		int posGuion = -1;
		for (int j = 0; j < len; j++) if (linea[j] == '-' && posGuion == -1) posGuion = j;
		if (posGuion != -1) {
			int freq = cadenaAEntero_local(linea + posGuion + 2);
			int lenTexto = posGuion - 1;
			if (lenTexto > 0) {
				char* texto = new char[lenTexto + 1];
				copiarCadena_local(texto, linea, lenTexto);
				int idx = buscarPalabra(texto);
				if (idx >= 0) frecuencias[idx] += freq;
				else {

					if (palabras == nullptr) { palabras = new char*[10]; frecuencias = new int[10]; cantidad = 0; }

					static int capacidad = 10;
					if (cantidad >= capacidad) {
						int nueva = capacidad * 2;
						char** np = new char*[nueva]; int* nf = new int[nueva];
						for (int k = 0; k < cantidad; k++) { np[k] = palabras[k]; nf[k] = frecuencias[k]; }
						delete[] palabras; delete[] frecuencias;
						palabras = np; frecuencias = nf; capacidad = nueva;
					}
					palabras[cantidad] = texto;
					frecuencias[cantidad] = freq;
					cantidad++;
				}
			}
		}
		delete[] linea;
	}
	delete[] lista;
}
