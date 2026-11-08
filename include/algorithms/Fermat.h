#ifndef PRIMOS_FERMAT_H
#define PRIMOS_FERMAT_H

#include <vector>
#include "../utils/ExplanationStep.h"

struct AnalisisBasesFermat {
    unsigned int bases_coprimas = 0;
    unsigned int bases_superadas = 0;
    bool es_carmichael = false;
    bool es_primo = false;
    unsigned int factor_primo = 0;
    unsigned int base_testigo = 0;
    unsigned int resto_testigo = 0;
};

/**
 * Ejecuta una ronda del test de Fermat.
 *
 * @param n Número que se quiere comprobar.
 * @param base Base manual; se reemplaza por una base aleatoria si aleatorio es true.
 * @param pasos Recibe la explicación de los cálculos realizados.
 * @param aleatorio Indica si se debe elegir una base aleatoria.
 * @return true si la ronda se supera; false si demuestra que n es compuesto.
 */
bool Fermat(unsigned int n, unsigned int& base, std::vector<PasoExplicativo>& pasos, bool aleatorio = true);

/** Devuelve la proporción de bases coprimas con n que superan Fermat. */
double proporcion_bases_fermat(unsigned int n);

/** Indica si n es un número de Carmichael. */
bool es_carmichael(unsigned int n);

/** Analiza las bases coprimas y recoge testigos, primalidad y clasificación Carmichael. */
AnalisisBasesFermat AnalizarBasesFermat(unsigned int n);


#endif //PRIMOS_FERMAT_H
