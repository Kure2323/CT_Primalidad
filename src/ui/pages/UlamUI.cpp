#include "../../../include/ui/pages/UlamUI.h"
#include "../../../include/algorithms/Eratosthenes.h"
#include "../../../include/algorithms/Ulam.h"

#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/ui/common/ResultView.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/utils/InputUtils.h"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/terminal.hpp"

using namespace ftxui;

namespace {
const std::string titulo_espiral = "Espiral de Ulam";
const std::string descripcion_espiral = "Explora la espiral completa por zonas, sin omitir números.";
const std::string titulo_explicacion_espiral = "Fundamento teórico";
const std::vector<std::string> explicacion_espiral = {
    "Se escribe el 1 en el centro y se colocan los números siguientes formando una espiral cuadrada.",
    "Los primos en cualquiera de las dos diagonales principales aparecen en cian; los demás primos aparecen en verde.",
    "La espiral permite observar patrones, pero que un número esté en una diagonal no garantiza que sea primo.",
    "El recuento contempla ambas diagonales geométricas; el centro contiene el 1 y no se cuenta como primo.",
    "Si la espiral no cabe, cambia de zona con las flechas o PageUp y PageDown."
};

struct EstadoUlam {
    std::string limite = "200";
    std::string resultado = "Introduce un límite y pulsa Calcular.";
    std::vector<PuntoUlam> puntos;
    std::vector<int> punto_por_celda;
    std::vector<bool> es_primo;
    unsigned int lado = 1;
    unsigned int primos = 0;
    unsigned int compuestos = 0;
    unsigned int primos_diagonales = 0;
    unsigned int pagina = 0;
    bool mostrar_numeros = false;
    bool calculado = false;
    TipoResultado tipo_resultado = TipoResultado::Informacion;
    std::shared_ptr<int> seccion_actual = std::make_shared<int>(0);
};

unsigned int CeldasPorPagina(const EstadoUlam& estado) {
    const auto terminal = Terminal::Size();
    const int ancho_celda = estado.mostrar_numeros ? 5 : 2;
    // Se limita la cuadrícula por el espacio disponible en ambos ejes para evitar páginas recortadas.
    const int ancho_disponible = std::max(1, (terminal.dimx - 6) / ancho_celda);
    const int alto_disponible = std::max(1, terminal.dimy - 16);
    return static_cast<unsigned int>(std::max(1, std::min(ancho_disponible, alto_disponible)));
}

struct DisenoPagina {
    unsigned int celdas;
    unsigned int paginas_por_eje;
    unsigned int paginas_totales;
    unsigned int x;
    unsigned int y;
};

DisenoPagina ObtenerDisenoPagina(const EstadoUlam& estado) {
    const unsigned int celdas = CeldasPorPagina(estado);
    const unsigned int paginas = (estado.lado + celdas - 1) / celdas;
    // El índice lineal de página se convierte en coordenadas dentro de la cuadrícula de páginas.
    return {celdas, paginas, paginas * paginas, estado.pagina % paginas, estado.pagina / paginas};
}

class CuadriculaUlam : public ComponentBase {
public:
    explicit CuadriculaUlam(std::shared_ptr<EstadoUlam> estado) : estado_(std::move(estado)) {}

