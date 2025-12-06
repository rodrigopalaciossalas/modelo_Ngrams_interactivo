# Modelo Predictivo N-Grams

Este es un proyecto para predecir palabras usando modelos Bigram y un poco de Unigram en C++ y Qt.

## Estructura
- **Backend**: Aquí está toda la lógica de los n-gramas (hecho a mano, sin usar librerías raras, todo con punteros).
- **Frontend**: La interfaz gráfica hecha con Qt.

## Como compilar y correr

Lo más fácil es usar **Qt Creator**:

1. Abres Qt Creator.
2. Le das a "Abrir Proyecto".
3. Buscas la carpeta `Frontend` y seleccionas el archivo `CMakeLists.txt`.
4. Te pedirá configurar un kit (selecciona el que tengas, normalmente Desktop MinGW o MSVC).
5. Esperas a que configure y le das al botón verde de **Play** (Run).

Y ya debería salir la ventanita.

## Nota importante
El programa necesita leer el archivo `2grams_english.txt` que está en `Backend/data/`. El código intenta buscarlo automáticamente subiendo de carpetas, así que no deberías tener problemas.
