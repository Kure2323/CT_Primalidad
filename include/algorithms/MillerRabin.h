#ifndef PRIMOS_MILLERRABIN_H
#define PRIMOS_MILLERRABIN_H
#include <vector>
#include "../utils/ExplanationStep.h"


/** Resultado de una ronda del test de Miller-Rabin. */
enum class ResultadoMillerRabin {
    Compuesto,
    SuperaRonda,
};

/**
 * Ejecuta una ronda de Miller-Rabin.
 * Una ronda fallida demuestra que n es compuesto; superarla no demuestra que sea primo.
 * Para n = 2 o 3 devuelve SuperaRonda sin usar la base.
 * @param n Número que se analiza.
 * @param base Base utilizada en la ronda.
 * @return `SuperaRonda` si pasa la ronda o `Compuesto` si demuestra que n es compuesto.
 */
ResultadoMillerRabin MillerRabin(unsigned int n, unsigned int base);
/**
 * Ejecuta una ronda de Miller-Rabin y añade la explicación a pasos.
 * @param n Número que se analiza.
 * @param base Base utilizada en la ronda.
 * @param pasos Vector donde se guarda la explicación del cálculo.
 * @return `SuperaRonda` si pasa la ronda o `Compuesto` si demuestra que n es compuesto.
 */
ResultadoMillerRabin MillerRabin(
    unsigned int n,
    unsigned int base,
    std::vector<PasoExplicativo>& pasos
);

#endif //PRIMOS_MILLERRABIN_H
