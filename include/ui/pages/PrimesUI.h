#ifndef PRIMOS_PRIMESUI_H
#define PRIMOS_PRIMESUI_H

#include "ftxui/component/component.hpp"

/**
 * Construye la pantalla que compara la cantidad de primos con N / ln(N).
 * @return Componente FTXUI con el límite y el desarrollo del cálculo.
 */
ftxui::Component CrearPantallaProporcionPrimos();

#endif //PRIMOS_PRIMESUI_H
