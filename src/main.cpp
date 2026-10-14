#include <iostream>
#include <vector>

#include "../include/Eratosthenes.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

// Bug en CLION que no muestra en la consola de ejecución el programa correctamente.
// Para ello hay que ejecutar manualmente el compilado filan situado en:
// cmake-build-debug/Primos
int main() {

    for (int i : Eratosthenes(200)) {
        std::cout << i << ", ";
    }

    using namespace ftxui;

    // Define the document structure
    Element document = vbox({
      text("HELL YEAH") | bold | center,
      separator(),
      hbox({
        text("Left Panel") | border,
        vbox({
          text("Main Content Area") | flex,
          separator(),
          text("Footer Information") | dim,
        }) | border | flex,
      }) | flex,
    });

    // Create the screen and render
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    return 0;
}