    Element OnRender() override {
        if (!estado_->calculado) {
            return vbox({text("Espiral de Ulam") | bold,
                         text("Pulsa Calcular para generar la espiral.") | color(Color::GrayDark)});
        }

        const unsigned int celdas = CeldasPorPagina(*estado_);
        const unsigned int paginas_por_eje = (estado_->lado + celdas - 1) / celdas;
        const unsigned int paginas_totales = paginas_por_eje * paginas_por_eje;
        // Al cambiar el tamaño del terminal, una página que antes existía puede quedar fuera de rango.
        estado_->pagina = std::min(estado_->pagina, paginas_totales - 1);
        const auto pagina = ObtenerDisenoPagina(*estado_);
        const unsigned int primer_x = pagina.x * pagina.celdas;
        const unsigned int primer_y = pagina.y * pagina.celdas;
        const unsigned int ultimo_x = std::min(estado_->lado, primer_x + pagina.celdas);
        const unsigned int ultimo_y = std::min(estado_->lado, primer_y + pagina.celdas);

        Elements filas;
        for (unsigned int y = primer_y; y < ultimo_y; ++y) {
            Elements celdas_fila;
            for (unsigned int x = primer_x; x < ultimo_x; ++x) {
                const int indice_punto = estado_->punto_por_celda[y * estado_->lado + x];
                if (indice_punto < 0) {
                    // El cuadrado que contiene la espiral tiene huecos en las esquinas.
                    celdas_fila.push_back(text(estado_->mostrar_numeros ? "     " : "  "));
                    continue;
                }
                const unsigned int numero = estado_->puntos[indice_punto].numero;
                const bool es_primo = numero > 1 && estado_->es_primo[numero];
                const auto& punto = estado_->puntos[indice_punto];
                // Las diagonales geométricas de la espiral cumplen x = y o x = -y.
                const bool esta_en_diagonal = punto.x == punto.y || punto.x == -punto.y;
                if (!estado_->mostrar_numeros && numero != 1 && !es_primo) {
                    // En la vista clásica se ocultan los compuestos para destacar la distribución de los primos.
                    celdas_fila.push_back(text("  "));
                    continue;
                }
                const Color color_numero = numero == 1 ? Color::Red :
                    es_primo ? (esta_en_diagonal ? Color::Cyan : Color::Green) : Color::GrayDark;
                if (estado_->mostrar_numeros) {
                    const std::string texto_numero = std::to_string(numero);
                    celdas_fila.push_back(text(std::string(5 - texto_numero.size(), ' ') + texto_numero) |
                                     color(color_numero) | ((es_primo || numero == 1) ? bold : dim));
                } else {
                    celdas_fila.push_back(text("██") | color(color_numero) | bold);
                }
            }
            filas.push_back(hbox(std::move(celdas_fila)));
        }

        const std::string modo = estado_->mostrar_numeros ? "Vista numerada" : "Vista clásica: primos resaltados";
        return vbox({
            text("Espiral de Ulam hasta " + std::to_string(estado_->puntos.size()) + " · " + modo) | bold,
            paginas_totales > 1
                ? text("Zona " + std::to_string(estado_->pagina + 1) + "/" + std::to_string(paginas_totales) +
                       " — usa las flechas para recorrerla") | color(Color::GrayLight)
                : text("Espiral completa") | color(Color::GrayLight),
            vbox(std::move(filas)) | border | center,
            hbox({text("Primos fuera de diagonales") | color(Color::Green) | bold,
                  text("   Primos en diagonales") | color(Color::Cyan) | bold,
                  text(estado_->mostrar_numeros ? "   Compuestos" : "   Otros números sin marcar") | color(Color::GrayLight),
                  text("   1") | color(Color::Red) | bold})
        });
    }

    bool OnEvent(Event event) override {
        const auto pagina = ObtenerDisenoPagina(*estado_);
        if (event == Event::ArrowLeft && pagina.x > 0) {
            estado_->pagina = pagina.y * pagina.paginas_por_eje + pagina.x - 1;
            return true;
        }
        if (event == Event::ArrowRight && pagina.x + 1 < pagina.paginas_por_eje) {
            estado_->pagina = pagina.y * pagina.paginas_por_eje + pagina.x + 1;
            return true;
        }
        if (event == Event::ArrowUp && pagina.y > 0) {
            estado_->pagina = (pagina.y - 1) * pagina.paginas_por_eje + pagina.x;
            return true;
        }
        if (event == Event::ArrowDown && pagina.y + 1 < pagina.paginas_por_eje) {
            estado_->pagina = (pagina.y + 1) * pagina.paginas_por_eje + pagina.x;
            return true;
        }
        if (event == Event::PageUp && estado_->pagina > 0) {
            --estado_->pagina;
            return true;
        }
        if (event == Event::PageDown && estado_->pagina + 1 < pagina.paginas_totales) {
            ++estado_->pagina;
            return true;
        }
        return false;
    }

