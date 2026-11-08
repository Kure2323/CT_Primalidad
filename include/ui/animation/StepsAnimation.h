#ifndef PRIMOS_STEPS_ANIMATION_H
#define PRIMOS_STEPS_ANIMATION_H

#include <memory>
#include <vector>
#include "../../utils/ExplanationStep.h"

#include "ftxui/component/component.hpp"

/**
 * Estado compartido de la animación de una explicación paso a paso.
 *
 * El algoritmo llena los pasos y la pantalla reinicia el índice visible antes
 * de iniciar una nueva ejecución.
 */
struct EstadoAnimacionPasos {
    std::vector<PasoExplicativo> pasos;  ///< Explicación completa, en orden de ejecución.
    std::size_t pasos_visibles = 0;      ///< Número de pasos que ya se pueden representar.
};

/**
 * Crea el componente que revela los pasos de forma progresiva.
 * @param estado Estado compartido con la explicación y el progreso.
 * @return Componente FTXUI que representa los pasos visibles.
 */
ftxui::Component AnimacionPasos(std::shared_ptr<EstadoAnimacionPasos> estado);

#endif
