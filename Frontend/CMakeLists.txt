cmake_minimum_required(VERSION 3.5)
project(Frontend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/manager
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/Ngram_Base
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/Ngrams/tipos
    ${CMAKE_CURRENT_SOURCE_DIR}/../Backend/utils
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp

    ../Backend/Ngrams/manager/NGramManager.cpp
    ../Backend/Ngrams/manager/PredictionSession.cpp
    ../Backend/Ngrams/tipos/bigrams/Bigram.cpp
    ../Backend/utils/FileManager.cpp
    ../Backend/utils/tokenizador.cpp
    ../Backend/utils/TextBuilder.cpp
)

add_executable(Frontend ${PROJECT_SOURCES})
target_link_libraries(Frontend PRIVATE Qt6::Widgets)

set_target_properties(Frontend PROPERTIES WIN32_EXECUTABLE TRUE)

