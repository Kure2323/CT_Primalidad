#ifndef PRIMOS_NUMBERTHEORY_H
#define PRIMOS_NUMBERTHEORY_H

#include <vector>
#include "../utils/ExplanationStep.h"

/** Indica si un número es primo mediante divisiones hasta su raíz cuadrada. */
bool EsPrimo(unsigned long numero);

/**
 * Calcula base elevado al exponente módulo modulo y guarda los pasos.
 * @param base Base de la potencia.
 * @param exponente Exponente no negativo.
 * @param modulo Módulo positivo.
 * @param pasos Recibe la explicación de los cálculos.
 * @return Resto de la potencia modular.
 */
unsigned int PotenciaModular(unsigned int base, unsigned int exponente, unsigned int modulo,
                             std::vector<PasoExplicativo>& pasos);
/**
 * Calcula base elevado al exponente módulo modulo sin generar pasos explicativos.
 * @param base Base de la potencia.
 * @param exponente Exponente no negativo.
 * @param modulo Módulo positivo.
 * @return Resto de la potencia modular.
 */
unsigned int PotenciaModular(unsigned int base, unsigned int exponente, unsigned int modulo);

#endif // PRIMOS_NUMBERTHEORY_H
