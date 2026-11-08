#include "../../../include/ui/animation/StepsView.h"

#include <algorithm>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/screen/terminal.hpp"

using namespace ftxui;
using GrupoPasos = std::vector<const PasoExplicativo*>;

/** Representa un paso con el estilo asociado a su tipo. */
static Element RepresentarPaso(const PasoExplicativo& paso) {
    if (paso.tipo == TipoPaso::Encabezado || paso.tipo == TipoPaso::Iteracion) {
        const auto color_encabezado =
            paso.tipo == TipoPaso::Iteracion ? Color::Cyan : Color::Blue;
        return vbox({separator(), text(paso.texto) | bold | color(color_encabezado)});
    }
    if (paso.tipo == TipoPaso::Calculo)
        return paragraph("  " + paso.texto) | color(Color::White);
    return paragraph(paso.texto) | color(Color::GrayLight);
}

/** Construye los elementos visuales de una secuencia de pasos. */
static Elements RepresentarPasos(const GrupoPasos& pasos) {
    Elements representados;
    for (const auto* paso : pasos) representados.push_back(RepresentarPaso(*paso));
    return representados;
}

/** Estima cuántas filas ocupa un paso al ajustarse al ancho indicado. */
static std::size_t EstimarFilas(const PasoExplicativo& paso, std::size_t ancho) {
    const auto ancho_texto = static_cast<std::size_t>(string_width(paso.texto));
    const auto filas_texto = std::max<std::size_t>(1, (ancho_texto + ancho - 1) / ancho);
    const bool encabezado = paso.tipo == TipoPaso::Encabezado || paso.tipo == TipoPaso::Iteracion;
    return filas_texto + (encabezado ? 1 : 0);
}

/** Estima la altura total de una iteración. */
static std::size_t EstimarAltura(const GrupoPasos& pasos, std::size_t ancho) {
    std::size_t altura = 1;
    for (const auto* paso : pasos) altura += EstimarFilas(*paso, ancho);
    return altura;
}

/** Separa la introducción y agrupa cada iteración sin copiar sus pasos. */
static std::vector<GrupoPasos> SepararIteraciones(
    std::span<const PasoExplicativo> pasos, GrupoPasos& introduccion) {
    std::vector<GrupoPasos> iteraciones;
    for (const auto& paso : pasos) {
        if (paso.tipo == TipoPaso::Iteracion) iteraciones.emplace_back();
        (iteraciones.empty() ? introduccion : iteraciones.back()).push_back(&paso);
    }
    return iteraciones;
}

/** Distribuye iteraciones completas entre columnas según su altura estimada. */
static std::vector<std::vector<GrupoPasos>> CrearColumnas(
    const std::vector<GrupoPasos>& iteraciones, std::size_t ancho, std::size_t altura) {
    std::vector<std::vector<GrupoPasos>> columnas(1);
    std::size_t filas_usadas = 0;
    for (const auto& iteracion : iteraciones) {
        const auto filas = EstimarAltura(iteracion, ancho);
        if (!columnas.back().empty() && filas_usadas + filas > altura) {
            columnas.emplace_back();
            filas_usadas = 0;
        }
        columnas.back().push_back(iteracion);
        filas_usadas += filas;
    }
    return columnas;
}

/** Construye la representación horizontal de las columnas. */
static Element RepresentarColumnas(const std::vector<std::vector<GrupoPasos>>& columnas) {
    Elements representadas;
    for (const auto& columna : columnas) {
        if (!representadas.empty()) representadas.push_back(separator());
        Elements grupos;
        for (const auto& iteracion : columna)
            grupos.push_back(vbox(RepresentarPasos(iteracion)));
        representadas.push_back(vbox(std::move(grupos)) | flex);
    }
    return hbox(std::move(representadas));
}

/** Distribuye los pasos tipados en columnas y aplica su estilo semántico. */
ftxui::Element VistaPasos(std::span<const PasoExplicativo> pasos) {
    GrupoPasos introduccion;
    const auto iteraciones = SepararIteraciones(pasos, introduccion);
    if (iteraciones.empty()) {
        if (pasos.empty()) {
            return vbox({text("Desarrollo paso a paso:") | bold, separator(),
                         text("Todavía no hay pasos que mostrar.")});
        }
        return vbox({text("Desarrollo paso a paso:") | bold, separator(),
                     vbox(RepresentarPasos(introduccion))});
    }

    const auto terminal = Terminal::Size();
    const auto ancho_columna = static_cast<std::size_t>(std::max(24, terminal.dimx / 2 - 3));
    const auto ancho_completo = static_cast<std::size_t>(std::max(30, terminal.dimx - 8));
    std::size_t altura_introduccion = 0;
    for (const auto* paso : introduccion)
        altura_introduccion += EstimarFilas(*paso, ancho_completo);
    const auto altura_disponible = static_cast<std::size_t>(
        std::max(4, terminal.dimy - 14 - static_cast<int>(altura_introduccion)));
    // Las iteraciones se mantienen completas; solo se inicia otra columna si no caben.
    const auto columnas = CrearColumnas(iteraciones, ancho_columna, altura_disponible);

    return vbox({
        text("Desarrollo paso a paso:") | bold,
        separator(),
        vbox(RepresentarPasos(introduccion)),
        RepresentarColumnas(columnas)
    });
}
