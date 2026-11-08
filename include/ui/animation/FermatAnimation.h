#ifndef PRIMOS_FERMATANIMATION_H
#define PRIMOS_FERMATANIMATION_H

#include <memory>
#include "StepsAnimation.h"

/** Estado de los pasos explicativos mostrados por la animación de Fermat. */
using EstadoAnimacionFermat = EstadoAnimacionPasos;

/**
 * Crea la animación progresiva de los pasos del test de Fermat.
 * @param estado Estado compartido que contiene los pasos y el progreso visible.
 * @return Componente FTXUI que presenta los pasos del test.
 */
ftxui::Component AnimacionFermat(std::shared_ptr<EstadoAnimacionFermat> estado);

#endif
