#ifndef PRIMOS_STEPSVIEW_H
#define PRIMOS_STEPSVIEW_H

#include <span>
#include <vector>

#include "../../utils/ExplanationStep.h"
#include "ftxui/dom/elements.hpp"

/**
 * Construye la vista de los pasos visibles de una explicación.
 * @param pasos Pasos tipados que se deben organizar y representar.
 * @return Elemento FTXUI con los pasos agrupados y formateados.
 */
ftxui::Element VistaPasos(std::span<const PasoExplicativo> pasos);

/** Adapta un vector de pasos a la vista sin crear una copia. */
inline ftxui::Element VistaPasos(const std::vector<PasoExplicativo>& pasos) {
    return VistaPasos(std::span<const PasoExplicativo>(pasos));
}

#endif // PRIMOS_STEPSVIEW_H