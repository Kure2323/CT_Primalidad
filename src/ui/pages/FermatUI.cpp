#include "../../../include/ui/pages/FermatUI.h"
#include "../../../include/algorithms/Fermat.h"

#include "../../../include/ui/animation/FermatAnimation.h"
#include "../../../include/ui/common/ResultView.h"
#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/ui/common/RandomBaseControl.h"
#include "../../../include/utils/InputUtils.h"

#include <exception>
#include <memory>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/animation.hpp"

using namespace ftxui;

namespace {
struct EstadoFermat {
    std::string numero = "561";
    std::string base = "2";
    bool base_aleatoria = false;
    std::string resultado = "Introduce un número y una base.";
    TipoResultado tipo_resultado = TipoResultado::Informacion;
    std::shared_ptr<EstadoAnimacionFermat> animacion = std::make_shared<EstadoAnimacionFermat>();
    std::shared_ptr<int> seccion_actual = std::make_shared<int>(0);
};
}

Component CrearPantallaFermat() {
    auto estado = std::make_shared<EstadoFermat>();
    auto entrada_numero = Input(&estado->numero, "Número");
    auto entrada_base = Input(&estado->base, "Base");

    auto ejecutar = Button("Calcular", [estado] {
        // Evita conservar pasos de una ejecución anterior, incluso si falla la nueva entrada.
        estado->animacion->pasos_visibles = 0;
        estado->animacion->pasos.clear();

        unsigned int n;
        unsigned int base = 0;
        if (!ConvertirAEnteroSinSigno(estado->numero, n) || n < 2) {
            estado->resultado = "Introduce un número entero mayor o igual que 2.";
            estado->tipo_resultado = TipoResultado::Error;
            return;
        }
        if (n != 2 && !estado->base_aleatoria &&
            (!ConvertirAEnteroSinSigno(estado->base, base) || base < 2 || base >= n)) {
            estado->resultado = "La base debe estar entre 2 y n - 1.";
            estado->tipo_resultado = TipoResultado::Error;
            return;
        }

        try {
            // Con base aleatoria, Fermat elige y devuelve la base a través de la referencia.
            const bool superado = Fermat(n, base, estado->animacion->pasos, estado->base_aleatoria);
            if (base != 0) estado->base = std::to_string(base);
            // Para n = 2 no existe una base en el intervalo permitido; el algoritmo resuelve ese caso directamente.
            if (n == 2) {
                estado->resultado = "2 es primo y supera el caso especial del test de Fermat.";
            } else {
                estado->resultado = superado
                    ? std::to_string(n) + " supera el test de Fermat para la base " + std::to_string(base) + "."
                    : std::to_string(n) + " es compuesto para la base " + std::to_string(base) + ".";
            }
            estado->tipo_resultado = superado ? TipoResultado::Exito : TipoResultado::Error;
            *estado->seccion_actual = 1;
            animation::RequestAnimationFrame();
        } catch (const std::exception& e) {
            estado->resultado = e.what();
            estado->tipo_resultado = TipoResultado::Error;
            estado->animacion->pasos.clear();
        }
    });

    auto entradas = SeccionEntrada(
        "Datos de entrada",
        {entrada_numero, entrada_base, ControlBaseAleatoria(&estado->base_aleatoria)},
        ejecutar
    );
    return DisenoPantalla(
        "Test de Fermat",
        "Comprobación de primalidad mediante el pequeño teorema de Fermat.",
        entradas,
        VistaExplicacion("Fundamento teórico", {
            "El pequeño teorema de Fermat dice que, si p es primo y p no divide a a, entonces a^(p-1) ≡ 1 (mod p).",
            "El test aplica la condición a un número n: para una base a coprima con n, calcula a^(n-1) mod n.",
            "Si el resto no es 1, n es compuesto. Si es 1, solo supera la prueba para esa base: no demuestra que sea primo.",
            "Ejemplo: 561 es compuesto, pero con la base 2 se obtiene 2^560 ≡ 1 (mod 561), así que esa ronda no lo detecta.",
            "Los números de Carmichael, como 561, superan la congruencia para todas las bases coprimas con ellos."
        }),
        AnimacionFermat(estado->animacion),
        Renderer([estado] { return VistaResultado(estado->resultado, estado->tipo_resultado); }),
        estado->seccion_actual
    );
}
