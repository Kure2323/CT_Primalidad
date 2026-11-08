#ifndef PRIMOS_ERATOSTENES_H
#define PRIMOS_ERATOSTENES_H

#include <vector>

/**
 * Encuentra todos los números primos menores o iguales que el límite.
 *
 * @param limite Límite superior de la búsqueda.
 * @return Lista ordenada de primos entre 2 y limite; vacía si el límite es menor que 2.
 * @throws std::invalid_argument si el límite supera el máximo permitido.
 */
std::vector<int> CribaEratostenes(unsigned int limite);

#endif //PRIMOS_ERATOSTENES_H
