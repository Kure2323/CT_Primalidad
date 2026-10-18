#include <iostream>
#include <numeric>
#include "../include/Eratosthenes.h"
#include "../include/Fermat.h"
#include "ftxui/component/app.hpp"
#include "ftxui/component/component.hpp"
#include <algorithm>
#include <charconv>
#include <memory>
#include <string>
#include <vector>
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

// Bug en CLION que no muestra en la consola de ejecución el programa correctamente.
// Para ello hay que ejecutar manualmente el compilado situado en:
// cmake-build-debug/Primos

// Debajo del main
bool is_prime(unsigned long n);
static Component build_eratosthenes();
static Component build_fermat();
static Component build_miller_rabin();
static Component build_primes();

int main() {
    srand(time(nullptr));

    Component eratosthenes { build_eratosthenes() };
    Component fermat { build_fermat() };
    Component miller_rabin { build_miller_rabin() };
    Component primes { build_primes() };

    const std::vector<std::string> entries {
        "Eratóstenes",
        "Test de Fermat",
        "Miller Rabin",
        "Proporción de primos"
    };
    int election = 0;

    // You can decorate components using the pipe operator.
    auto dashboard =
    Menu(&entries, &election, MenuOption::HorizontalAnimated());

    auto content = Container::Tab({
        eratosthenes,
        fermat,
        miller_rabin,
        primes
    }, &election);

    auto component = Container::Vertical({
        dashboard,
        content
    }) | border;

    // Start the main loop
    auto app = App::TerminalOutput().Fullscreen();
    app.Loop(component);

    return 0;
}

bool is_prime(unsigned long n) {
    if (n < 2) {return false;}
    if (n%2 == 0) {return n == 2;}

    for (unsigned long i = 3; i * i <= n; i+=2) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}

Component build_eratosthenes() {
    struct State {
        std::string fin = "200";
        std::string resultado = "Introduce un límite y pulsa Calcular.";
        std::vector<int> primes;
        unsigned int limite = 200;
        bool calculado = false;
    };

    auto state = std::make_shared<State>();

    auto input_fin = Input(&state->fin, "Fin");

    auto ejecutar = Button("Calcular", [state] {
        unsigned int fin;

        const char* inicio = state->fin.data();
        const char* final = inicio + state->fin.size();

        auto [ptr, ec] = std::from_chars(inicio, final, fin);

        if (state->fin.empty() ||
            ec != std::errc{} ||
            ptr != final) {
            state->resultado = "Introduce un unsigned int válido.";
            state->calculado = false;
            return;
        }

        auto primes = Eratosthenes(fin);

        state->primes = std::move(primes);
        state->limite = fin;
        state->calculado = true;

        state->resultado.clear();

        for (unsigned int i = 0; i < state->primes.size(); ++i) {
            if (i > 0) {
                state->resultado += ", ";
            }
            state->resultado += std::to_string(state->primes[i]);
        }

        if (state->primes.empty()) {
            state->resultado = "No se han encontrado primos.";
        }
    });

    auto inputs = Container::Horizontal({
        input_fin,
        ejecutar
    });

    return Renderer(inputs, [state, input_fin, ejecutar] {
        Elements filas;

        // Representación gráfica de los números del 1 al 30.
        for (unsigned int fila = 1; fila <= 30; fila += 10) {
            Elements celdas;

            for (unsigned int n = fila; n < fila + 10; ++n) {
                const bool primo = std::find(
                    state->primes.begin(),
                    state->primes.end(),
                    static_cast<int>(n)
                ) != state->primes.end();

                Element celda = text(std::to_string(n))
                    | center
                    | size(WIDTH, EQUAL, 5)
                    | border;

                if (state->calculado && n <= state->limite) {
                    if (primo) {
                        celda |= color(Color::Green);
                        celda |= bold;
                    } else if (n == 1) {
                        celda |= color(Color::Red);
                    } else {
                        celda |= color(Color::GrayDark);
                    }
                }

                celdas.push_back(celda);
            }

            filas.push_back(hbox(std::move(celdas)));
        }

        return vbox({
            text("Criba de Eratóstenes") | bold,
            separator(),

            text("Límite superior:"),
            hbox({
                input_fin->Render() | flex,
                text("     "),
                ejecutar->Render()
            }),

            separator(),
            text("Resultado:") | bold,
            paragraph(state->resultado),

            separator(),
            text("Representación gráfica (1-30)") | bold,
            hbox({
                text("Verde: primos"),
                text(" | "),
                text("Gris: compuestos"),
                text(" | "),
                text("Rojo: 1")
            }),
            vbox(std::move(filas)),

            separator(),
            text("¿Qué es la criba de Eratóstenes?") | bold,
            paragraph(
                "La criba de Eratóstenes es un algoritmo que permite "
                "encontrar todos los números primos menores o iguales "
                "que un límite dado."
            ),
            paragraph(
                "Primero se enumeran los números desde 2 hasta el límite. "
                "Después se eliminan los múltiplos de cada número primo, "
                "comenzando por sus cuadrados. Los números que quedan "
                "sin eliminar son primos."
            ),
            paragraph(
                "Por ejemplo, para encontrar los primos hasta 30, "
                "se eliminan los múltiplos de 2, de 3 y de los siguientes "
                "primos necesarios. Al final quedan 2, 3, 5, 7, 11, 13, "
                "17, 19, 23 y 29."
            )
        }) | border | yframe | yflex;
    });
}

