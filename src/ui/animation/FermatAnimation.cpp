#include "../../../include/ui/animation/FermatAnimation.h"
#include "../../../include/ui/animation/StepsAnimation.h"

#include <utility>

/** Crea la animación de los pasos del test de Fermat. */
ftxui::Component AnimacionFermat(std::shared_ptr<EstadoAnimacionFermat> estado) {
    return AnimacionPasos(std::move(estado));
}
