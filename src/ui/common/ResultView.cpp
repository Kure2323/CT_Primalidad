#include "../../../include/ui/common/ResultView.h"

#include "ftxui/dom/elements.hpp"

using namespace ftxui;

/** Asocia un color al tipo de resultado y construye su presentación. */
Element VistaResultado(const std::string& mensaje, TipoResultado tipo) {

    Color color_resultado;

    switch (tipo) {
        case TipoResultado::Informacion:
            color_resultado = Color::GrayDark;
            break;

        case TipoResultado::Exito:
            color_resultado = Color::Green;
            break;

        case TipoResultado::Error:
            color_resultado = Color::Red;
            break;
    }
    return vbox({
        text("Resultado") | bold,
        paragraph(mensaje) | color(color_resultado)
    });
}