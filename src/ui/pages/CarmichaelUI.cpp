#include "../../../include/ui/pages/CarmichaelUI.h"

#include "../../../include/ui/animation/CarmichaelAnimation.h"
#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/ui/common/ResultView.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/utils/InputUtils.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <memory>
#include <numeric>
#include <numbers>
#include <string>
#include <system_error>

#include "ftxui/component/animation.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

namespace {
// El gráfico siempre tiene este tamaño; precalcular cada ángulo evita repetir atan2 en cada fotograma.
const std::array<double, 48 * 40>& AngulosGrafico() {
    static const std::array<double, 48 * 40> angulos = [] {
        std::array<double, 48 * 40> resultado{};
        constexpr int centro_x = 48 / 2;
        constexpr int centro_y = 40 / 2;
        for (int y = 0; y < 40; ++y) {
            for (int x = 0; x < 48; ++x) {
                const double dx = x - centro_x;
                const double dy = y - centro_y;
                double angulo = std::atan2(-dy, dx);
                if (angulo < 0) angulo += 2.0 * std::numbers::pi;
                resultado[y * 48 + x] = angulo;
            }
        }
        return resultado;
    }();
    return angulos;
}

struct EstadoCarmichael {
    std::string numero = "561";
    std::string resultado = "Introduce un número y pulsa Calcular.";
    TipoResultado tipo_resultado = TipoResultado::Informacion;
    std::shared_ptr<EstadoAnimacionCarmichael> analisis = std::make_shared<EstadoAnimacionCarmichael>();
    std::shared_ptr<int> seccion_actual = std::make_shared<int>(0);
};

void BuscarFactorPrimo(EstadoAnimacionCarmichael& analisis) {
    analisis.factor_primo = 0;
    // La condición equivalente factor <= n / factor evita desbordar al calcular factor * factor.
    for (unsigned int factor = 2; factor <= analisis.numero / factor; ++factor) {
        if (analisis.numero % factor == 0) {
            analisis.factor_primo = factor;
            break;
        }
    }
    analisis.es_primo = analisis.factor_primo == 0;
}

bool EsNumeroCarmichael(const EstadoAnimacionCarmichael& analisis) {
    // 561 es el menor Carmichael; además, debe ser compuesto y superar Fermat con todas las bases coprimas.
    return analisis.numero >= 561 && !analisis.es_primo &&
           analisis.bases_totales > 0 && analisis.bases_fallidas == 0;
}

std::string FormatearPorcentaje(double valor) {
    char buffer[32];
    const auto resultado = std::to_chars(buffer, buffer + sizeof(buffer), valor, std::chars_format::fixed, 1);
    return resultado.ec == std::errc{} ? std::string(buffer, resultado.ptr) : std::to_string(valor);
}

std::string ExplicacionFinal(const EstadoAnimacionCarmichael& analisis) {
    const double porcentaje = analisis.bases_totales == 0 ? 0.0 :
        100.0 * analisis.bases_superadas / analisis.bases_totales;
    const std::string proporcion = FormatearPorcentaje(porcentaje) + "%).";
    if (analisis.es_primo) {
        std::string mensaje = std::to_string(analisis.numero) +
            " es primo, no de Carmichael: no es compuesto y no tiene divisores primos hasta su raíz cuadrada. ";
        if (analisis.bases_totales == 0) return mensaje + "No hay bases en el rango 2..n-1 que probar.";
        return mensaje + "Por el pequeño teorema de Fermat, pasan " +
            std::to_string(analisis.bases_superadas) + " de " +
            std::to_string(analisis.bases_totales) + " bases coprimas (" + proporcion;
    }

    const std::string numero = std::to_string(analisis.numero);
    const std::string factor = std::to_string(analisis.factor_primo);
    const std::string otro_factor = std::to_string(analisis.numero / analisis.factor_primo);
    if (EsNumeroCarmichael(analisis)) {
        return numero + " es compuesto (" + numero + " = " + factor + " × " + otro_factor +
            ") y Fermat pasa para las " + std::to_string(analisis.bases_superadas) + " de " +
            std::to_string(analisis.bases_totales) + " bases coprimas (" + proporcion.substr(0, proporcion.size() - 1) +
            " Por definición, es de Carmichael.";
    }

    std::string mensaje = numero + " no es de Carmichael: es compuesto (" + numero + " = " + factor +
        " × " + otro_factor + "). ";
    if (analisis.base_testigo == 0) return mensaje + "No supera todas las bases coprimas probadas.";
    return mensaje + "Contraejemplo: con la base coprima " + std::to_string(analisis.base_testigo) +
        ", " + std::to_string(analisis.base_testigo) + "^(" + numero + "-1) mod " + numero + " = " +
        std::to_string(analisis.resto_testigo) + " ≠ 1. Solo pasan " +
        std::to_string(analisis.bases_superadas) + " de " + std::to_string(analisis.bases_totales) +
        " bases coprimas (" + proporcion;
}


}

