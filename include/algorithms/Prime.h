#ifndef PRIMOS_PRIME_H
#define PRIMOS_PRIME_H

#include <cstddef>
#include <vector>

/** Resultados de comparar la cantidad de primos con su aproximación. */
struct PuntoComparacionPrimos {
    unsigned int x;
    std::size_t cantidad_primos;
    double aproximacion;
};

struct ResultadoPrimos {
    unsigned int limite;
    std::size_t cantidad_primos;
    double aproximacion;
    double diferencia;
    std::vector<PuntoComparacionPrimos> puntos_grafico;
};

/**
 * Cuenta los primos hasta el límite y los compara con límite / ln(límite).
 * @param limite Extremo superior del recuento.
 * @return Límite, cantidad exacta, aproximación y diferencia absoluta.
 * @throws std::invalid_argument si el límite queda fuera del rango permitido.
 */
ResultadoPrimos CalcularPrimos(unsigned int limite);


#endif //PRIMOS_PRIME_H
