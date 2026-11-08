cmake_minimum_required(VERSION 3.5)
project(Primos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Primos
        src/main.cpp
        src/algorithms/Eratosthenes.cpp
        include/algorithms/Eratosthenes.h
        src/algorithms/Fermat.cpp
        include/algorithms/Fermat.h
        src/algorithms/NumberTheory.cpp
        include/algorithms/NumberTheory.h
        src/ui/pages/EratosthenesUI.cpp
        include/ui/pages/EratosthenesUI.h
        src/ui/animation/EratosthenesAnimation.cpp
        include/ui/animation/EratosthenesAnimation.h
        src/ui/pages/FermatUI.cpp
        include/ui/pages/FermatUI.h
        src/ui/pages/CarmichaelUI.cpp
        include/ui/pages/CarmichaelUI.h
        src/ui/animation/CarmichaelAnimation.cpp
        include/ui/animation/CarmichaelAnimation.h
        src/ui/pages/MillerRabinUI.cpp
        include/ui/pages/MillerRabinUI.h
        src/ui/pages/PrimesUI.cpp
        include/ui/pages/PrimesUI.h
        src/ui/pages/UlamUI.cpp
        include/ui/pages/UlamUI.h
        src/algorithms/Ulam.cpp
        include/algorithms/Ulam.h
        src/algorithms/MillerRabin.cpp
        include/algorithms/MillerRabin.h
        src/ui/common/ResultView.cpp
        include/ui/common/ResultView.h
        src/ui/animation/StepsView.cpp
        include/ui/animation/StepsView.h
        src/ui/animation/StepsAnimation.cpp
        include/ui/animation/StepsAnimation.h
        src/ui/animation/FermatAnimation.cpp
        include/ui/animation/FermatAnimation.h
        src/ui/animation/MillerRabinAnimation.cpp
        include/ui/animation/MillerRabinAnimation.h
        src/ui/common/ExplanationView.cpp
        include/ui/common/ExplanationView.h
        src/utils/InputUtils.cpp
        include/utils/InputUtils.h
        include/utils/ExplanationStep.h
        src/utils/RandomUtils.cpp
        include/utils/RandomUtils.h
        src/ui/common/ScreenLayout.cpp
        include/ui/common/ScreenLayout.h
        src/ui/common/InputSection.cpp
        include/ui/common/InputSection.h
        src/ui/common/RandomBaseControl.cpp
        include/ui/common/RandomBaseControl.h
        src/algorithms/Prime.cpp
        include/algorithms/Prime.h
        src/algorithms/RsaPrimeGenerator.cpp
        include/algorithms/RsaPrimeGenerator.h
        src/ui/pages/RsaPrimeUI.cpp
        include/ui/pages/RsaPrimeUI.h
)

include(FetchContent)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v7.0.3  # Etiqueta o confirmación de la versión de FTXUI.
)

FetchContent_Declare(boost_multiprecision
        GIT_REPOSITORY https://github.com/boostorg/multiprecision.git
        GIT_TAG boost-1.86.0
        GIT_SHALLOW TRUE
)
FetchContent_Declare(boost_config
        GIT_REPOSITORY https://github.com/boostorg/config.git
        GIT_TAG boost-1.86.0
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(ftxui boost_multiprecision boost_config)
target_include_directories(Primos PRIVATE ${boost_multiprecision_SOURCE_DIR}/include ${boost_config_SOURCE_DIR}/include)
target_link_libraries(Primos
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)
