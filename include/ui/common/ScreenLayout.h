#ifndef PRIMOS_SCREENLAYOUT_H
#define PRIMOS_SCREENLAYOUT_H

#include <string>
#include <memory>
#include "ftxui/component/component.hpp"

/**
 * Construye una pantalla uniforme con navegación entre entrada y desarrollo.
 * La sección de entrada contiene los campos, el resultado y la explicación;
 * la sección de desarrollo presenta la visualización y el resultado.
 * @param titulo Título principal de la pantalla.
 * @param descripcion Resumen del algoritmo o herramienta.
 * @param entradas Componente con los campos y acciones de entrada.
 * @param explicacion Componente con el fundamento teórico.
 * @param desarrollo Componente de visualización o animación.
 * @param resultado Componente que muestra el resultado actual.
 * @param seccion_actual Índice compartido de sección seleccionada; si es nulo,
 *        la pantalla crea y administra uno propio.
 * @return Componente FTXUI con la cabecera y la navegación de secciones.
 */
ftxui::Component DisenoPantalla(
    const std::string& titulo,
    const std::string& descripcion,
    ftxui::Component entradas,
    ftxui::Component explicacion,
    ftxui::Component desarrollo,
    ftxui::Component resultado,
    std::shared_ptr<int> seccion_actual = nullptr
);


#endif //PRIMOS_SCREENLAYOUT_H
