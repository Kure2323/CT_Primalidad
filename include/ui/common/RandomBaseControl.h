#ifndef PRIMOS_RANDOMBASECONTROL_H
#define PRIMOS_RANDOMBASECONTROL_H

#include "ftxui/component/component.hpp"

/**
 * Crea la casilla para elegir una base aleatoria.
 * @param aleatoria Puntero al estado que actualiza la casilla.
 * @return Componente FTXUI enlazado al valor indicado.
 */
ftxui::Component ControlBaseAleatoria(bool* aleatoria);

#endif
