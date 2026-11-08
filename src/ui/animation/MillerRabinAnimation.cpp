#include "../../../include/ui/animation/MillerRabinAnimation.h"
#include "../../../include/ui/animation/StepsAnimation.h"

#include <utility>

/** Crea la animación de los pasos del test de Miller-Rabin. */
ftxui::Component AnimacionMillerRabin(std::shared_ptr<EstadoAnimacionMillerRabin> estado) {
    return AnimacionPasos(std::move(estado));
}
