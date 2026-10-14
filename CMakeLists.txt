cmake_minimum_required(VERSION 3.5)
project(Primos)

set(CMAKE_CXX_STANDARD 20)

add_executable(Primos
        src/main.cpp
        include/Eratosthenes.cpp
)
