#ifndef PRIMOS_EXPLANATIONVIEW_H
#define PRIMOS_EXPLANATIONVIEW_H

#include <string>
#include <vector>

#include "ftxui/component/component.hpp"

/**
 * Crea una vista paginada con la explicación teórica de un algoritmo.
 * @param titulo Título que encabeza la explicación.
 * @param parrafos Párrafos que se ajustan al espacio disponible del terminal.
 * @return Componente FTXUI que permite recorrer los párrafos por páginas.
 */
ftxui::Component VistaExplicacion(
    const std::string& titulo,
    const std::vector<std::string>& parrafos
);

#endif //PRIMOS_EXPLANATIONVIEW_H
