#ifndef PRIMOS_RESULTVIEW_H
#define PRIMOS_RESULTVIEW_H
#include "ftxui/ftxui.hpp"


/** Categoría visual de un resultado mostrado al usuario. */
enum class TipoResultado {
    Informacion,
    Exito,
    Error
};

/**
 * Construye la representación visual de un resultado.
 * @param mensaje Texto descriptivo que se presenta.
 * @param tipo Categoría que determina el color del mensaje.
 * @return Elemento FTXUI con título y mensaje coloreado.
 */
ftxui::Element VistaResultado(const std::string& mensaje, TipoResultado tipo);


#endif //PRIMOS_RESULTVIEW_H
