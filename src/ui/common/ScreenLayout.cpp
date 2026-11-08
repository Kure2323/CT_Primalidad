#include "../../../include/ui/common/ScreenLayout.h"

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

namespace {
/** Conserva los nombres de sección y la selección compartida del menú. */
struct EstadoDisenoPantalla {
    std::vector<std::string> nombres_secciones = {
        "Datos de entrada", "Desarrollo"
    };
    std::shared_ptr<int> seccion_actual;
};
}

/** Compone la cabecera, el menú y el contenido de cada sección. */
Component DisenoPantalla(
    const std::string& titulo,
    const std::string& descripcion,
    Component entradas,
    Component explicacion,
    Component desarrollo,
    Component resultado,
    std::shared_ptr<int> seccion_actual
) {
    auto estado = std::make_shared<EstadoDisenoPantalla>();
    estado->seccion_actual = seccion_actual ? std::move(seccion_actual) : std::make_shared<int>(0);
    auto menu_secciones = Menu(&estado->nombres_secciones, estado->seccion_actual.get(), MenuOption::HorizontalAnimated());
    // Agrupa los componentes para conservar la navegación por teclado de la sección.
    auto datos_con_fundamento = Container::Vertical({entradas, explicacion});
    auto entradas_con_resultado = Renderer(datos_con_fundamento, [entradas, resultado, explicacion] {
        return vbox({
            entradas->Render(),
            separator(),
            resultado->Render(),
            separator(),
            explicacion->Render()
        });
    });
    auto desarrollo_con_resultado = Renderer(desarrollo, [desarrollo, resultado] {
        return vbox({desarrollo->Render(), separator(), resultado->Render()});
    });
    auto secciones = Container::Tab(
        {entradas_con_resultado, desarrollo_con_resultado},
        estado->seccion_actual.get()
    );
    auto contenido = Container::Vertical({menu_secciones, secciones});

    auto pantalla = Renderer(contenido, [titulo, descripcion, menu_secciones, secciones, estado] {
        auto cabecera = vbox({
            paragraph(titulo) | bold | center,
            paragraph(descripcion),
            separator(),
        });
        return vbox({
            cabecera,
            menu_secciones->Render(),
            text("Usa ← y → para cambiar de sección"),
            separator(),
            secciones->Render() | flex
        })
            | flex | border;
    });

    return pantalla;
}
