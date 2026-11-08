#include "../../../include/ui/common/RandomBaseControl.h"

#include "ftxui/component/component.hpp"

using namespace ftxui;

/** Crea la casilla para elegir una base aleatoria. */
Component ControlBaseAleatoria(bool* aleatoria) {
    return Checkbox("Elegir base aleatoriamente", aleatoria);
}
