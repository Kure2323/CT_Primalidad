#include "../../../include/ui/pages/PrimesUI.h"
#include "../../../include/algorithms/Prime.h"

#include "../../../include/ui/common/ResultView.h"
#include "../../../include/ui/animation/StepsView.h"
#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/utils/InputUtils.h"
#include "../../../include/utils/ExplanationStep.h"

#include <exception>
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/terminal.hpp"

using namespace ftxui;


struct EstadoPrimos {
    std::string limite = "100";
    std::string resultado = "Introduce un límite y pulsa Calcular.";
    std::vector<PasoExplicativo> pasos;
    std::vector<PuntoComparacionPrimos> puntos_grafico;
    TipoResultado tipo_resultado = TipoResultado::Informacion;
    std::shared_ptr<int> seccion_actual = std::make_shared<int>(0);
};

static Element GraficoComparacion(const std::vector<PuntoComparacionPrimos>& puntos) {
    if (puntos.empty())
        return text("Calcula un límite para ver la comparación gráfica.") | color(Color::GrayLight);

    const auto terminal = Terminal::Size();
    const int ancho = std::clamp((terminal.dimx / 2 - 4) * 2, 40, 120);
    const int alto = std::clamp((terminal.dimy - 12) * 4, 32, 56);
    const double valor_maximo = std::max({
        static_cast<double>(puntos.back().cantidad_primos),
        puntos.front().aproximacion,
        puntos.back().aproximacion
    });
    const double escala = valor_maximo > 0 ? valor_maximo : 1.0;
    const unsigned int limite = puntos.back().x;

    auto grafico = canvas(ancho, alto, [&puntos, escala, limite](Canvas& dibujo) {
        const int x_eje = 8;
        const int y_eje = dibujo.height() - 3;
        const int ancho_util = dibujo.width() - x_eje - 2;
        const int alto_util = y_eje - 2;

        for (int marca = 1; marca <= 3; ++marca) {
            const int y = y_eje - alto_util * marca / 4;
            dibujo.DrawPointLine(x_eje, y, dibujo.width() - 2, y, Color::GrayDark);
        }
        dibujo.DrawPointLine(x_eje, 1, x_eje, y_eje, Color::GrayLight);
        dibujo.DrawPointLine(x_eje, y_eje, dibujo.width() - 2, y_eje, Color::GrayLight);

        const auto coordenadas = [=](unsigned int x, double valor) {
            const double proporcion_x = limite == 2 ? 0.0 :
                static_cast<double>(x - 2) / (limite - 2);
            const double proporcion_y = std::clamp(valor / escala, 0.0, 1.0);
            return std::pair{
                x_eje + static_cast<int>(std::round(proporcion_x * ancho_util)),
                y_eje - static_cast<int>(std::round(proporcion_y * alto_util))
            };
        };

        auto anterior_real = coordenadas(puntos.front().x,
                                         static_cast<double>(puntos.front().cantidad_primos));
        auto anterior_aproximacion = coordenadas(puntos.front().x, puntos.front().aproximacion);
        dibujo.DrawPoint(anterior_real.first, anterior_real.second, true, Color::Green);
        dibujo.DrawPoint(anterior_aproximacion.first, anterior_aproximacion.second, true, Color::Yellow);
        for (std::size_t indice = 1; indice < puntos.size(); ++indice) {
            const auto real = coordenadas(puntos[indice].x,
                                          static_cast<double>(puntos[indice].cantidad_primos));
            const auto aproximacion = coordenadas(puntos[indice].x, puntos[indice].aproximacion);
            dibujo.DrawPointLine(anterior_real.first, anterior_real.second,
                                 real.first, real.second, Color::Green);
            if (anterior_aproximacion.first != aproximacion.first) {
                dibujo.DrawPointLine(anterior_aproximacion.first, anterior_aproximacion.second,
                                     aproximacion.first, aproximacion.second, Color::Yellow);
            }
            anterior_real = real;
            anterior_aproximacion = aproximacion;
        }
    });

    return vbox({
        text("π(x) frente a x / ln(x)") | bold,
        text("π(x): 0–" + std::to_string(static_cast<unsigned int>(std::ceil(escala)))) |
            color(Color::GrayLight),
        grafico,
        text("x: 2 → " + std::to_string(limite)) | color(Color::GrayLight),
        text("━ π(x), recuento real") | color(Color::Green),
        text("━ x / ln(x), aproximación") | color(Color::Yellow)
    });
}


Component CrearPantallaProporcionPrimos() {
    auto estado = std::make_shared<EstadoPrimos>();

    auto entrada_limite = Input(&estado->limite, "Límite");

    auto ejecutar = Button("Calcular", [estado] {
        estado->pasos.clear();
        estado->puntos_grafico.clear();
        unsigned int n;
        if (!ConvertirAEnteroSinSigno(estado->limite, n)) {
            estado->resultado = "Introduce un número entero válido.";
            estado->tipo_resultado = TipoResultado::Error;
            return;
        }

        try {
            const ResultadoPrimos datos = CalcularPrimos(n);
            estado->puntos_grafico = datos.puntos_grafico;
            // Los pasos se preparan como datos para que VistaPasos los muestre con su formato común.
            estado->pasos = {
                {"1. Aplicamos la criba de Eratóstenes hasta N = " +
                     std::to_string(datos.limite) + ".", TipoPaso::Texto},
                {"2. Contamos los primos encontrados: " +
                     std::to_string(datos.cantidad_primos) + ".", TipoPaso::Texto},
                {"3. Aproximación N / ln(N): " +
                     std::to_string(datos.aproximacion) + ".", TipoPaso::Texto},
                {"4. Diferencia absoluta: " +
                     std::to_string(datos.diferencia) + ".", TipoPaso::Texto}
            };
            estado->resultado = "Primos reales: " + std::to_string(datos.cantidad_primos)
                + " | Aproximación N / ln(N): " + std::to_string(datos.aproximacion)
                + " | Diferencia: " + std::to_string(datos.diferencia);
            estado->tipo_resultado = TipoResultado::Exito;
            *estado->seccion_actual = 1;
        } catch (const std::exception& e) {
            estado->pasos.clear();
            estado->puntos_grafico.clear();
            estado->resultado = e.what();
            estado->tipo_resultado = TipoResultado::Error;
        }
    });

    auto entradas = SeccionEntrada("Datos de entrada", {entrada_limite}, ejecutar);
    auto desarrollo = Renderer([estado] {
        return hbox({
            GraficoComparacion(estado->puntos_grafico),
            separator(),
            VistaPasos(estado->pasos)
        });
    });

    return DisenoPantalla(
        "Proporción de números primos",
        "Compara el recuento exacto con una aproximación teórica.",
        entradas,
        VistaExplicacion("Fundamento teórico", {
                "La función π(N) cuenta los primos menores o iguales que N. El recuento exacto de esta pantalla se obtiene con la criba de Eratóstenes.",
                "El teorema de los números primos establece que π(N) se aproxima a N / ln(N) cuando N crece.",
                "N / ln(N) es una estimación, no un recuento exacto; su precisión mejora para valores grandes de N."
            }),
        desarrollo,
        Renderer([estado] {
            return VistaResultado(estado->resultado, estado->tipo_resultado);
        }),
        estado->seccion_actual
    );
}
