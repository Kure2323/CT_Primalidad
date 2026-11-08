#include "../../include/algorithms/Ulam.h"

#include <stdexcept>
#include <string>

/** Coloca cada número siguiendo el recorrido en espiral. */
std::vector<PuntoUlam> ConstruirEspiralUlam(unsigned int limite) {
    constexpr unsigned int limite_maximo = 2'000;
    if (limite == 0 || limite > limite_maximo) {
        throw std::invalid_argument("El límite de la espiral debe estar entre 1 y " +
                                    std::to_string(limite_maximo) + ".");
    }

    std::vector<PuntoUlam> puntos;
    puntos.reserve(limite);
    int x = 0;
    int y = 0;
    puntos.push_back({1, x, y});

    constexpr int dx[] = {1, 0, -1, 0};
    constexpr int dy[] = {0, -1, 0, 1};
    unsigned int numero = 2;
    unsigned int longitud = 1;
    unsigned int direccion = 0;

    while (numero <= limite) {
        // La espiral recorre dos lados por longitud antes de aumentar el tamaño del tramo.
        for (int lado = 0; lado < 2 && numero <= limite; ++lado) {
            for (unsigned int paso = 0; paso < longitud && numero <= limite; ++paso) {
                x += dx[direccion];
                y += dy[direccion];
                puntos.push_back({numero++, x, y});
            }
            direccion = (direccion + 1) % 4;
        }
        ++longitud;
    }

    return puntos;
}
