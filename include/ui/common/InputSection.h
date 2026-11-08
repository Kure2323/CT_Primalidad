#ifndef PRIMOS_INPUTSECTION_H
#define PRIMOS_INPUTSECTION_H

#include <string>
#include <vector>

#include "ftxui/component/component.hpp"

/**
 * Construye una sección de entrada con sus campos y acciones.
 * El orden de navegación por teclado coincide con el orden de los campos,
 * seguido del botón principal y, si existe, la acción auxiliar.
 * @param titulo Título mostrado sobre los campos.
 * @param campos Componentes de entrada en orden de navegación.
 * @param boton Acción principal de la sección.
 * @param accion_auxiliar Acción opcional mostrada junto al botón principal.
 * @return Componente FTXUI con disposición vertical y orden de foco definido.
 */
ftxui::Component SeccionEntrada(
    const std::string& titulo,
    const std::vector<ftxui::Component>& campos,
    ftxui::Component boton,
    ftxui::Component accion_auxiliar = nullptr
);

#endif //PRIMOS_INPUTSECTION_H
