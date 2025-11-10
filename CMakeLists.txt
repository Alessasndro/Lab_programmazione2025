cmake_minimum_required(VERSION 4.0)
project(Labb)

set(CMAKE_CXX_STANDARD 20)

add_executable(Labb main.cpp
        Transazione.cpp
        Transazione.h
        Gestore.cpp
        Gestore.h)
