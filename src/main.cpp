#include <string>
#include <vector>
#include "ftxui/component/app.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

#include "../include/ui/pages/EratosthenesUI.h"
#include "../include/ui/pages/FermatUI.h"
#include "../include/ui/pages/CarmichaelUI.h"
#include "../include/ui/pages/MillerRabinUI.h"
#include "../include/ui/pages/PrimesUI.h"
#include "../include/ui/pages/UlamUI.h"
#include "../include/ui/pages/RsaPrimeUI.h"

using namespace ftxui;

// Configuración de ejecución en CLion:
// 1. Abrir Run > Edit Configurations.
// 2. Seleccionar la configuración de ejecución del proyecto (Primos).
// 3. Activar la opción «Run in external console».
// 4. Pulsar Apply y después OK.
//
// Esto permite visualizar correctamente la interfaz de FTXUI
// en una consola externa.

int main() {
    // Cada pantalla conserva su propio estado mientras permanezca abierta.
    Component pantalla_eratostenes{CrearPantallaEratostenes()};
    Component pantalla_fermat{CrearPantallaFermat()};
    Component pantalla_carmichael{CrearPantallaCarmichael()};
    Component pantalla_miller_rabin{CrearPantallaMillerRabin()};
    Component pantalla_proporcion_primos{CrearPantallaProporcionPrimos()};
    Component pantalla_ulam{CrearPantallaUlam()};
    Component pantalla_generador_rsa{CrearPantallaGeneradorRsa()};

    // El orden del menú debe coincidir con el de las pantallas de Container::Tab.
    const std::vector<std::string> opciones {
        "Eratóstenes",
        "Test de Fermat",
        "Carmichael",
        "Miller Rabin",
        "Proporción de primos",
        "Ulam",
        "Generador RSA",
    };
    int seleccion = 0;

    auto menu_principal = Menu(&opciones, &seleccion, MenuOption::HorizontalAnimated());

    auto contenido = Container::Tab({
        pantalla_eratostenes,
        pantalla_fermat,
        pantalla_carmichael,
        pantalla_miller_rabin,
        pantalla_proporcion_primos,
        pantalla_ulam,
        pantalla_generador_rsa
    }, &seleccion);

    auto componente = Container::Vertical({
        menu_principal,
        contenido
    }) | border;

    auto aplicacion = App::TerminalOutput().Fullscreen();
    aplicacion.Loop(componente);

    return 0;
}
