#ifndef PRIMOS_MILLERRABINANIMATION_H
#define PRIMOS_MILLERRABINANIMATION_H

#include "StepsAnimation.h"

/** Estado de los pasos explicativos mostrados por Miller-Rabin. */
using EstadoAnimacionMillerRabin = EstadoAnimacionPasos;

/**
 * Crea la animación progresiva de los pasos del test de Miller-Rabin.
 * @param estado Estado compartido que contiene los pasos y el progreso visible.
 * @return Componente FTXUI que presenta los pasos de la ronda.
 */
ftxui::Component AnimacionMillerRabin(std::shared_ptr<EstadoAnimacionMillerRabin> estado);

#endif
