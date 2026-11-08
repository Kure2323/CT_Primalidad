#include "../../../include/ui/common/InputSection.h"

#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

/** Agrupa campos y acciones con un orden de navegación coherente. */
Component SeccionEntrada(
    const std::string& titulo,
    const std::vector<Component>& campos,
    Component boton,
    Component accion_auxiliar
) {
    // El orden del contenedor define el recorrido del foco con el teclado.
    auto controles = campos;
    controles.push_back(boton);
    if (accion_auxiliar) controles.push_back(accion_auxiliar);
    auto contenedor = Container::Vertical(controles);

    return Renderer(contenedor, [titulo, campos, boton, accion_auxiliar] {
        Elements elementos;
        for (const auto& campo : campos) {
            elementos.push_back(campo->Render());
        }

        Elements acciones;
        acciones.push_back(boton->Render());
        if (accion_auxiliar) acciones.push_back(accion_auxiliar->Render());

        // Las acciones quedan después de los campos y al final del recorrido del foco.
        return vbox({
            text(titulo) | bold,
            separator(),
            vbox(std::move(elementos)),
            vbox(std::move(acciones))
        });
    });
}
