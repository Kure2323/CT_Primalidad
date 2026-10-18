//
// Created by polete on 10/15/26.
//

#include "Fermat.h"
#include <cstdlib>
#include <numeric>

/**
 * Dado que realizar operaciones con exponentes tan altos es inviable de computar, pues hace overwflow
 * inclusive a unsigned long long, he usado una solución que me ha permitido obtener el módulo n del número
 * exponenciado n-1.
 * Para ello he usado lo que se conoce como Exponenciación Modular el cual es una aplicación del
 * algoritmo extendido de Euclides.
 * @param n Valor a calcular
 * @param base Base sobre donde se calcula. 0 = random. Se modifica su valor en ejecución.
 * @return true: posible primo, false: compuesto
 */
bool Fermat(const unsigned int n, unsigned int& base) {

    if (n < 2) return false;
    if (n == 2) return true;

    if (base == 0 || base >= n) {
        base = rand() % (n-2) + 2; // Random entre 2 y n-1
    }

    // Euclides/Exponenciación Modular
    unsigned int rest = 1;
    for (int i = 1; i <= n-1; i++) {
        rest = (static_cast<unsigned long long>(base)*rest) % n;
    }

    return rest == 1;
}

/**
 * Tontería para ver con qué frecuencia da resultado primo en n.
 * No es más que Fermat en las bases [2, n-1]
 * @param n
 * @return primos según Fermat/n-2
 */
double prime_ratio(const unsigned int n) {
    if (n < 3) return 0.0;

    unsigned int count = 0;
    for (unsigned int i = 2; i < n; i++) {
        count += Fermat(n,i) ? 1 : 0;
    }
    return static_cast<double>(count) / (n-2);
}

bool is_carmichael(const unsigned int n) {
    if (n < 561) return false;

    for (unsigned int i = 2; i < n; i++) {
        if (std::gcd(i, n) == 1) {
            if (!Fermat(n, i)) {
                return false;
            }
        }
    }
    return true;
}
