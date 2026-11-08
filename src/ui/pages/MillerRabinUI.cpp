#include "../../../include/ui/pages/MillerRabinUI.h"
#include "../../../include/algorithms/MillerRabin.h"

#include "../../../include/ui/common/ResultView.h"
#include "../../../include/ui/animation/MillerRabinAnimation.h"
#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/ui/common/RandomBaseControl.h"

#include "../../../include/utils/InputUtils.h"
#include "../../../include/utils/RandomUtils.h"

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/animation.hpp"

using namespace ftxui;


struct EstadoMillerRabin {
    std::string n = "561";
    std::string base = "2";
    bool base_aleatoria = false;

    std::string resultado;
    std::shared_ptr<EstadoAnimacionMillerRabin> animacion = std::make_shared<EstadoAnimacionMillerRabin>();

    TipoResultado tipo_resultado = TipoResultado::Informacion;
    std::shared_ptr<int> seccion_actual = std::make_shared<int>(0);
};


Component CrearPantallaMillerRabin() {
    auto estado = std::make_shared<EstadoMillerRabin>();


    auto entrada_n = Input(&estado->n, "Introduce n");
    auto entrada_base = Input(&estado->base, "Introduce la base");

    auto boton = Button("Calcular", [estado] {
        // Los pasos previos no deben seguir apareciendo si la entrada nueva no es válida.
        estado->animacion->pasos.clear();
        estado->animacion->pasos_visibles = 0;
        // La base puede quedar vacía cuando se activa la selección aleatoria.
        if (estado->n.empty() || (estado->base.empty() && !estado->base_aleatoria)) {
            estado->resultado = "Introduce el número y la base.";
            estado->tipo_resultado = TipoResultado::Error;
            estado->animacion->pasos.clear();

            return;
        }


        unsigned int valor_n;
        unsigned int valor_base;

        if (!ConvertirAEnteroSinSigno(estado->n, valor_n) ||
            (!estado->base_aleatoria && !ConvertirAEnteroSinSigno(estado->base, valor_base))) {
            estado->resultado = "Introduce números enteros válidos.";
            estado->tipo_resultado = TipoResultado::Error;
            estado->animacion->pasos.clear();

            return;
        }


        try {
            if (estado->base_aleatoria) {
                // La base aleatoria requiere el intervalo [2, n - 2]; para n pequeño se usa 2 y el test trata sus casos límite.
                if (valor_n < 5) {
                    valor_base = 2;
                } else {
                    valor_base = EnteroAleatorio(2, valor_n - 2);
                }
                estado->base = std::to_string(valor_base);
            }
            const auto resultado_ronda = MillerRabin(
                valor_n,
                valor_base,
                estado->animacion->pasos
            );

            const bool superada = resultado_ronda == ResultadoMillerRabin::SuperaRonda;
            estado->resultado = superada
                ? "El número supera esta ronda de Miller-Rabin; esto no demuestra que sea primo."
                : "El número es compuesto: no ha superado la ronda de Miller-Rabin.";

            // Superar una ronda no demuestra primalidad; evita presentarlo como éxito definitivo.
            estado->tipo_resultado = superada ? TipoResultado::Informacion : TipoResultado::Error;
            *estado->seccion_actual = 1;
            animation::RequestAnimationFrame();
        } catch (const std::exception &e) {
            estado->resultado = e.what();
            estado->tipo_resultado = TipoResultado::Error;
            estado->animacion->pasos.clear();
        }
    });


    auto entradas = SeccionEntrada(
        "Datos de entrada",
        {entrada_n, entrada_base},
        boton,
        ControlBaseAleatoria(&estado->base_aleatoria)
    );
    return DisenoPantalla(
        "Test de Miller-Rabin",
        "Comprobación de primalidad mediante una base.",

        entradas,

        VistaExplicacion(
                "Fundamento teórico",
                {
                    "Miller-Rabin busca testigos de que un número impar n > 2 es compuesto.",
                    "Se descompone n - 1 = 2^s · d, con d impar, y se calcula x = base^d mod n.",
                    "Si x es 1 o n - 1, la base supera la ronda.",
                    "Si no, se eleva x al cuadrado módulo n hasta s - 1 veces. Obtener n - 1 hace que la ronda se supere.",
                    "Si no aparece n - 1, la base demuestra que n es compuesto; obtener 1 antes también lo demuestra.",
                    "Ejemplo: para n = 561 y base 2, la secuencia llega a 1 sin pasar antes por 560; la ronda detecta que es compuesto.",
                    "Superar una ronda solo indica que la base no encontró un testigo; no demuestra que n sea primo."
                }
            ),

        AnimacionMillerRabin(estado->animacion),

        Renderer([estado] {
            return VistaResultado(
                estado->resultado,
                estado->tipo_resultado
            );
        }),
        estado->seccion_actual
    );
}
