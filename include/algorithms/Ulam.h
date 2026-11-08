#ifndef PRIMOS_ULAM_H
#define PRIMOS_ULAM_H

#include <vector>

/** Número y coordenadas de un punto de la espiral. */
struct PuntoUlam {
    unsigned int numero;
    int x;
    int y;
};

/**
 * Construye la espiral de Ulam hasta el límite indicado.
 * @param limite Mayor número que se incluye en la espiral.
 * @return Puntos ordenados por número, con el 1 situado en el origen.
 * @throws std::invalid_argument si el límite queda fuera del rango permitido.
 */
std::vector<PuntoUlam> ConstruirEspiralUlam(unsigned int limite);

#endif
