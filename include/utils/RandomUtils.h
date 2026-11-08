#ifndef PRIMOS_RANDOMUTILS_H
#define PRIMOS_RANDOMUTILS_H

/**
 * Genera un entero aleatorio dentro del intervalo inclusivo indicado.
 * @param minimo Extremo inferior del intervalo.
 * @param maximo Extremo superior del intervalo.
 * @return Entero aleatorio entre ambos extremos.
 * @throws std::invalid_argument si minimo es mayor que maximo.
 */
unsigned int EnteroAleatorio(unsigned int minimo, unsigned int maximo);

#endif
