cmake_minimum_required(VERSION 3.5)
project(Primos)

set(CMAKE_CXX_STANDARD 20)

add_executable(Primos
        src/main.cpp
        include/Eratosthenes.cpp
)

include(FetchContent)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v7.0.3  # Replace with a version, tag, or commit hash
)

FetchContent_MakeAvailable(ftxui)
target_link_libraries(Primos
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)