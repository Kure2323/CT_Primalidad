#include "../../../include/ui/common/ExplanationView.h"

#include <algorithm>
#include <sstream>
#include <utility>

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/screen/terminal.hpp"

using namespace ftxui;

namespace {
using Pagina = std::vector<std::string>;

/** Ajusta un párrafo a líneas que no superen el ancho indicado. */
std::vector<std::string> AjustarLineas(const std::string& parrafo, std::size_t ancho) {
    std::istringstream entrada(parrafo);
    std::vector<std::string> lineas;
    std::string palabra;
    std::string linea;
    std::size_t ancho_linea = 0;
    while (entrada >> palabra) {
        const auto ancho_palabra = static_cast<std::size_t>(string_width(palabra));
        if (!linea.empty() && ancho_linea + 1 + ancho_palabra > ancho) {
            lineas.push_back(std::move(linea));
            linea.clear();
            ancho_linea = 0;
        }
        if (!linea.empty()) {
            linea += ' ';
            ++ancho_linea;
        }
        linea += palabra;
        ancho_linea += ancho_palabra;
    }
    if (!linea.empty()) lineas.push_back(std::move(linea));
    if (lineas.empty()) lineas.emplace_back();
    return lineas;
}

/** Distribuye los párrafos entre páginas según el tamaño actual del terminal. */
std::vector<Pagina> Paginar(const std::vector<std::string>& parrafos, int ancho, int alto) {
    const auto ancho_linea = static_cast<std::size_t>(std::max(12, ancho - 3));
    const auto lineas_por_pagina = static_cast<std::size_t>(std::max(3, alto - 13));
    std::vector<Pagina> paginas(1);

    const auto agregar_linea = [&paginas, lineas_por_pagina](std::string linea) {
        if (paginas.back().size() == lineas_por_pagina) paginas.emplace_back();
        paginas.back().push_back(std::move(linea));
    };

    for (std::size_t indice = 0; indice < parrafos.size(); ++indice) {
        if (indice > 0) agregar_linea({});
        for (auto& linea : AjustarLineas(parrafos[indice], ancho_linea))
            agregar_linea(std::move(linea));
    }
    return paginas;
}

/** Permite consultar la explicación distribuida por páginas. */
class ComponenteExplicacion : public ComponentBase {
public:
    ComponenteExplicacion(std::string titulo, std::vector<std::string> parrafos)
        : titulo_(std::move(titulo)), parrafos_(std::move(parrafos)) {}

    /** Dibuja la página actual y sus indicaciones de navegación. */
    Element OnRender() override {
        const auto& paginas = PaginasActuales();
        pagina_actual_ = std::min(pagina_actual_, paginas.size() - 1);
        Elements lineas;
        for (const auto& linea : paginas[pagina_actual_]) lineas.push_back(text(linea));
        Elements contenido = {text(titulo_) | bold, separator()};
        if (paginas.size() > 1)
            contenido.push_back(text("Página " + std::to_string(pagina_actual_ + 1) + "/" +
                                   std::to_string(paginas.size()) + " — Re Pág / Av Pág"));
        contenido.push_back(vbox(std::move(lineas)) | flex);
        return vbox(std::move(contenido));
    }

    /** Cambia de página cuando se pulsan Av Pág o Re Pág. */
    bool OnEvent(Event event) override {
        const auto cantidad_paginas = PaginasActuales().size();
        if (event == Event::PageDown && pagina_actual_ + 1 < cantidad_paginas) { ++pagina_actual_; return true; }
        if (event == Event::PageUp && pagina_actual_ > 0) { --pagina_actual_; return true; }
        return false;
    }

private:
    /** Reutiliza la paginación mientras no cambien las dimensiones del terminal. */
    const std::vector<Pagina>& PaginasActuales() {
        const auto tamano_terminal = Terminal::Size();
        if (tamano_terminal.dimx != ancho_cacheado_ || tamano_terminal.dimy != alto_cacheado_) {
            // Una redimensión cambia el ancho de línea y cuántas caben en cada página.
            paginas_cacheadas_ = Paginar(parrafos_, tamano_terminal.dimx - 4, tamano_terminal.dimy);
            ancho_cacheado_ = tamano_terminal.dimx;
            alto_cacheado_ = tamano_terminal.dimy;
        }
        return paginas_cacheadas_;
    }
    std::string titulo_;
    std::vector<std::string> parrafos_;
    std::size_t pagina_actual_ = 0;
    int ancho_cacheado_ = -1;
    int alto_cacheado_ = -1;
    std::vector<Pagina> paginas_cacheadas_;
};
}

/** Crea una vista paginada de la explicación teórica. */
Component VistaExplicacion(const std::string& titulo, const std::vector<std::string>& parrafos) {
    return Make<ComponenteExplicacion>(titulo, parrafos);
}
