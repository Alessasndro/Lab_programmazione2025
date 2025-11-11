# Aggiornato il requisito minimo di CMake a 3.10 per supportare add_subdirectory in modo moderno
cmake_minimum_required(VERSION 3.10)
project(Labb)

set(CMAKE_CXX_STANDARD 20)

# ===============================================
# INCLUSIONE INTERNA DI GOOGLE TEST
# ===============================================
# Aggiunge il progetto googletest come sottodirectory.
# Usa il percorso dove hai clonato GTest.
add_subdirectory(Test/googletest)

# ===============================================
# ESEGUIBILE PRINCIPALE (Labb)
# ===============================================
add_executable(Labb main.cpp
        Transazione.cpp
        Transazione.h
        Gestore.cpp
        Gestore.h)

# ===============================================
# ESEGUIBILE DI UNIT TEST
# ===============================================

# Include i file sorgente per i test dalla cartella Test/test1/
add_executable(unit_tests
        Test/test1.cpp)

# Collega l'eseguibile di test alle librerie di Google Test (gtest_main)
# e ai file di implementazione delle tue classi.
target_link_libraries(unit_tests
        # Target fornito da add_subdirectory(Test/googletest)
        gtest_main

        # Collega le implementazioni delle tue classi
        Transazione.cpp
        Gestore.cpp
)

# Assicurati che l'eseguibile di test possa trovare i file header del progetto (.h)
target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR} # Per trovare Transazione.h e Gestore.h
)

# Abilita CTest per l'esecuzione dei test
enable_testing()