Component build_fermat() {
    struct State {
        std::string numero = "561";
        std::string base = "0";
        std::string resultado = "Introduce un número y una base.";
        bool calculado = false;
        bool pasa_test = false;
    };

    auto state = std::make_shared<State>();

    auto input_numero = Input(&state->numero, "Número");
    auto input_base = Input(&state->base, "Base");

    auto ejecutar = Button("Calcular", [state] {
        unsigned int n;
        unsigned int base;

        auto [ptr_n, ec_n] = std::from_chars(
            state->numero.data(),
            state->numero.data() + state->numero.size(),
            n
        );

        auto [ptr_b, ec_b] = std::from_chars(
            state->base.data(),
            state->base.data() + state->base.size(),
            base
        );

        if (state->numero.empty() ||
            ec_n != std::errc{} ||
            ptr_n != state->numero.data() + state->numero.size() ||
            n < 2) {
            state->resultado = "Introduce un número entero mayor o igual que 2.";
            state->calculado = false;
            return;
        }

        if (state->base.empty() ||
            ec_b != std::errc{} ||
            ptr_b != state->base.data() + state->base.size() ||
            base < 0 || base >= n) {
            state->resultado = "La base debe estar entre 2 y n - 1.";
            state->calculado = false;
            return;
        }


        state->pasa_test = Fermat(n, base);
        state->calculado = true;

        if (state->pasa_test) {
            state->resultado =
                std::to_string(n) +
                " supera el test de Fermat para la base " +
                std::to_string(base) + ".\n\"Cociente de primalidad\": " + std::to_string(prime_ratio(n)) +
                    "\nEs un número de Carmichael: " + (is_carmichael(n) ? "true" : "false");
        } else {
            state->resultado =
                std::to_string(n) +
                " es compuesto: no supera el test de Fermat para la base " + std::to_string(base) + ".\n\"Cociente de primalidad\": " + std::to_string(prime_ratio(n)) +
                    "\nEs un número de Carmichael: " + (is_carmichael(n) ? "true" : "false");
        }
    });

    auto inputs = Container::Horizontal({
        input_numero,
        input_base,
        ejecutar
    });

    return Renderer(inputs, [state, input_numero, input_base, ejecutar] {
        Element resultado_visual;

        if (!state->calculado) {
            resultado_visual = text("Esperando el cálculo...")
                | color(Color::GrayDark);
        } else if (state->pasa_test) {
            resultado_visual = vbox({
                text("n^(a-1) mod a = 1") | bold | color(Color::Green),
                text("El número supera esta prueba, pero podría ser compuesto.")
            });
        } else {
            resultado_visual = vbox({
                text("n^(a-1) mod a != 1") | bold | color(Color::Red),
                text("El número es compuesto.")
            });
        }

        return vbox({
            text("Test de Fermat") | bold | center,
            separator(),

            text("Número a comprobar (n):"),
            hbox({
                input_numero->Render() | flex,
                text("    ")
            }),

            text("Base (a):"),
            hbox({
                input_base->Render() | flex,
                text("    "),
                ejecutar->Render()
            }),

            separator(),
            text("Resultado:") | bold,
            paragraph(state->resultado),

            separator(),
            text("Representación del test") | bold,
            resultado_visual,

            separator(),
            text("¿Qué es el test de Fermat?") | bold,
            paragraph(
                "El pequeño teorema de Fermat establece que, si p es "
                "primo y a no es divisible por p, entonces "
                "a^(p-1) es congruente con 1 módulo p."
            ),
            paragraph(
                "El test utiliza esta propiedad para comprobar si un "
                "número podría ser primo. Se elige una base a y se "
                "comprueba si a^(n-1) mod n es igual a 1."
            ),
            paragraph(
                "Si el resultado no es 1, el número es compuesto. "
                "Si es 1, el número supera la prueba para esa base, "
                "pero no queda demostrado que sea primo: existen "
                "números compuestos, como 561, que pueden superar "
                "el test para determinadas bases."
            ),
            paragraph(
                "Por eso se trata de una prueba probabilística y conviene "
                "probar varias bases. La elección de las bases influye "
                "en la fiabilidad del resultado."
                ),
            separator(),
            text("¿Qué son los números de Carmichael?") | bold,
            paragraph(
            "Los números de Carmichael son aquellos números compuestos "
            "que superan el test de Fermat para todas las bases coprimas con ellos."
            ),
        }) | border | yframe | yflex;
    });
}

Component build_miller_rabin() {
    return Renderer([] {
        return vbox({
            text("Test de Fermat") | bold,
            separator(),
            text("Aquí irá el formulario de Fermat.")
        });
    });
}

Component build_primes() {
    return Renderer([] {
         return vbox({
             text("Test de Fermat") | bold,
             separator(),
             text("Aquí irá el formulario de Fermat.")
         });
     });
}
