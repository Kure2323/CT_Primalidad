#ifndef PRIMOS_CARMICHAELUI_H
#define PRIMOS_CARMICHAELUI_H

#include "ftxui/component/component.hpp"

/**
 * Construye la pantalla para analizar las bases coprimas de un número
 * y comprobar si supera el test de Fermat para todas ellas.
 * @return Componente FTXUI con la entrada, el análisis progresivo y el gráfico.
 */
ftxui::Component CrearPantallaCarmichael();

#endif
