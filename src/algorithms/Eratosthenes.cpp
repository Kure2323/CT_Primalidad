#include "../../include/algorithms/Eratosthenes.h"
#include <vector>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <string>

/**
 * Aplica la criba: marca los múltiplos de cada primo desde su cuadrado.
 */
std::vector<int> CribaEratostenes(const unsigned int limite) {
    constexpr unsigned int limite_maximo = 10'000;

    if (limite > limite_maximo || limite > static_cast<unsigned int>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("El límite de Eratóstenes debe estar entre 0 y " +
                                    std::to_string(limite_maximo) + ".");
    }

    // No hay números primos anteriores al 2.
    if (limite < 2) return {};

    // Cada posición del vector indica si el número correspondiente sigue siendo candidato a primo.
    std::vector<bool> es_primo(limite + 1, true);

    // El 0 y el 1 no son números primos.
    es_primo[0] = false;
    es_primo[1] = false;


    // Solo hace falta buscar divisores hasta la raíz cuadrada del límite.
    for (unsigned int primo = 2; primo <= limite / primo; ++primo) {
        if (!es_primo[primo]) {
            continue;
        }

        // Los múltiplos anteriores al cuadrado ya se marcaron con primos menores.
        for (unsigned int multiplo = primo * primo; multiplo <= limite; multiplo += primo) {
            es_primo[multiplo] = false;
        }
    }

    std::vector<int> primos;
    for (unsigned int numero = 2; numero <= limite; ++numero) {
        if (es_primo[numero]) {
            primos.push_back(static_cast<int>(numero));
        }
    }

    return primos;
}