    bool Focusable() const override { return estado_->calculado; }

private:
    std::shared_ptr<EstadoUlam> estado_;
};
}

Component CrearPantallaUlam() {
    auto estado = std::make_shared<EstadoUlam>();
    auto entrada_limite = Input(&estado->limite, "Límite");
    auto mostrar_numeros = Checkbox("Mostrar todos los números", &estado->mostrar_numeros);

    auto calcular = Button("Calcular", [estado] {
        estado->calculado = false;
        estado->puntos.clear();
        estado->punto_por_celda.clear();
        estado->es_primo.clear();
        unsigned int limite;
        if (!ConvertirAEnteroSinSigno(estado->limite, limite) || limite == 0) {
            estado->resultado = "Introduce un entero mayor que 0.";
            estado->tipo_resultado = TipoResultado::Error;
            return;
        }
        try {
            estado->puntos = ConstruirEspiralUlam(limite);
            int coordenada_maxima = 0;
            for (const auto& punto : estado->puntos) {
                coordenada_maxima = std::max({coordenada_maxima, std::abs(punto.x), std::abs(punto.y)});
            }
            estado->lado = static_cast<unsigned int>(2 * coordenada_maxima + 1);
            estado->punto_por_celda.assign(estado->lado * estado->lado, -1);
            const int centro = static_cast<int>(estado->lado / 2);
            for (std::size_t indice = 0; indice < estado->puntos.size(); ++indice) {
                const auto& punto = estado->puntos[indice];
                const unsigned int x = static_cast<unsigned int>(punto.x + centro);
                const unsigned int y = static_cast<unsigned int>(centro - punto.y);
                // Se invierte el eje vertical para que las coordenadas cartesianas coincidan con las filas del terminal.
                estado->punto_por_celda[y * estado->lado + x] = static_cast<int>(indice);
            }
            estado->es_primo.assign(limite + 1, false);
            // La criba calcula todas las marcas de primos en una sola pasada.
            for (const int primo : CribaEratostenes(limite)) {
                estado->es_primo[static_cast<unsigned int>(primo)] = true;
            }
            estado->primos = 0;
            estado->compuestos = 0;
            estado->primos_diagonales = 0;
            for (unsigned int numero = 2; numero <= limite; ++numero) {
                if (estado->es_primo[numero]) {
                    ++estado->primos;
                    // La espiral almacena el punto del número n en el índice n - 1.
                    const auto& punto = estado->puntos[numero - 1];
                    if (punto.x == punto.y || punto.x == -punto.y) ++estado->primos_diagonales;
                } else ++estado->compuestos;
            }
            estado->pagina = 0;
            estado->calculado = true;
            estado->resultado = "Hasta " + std::to_string(limite) + ": " +
                std::to_string(estado->primos) + " primos y " +
                std::to_string(estado->compuestos) + " compuestos; " +
                std::to_string(estado->primos_diagonales) + " primos en las diagonales.";
            estado->tipo_resultado = TipoResultado::Exito;
            *estado->seccion_actual = 1;
        } catch (const std::exception& error) {
            estado->resultado = error.what();
            estado->tipo_resultado = TipoResultado::Error;
        }
    });

    auto entradas = SeccionEntrada("Datos de entrada", {entrada_limite}, calcular);
    auto cuadricula = Make<CuadriculaUlam>(estado);
    auto controles_desarrollo = Container::Vertical({
        mostrar_numeros, cuadricula
    });
    auto desarrollo = Renderer(controles_desarrollo, [mostrar_numeros, cuadricula] {
        return vbox({
            mostrar_numeros->Render(),
            cuadricula->Render()
        });
    });

    return DisenoPantalla(
        titulo_espiral,
        descripcion_espiral,
        entradas,
        VistaExplicacion(titulo_explicacion_espiral, explicacion_espiral),
        desarrollo,
        Renderer([estado] { return VistaResultado(estado->resultado, estado->tipo_resultado); }),
        estado->seccion_actual
    );
}
