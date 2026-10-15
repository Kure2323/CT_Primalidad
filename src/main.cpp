#include <iostream>
#include <numeric>
#include <vector>

#include "../include/Eratosthenes.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "../include/Fermat.h"

// Bug en CLION que no muestra en la consola de ejecución el programa correctamente.
// Para ello hay que ejecutar manualmente el compilado situado en:
// cmake-build-debug/Primos

bool esPrimo(unsigned long n);

int main() {
    srand(time(nullptr)); // Needed


    // Deprisa y mal implementado pero es el ejercicio 2
    const unsigned int n = 561;
    unsigned int base = 2;

    for (int i = 2; i<n; i++) {
        if (std::gcd(n, i) == 1) {
            base = i;
            if (Fermat(n, base) == true) {
                std::cout << n << " es primo en base " << base << '\n';
            } else {
                std::cout << n << " no es primo en base " << base << '\n';
            }
        }
    }

    std::cout << "Cociente de primalidad del 561: " << cocienteDePrimalidad(561);


    std::cout << esPrimo(n);

    // using namespace ftxui;
    //
    // // Define the document structure
    // Element document = vbox({
    //   text("HELL YEAH") | bold | center,
    //   separator(),
    //   hbox({
    //     text("Left Panel") | border,
    //     vbox({
    //       text("Main Content Area") | flex,
    //       separator(),
    //       text("Footer Information") | dim,
    //     }) | border | flex,
    //   }) | flex,
    // });
    //
    // // Create the screen and render
    // auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    // Render(screen, document);
    // screen.Print();
    return 0;
}

bool esPrimo(unsigned long n) {
    if (n < 2) {return false;}
    if (n%2 == 0) {return n == 2;}

    for (unsigned long i = 3; i * i <= n; i+=2) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}

