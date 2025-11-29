cmake_minimum_required(VERSION 3.5)

project(Frontend VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# --- 1. AQUÍ CONECTAMOS LAS CABECERAS (HEADERS) DEL BACKEND ---
# Le decimos a CMake dónde buscar tus archivos .hpp
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/Ngram_Base
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/tipos/bigrams
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/tipos/unigrams
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/manager
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/utils
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        
        # --- 2. AQUÍ AGREGAMOS LA LÓGICA DEL BACKEND (.CPP) ---
        # NOTA: NO agregamos 'Backend/main.cpp' para evitar conflicto.
        # Solo agregamos los archivos que hacen el trabajo real.
        
        ../Backend/Ngrams/tipos/bigrams/Bigram.cpp
        # ../Backend/Ngrams/tipos/unigrams/Unigram.cpp (Si lo tienes)
        # ../Backend/Ngrams/manager/NgramManager.cpp (Si lo tienes)
        
        # Utils
        ../Backend/utils/FileManager.cpp
        ../Backend/utils/InputOutput.cpp
        ../Backend/utils/TextProcessor.cpp
        ../Backend/utils/tokenizador.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Frontend
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Frontend SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Frontend
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Frontend PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Configuración estándar para Mac/iOS/Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Frontend)
endif()
set_target_properties(Frontend PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Frontend
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Frontend)
endif()