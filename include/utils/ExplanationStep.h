#ifndef PRIMOS_EXPLANATION_STEP_H
#define PRIMOS_EXPLANATION_STEP_H

#include <string>

/** Clasificación visual de cada línea explicativa del algoritmo. */
enum class TipoPaso {
    Texto,
    Encabezado,
    Iteracion,
    Calculo
};

/** Texto y función semántica que la vista utiliza para presentarlo. */
struct PasoExplicativo {
    std::string texto;
    TipoPaso tipo = TipoPaso::Texto;
};

#endif