Component CrearPantallaCarmichael() {
    auto estado = std::make_shared<EstadoCarmichael>();
    auto entrada_numero = Input(&estado->numero, "Número");

    estado->analisis->al_progresar = [estado] {
        const auto& analisis = *estado->analisis;
        estado->resultado = "Base " + std::to_string(analisis.base_actual) +
            (analisis.ultima_superada ? " supera" : " falla") + " Fermat. Progreso: " +
            std::to_string(analisis.bases_revisadas) + " de " +
            std::to_string(analisis.bases_totales) + " bases coprimas.";
        estado->tipo_resultado = TipoResultado::Informacion;
    };
    estado->analisis->al_terminar = [estado] {
        estado->resultado = ExplicacionFinal(*estado->analisis);
        estado->tipo_resultado = EsNumeroCarmichael(*estado->analisis) ? TipoResultado::Exito : TipoResultado::Informacion;
    };

    auto calcular = Button("Calcular", [estado] {
        constexpr unsigned int limite = 2'000;
        auto& analisis = *estado->analisis;
        // Al reiniciar aquí, una entrada no válida tampoco deja activa la animación anterior.
        analisis.animando = false;
        analisis.terminado = false;
        analisis.numero = 0;
        analisis.siguiente_base = 2;
        analisis.base_actual = 0;
        analisis.ultima_superada = false;
        analisis.bases_totales = 0;
        analisis.bases_revisadas = 0;
        analisis.bases_superadas = 0;
        analisis.bases_fallidas = 0;
        analisis.base_testigo = 0;
        analisis.resto_testigo = 0;
        analisis.factor_primo = 0;
        analisis.es_primo = false;

        unsigned int n;
        if (!ConvertirAEnteroSinSigno(estado->numero, n) || n < 2) {
            estado->resultado = "Introduce un número entero mayor o igual que 2.";
            estado->tipo_resultado = TipoResultado::Error;
            return;
        }
        if (n > limite) {
            estado->resultado = "La comprobación exhaustiva está limitada a n ≤ " +
                std::to_string(limite) + ".";
            estado->tipo_resultado = TipoResultado::Error;
            return;
        }

        analisis.numero = n;
        BuscarFactorPrimo(analisis);
        // Solo cuentan bases coprimas: Fermat no evalúa las que comparten un divisor con n.
        for (unsigned int base = 2; base < n; ++base) {
            if (std::gcd(base, n) == 1) ++analisis.bases_totales;
        }
        estado->resultado = "Iniciando comprobación de las bases coprimas...";
        estado->tipo_resultado = TipoResultado::Informacion;
        analisis.animando = true;
        *estado->seccion_actual = 1;
        animation::RequestAnimationFrame();
    });

    auto entradas = SeccionEntrada("Datos de entrada", {entrada_numero}, calcular);
    auto grafico = Renderer([estado] {
        const auto& analisis = *estado->analisis;
        const double total = analisis.bases_totales;
        // Las proporciones se traducen en ángulos para pintar los sectores del gráfico circular.
        const double proporciones[3] = {
            total == 0 ? 0.0 : analisis.bases_superadas / total,
            total == 0 ? 0.0 : analisis.bases_fallidas / total,
            total == 0 ? 1.0 : (total - analisis.bases_revisadas) / total
        };
        auto tarta = canvas(48, 40, [proporciones](Canvas& dibujo) {
            const int centro_x = dibujo.width() / 2;
            const int centro_y = dibujo.height() / 2;
            const int radio = std::max(1, std::min(centro_x, centro_y) - 2);
            const double limite_verde = 2.0 * std::numbers::pi * proporciones[0];
            const double limite_rojo = 2.0 * std::numbers::pi * (proporciones[0] + proporciones[1]);
            const auto& angulos = AngulosGrafico();
            for (int y = 0; y < dibujo.height(); ++y) {
                for (int x = 0; x < dibujo.width(); ++x) {
                    const double dx = x - centro_x;
                    const double dy = y - centro_y;
                    if (dx * dx + dy * dy > radio * radio) continue;
                    const double angulo = angulos[y * 48 + x];
                    const Color color_sector = angulo < limite_verde ? Color::Green :
                        angulo < limite_rojo ? Color::Red : Color::GrayDark;
                    dibujo.DrawPoint(x, y, true, color_sector);
                }
            }
            dibujo.DrawPointCircle(centro_x, centro_y, radio, Color::White);
        });

        const double porcentaje_superadas = total == 0 ? 0.0 : 100.0 * analisis.bases_superadas / total;
        const double porcentaje_fallidas = total == 0 ? 0.0 : 100.0 * analisis.bases_fallidas / total;
        const double porcentaje_pendientes = total == 0 ? 0.0 :
            100.0 * (analisis.bases_totales - analisis.bases_revisadas) / total;
        return vbox({
            text("Bases coprimas: resultado de Fermat") | bold,
            total == 0
                ? text("Calcula un número para ver el análisis de sus bases coprimas.") | color(Color::GrayLight)
                : hbox({tarta, vbox({
                text("Superan: " + std::to_string(analisis.bases_superadas) + " (" +
                    FormatearPorcentaje(porcentaje_superadas) + "%)") | color(Color::Green) | bold,
                text("Fallan: " + std::to_string(analisis.bases_fallidas) + " (" +
                    FormatearPorcentaje(porcentaje_fallidas) + "%)") | color(Color::Red) | bold,
                text("Pendientes: " + std::to_string(analisis.bases_totales - analisis.bases_revisadas) + " (" +
                    FormatearPorcentaje(porcentaje_pendientes) + "%)") | color(Color::GrayLight),
                text("Revisadas: " + std::to_string(analisis.bases_revisadas) + "/" + std::to_string(analisis.bases_totales)),
                text("Verde: supera | Rojo: falla | Gris: pendiente") | color(Color::GrayLight)
            })}),
            text("Se analizan las bases coprimas del intervalo 2..n-1.") | color(Color::GrayLight)
        });
    });
    auto desarrollo = Container::Vertical({
        AnimacionCarmichael(estado->analisis),
        grafico
    });

    return DisenoPantalla(
        "Números de Carmichael",
        "Comprueba progresivamente qué bases coprimas engañan al test de Fermat.",
        entradas,
        VistaExplicacion("Fundamento teórico", {
            "Un número de Carmichael es compuesto y cumple a^(n-1) ≡ 1 (mod n) para toda base a coprima con n.",
            "561 = 3 × 11 × 17 es el menor número de Carmichael. Por ejemplo, 2^560 ≡ 1 (mod 561), aunque 561 no es primo.",
            "La prueba recorre las bases de 2 a n - 1, omite las que no son coprimas y actualiza el gráfico con cada resultado.",
            "Por eso, superar Fermat con una o incluso con todas las bases coprimas no basta para concluir que un número es primo."
        }),
        desarrollo,
        Renderer([estado] { return VistaResultado(estado->resultado, estado->tipo_resultado); }),
        estado->seccion_actual
    );
}
