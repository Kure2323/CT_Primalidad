#include "../../../include/ui/pages/EratosthenesUI.h"
#include "../../../include/ui/animation/EratosthenesAnimation.h"
#include "../../../include/ui/common/ResultView.h"
#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/utils/InputUtils.h"

#include <memory>
#include <string>
#include <exception>

#include "ftxui/component/component.hpp"

using namespace ftxui;


struct EstadoEratostenes {
    std::string fin = "200";
    std::string resultado = "Introduce un límite y pulsa Calcular.";
    TipoResultado tipo_resultado = TipoResultado::Informacion;
    std::shared_ptr<EstadoAnimacionEratostenes> animacion;
    std::shared_ptr<int> seccion_actual = std::make_shared<int>(0);
};

Component CrearPantallaEratostenes() {
    auto estado = std::make_shared<EstadoEratostenes>();
    estado->animacion = std::make_shared<EstadoAnimacionEratostenes>();
    estado->animacion->al_terminar = [estado](const std::vector<int>& primos) {
        estado->resultado.clear();
        for (int primo : primos) {
            if (!estado->resultado.empty()) estado->resultado += ", ";
            estado->resultado += std::to_string(primo);
        }
        if (estado->resultado.empty()) estado->resultado = "No se han encontrado primos.";
        estado->tipo_resultado = TipoResultado::Exito;
    };
    auto entrada_fin = Input(&estado->fin, "Límite");
    auto ejecutar = Button("Calcular", [estado] {
        constexpr unsigned int limite = 1'000;
        unsigned int fin;
        if (!ConvertirAEnteroSinSigno(estado->fin, fin)) {
            estado->resultado = "Introduce un número entero sin signo válido.";
            estado->tipo_resultado = TipoResultado::Error;
            estado->animacion->calculado = false;
            estado->animacion->animando = false;
            estado->animacion->compuesto.clear();
            return;
        }
        try {
            if (fin > limite) {
                throw std::invalid_argument("El límite de la criba animada es " +
                    std::to_string(limite) + ".");
            }
            auto& anim = *estado->animacion;
            anim.limite = fin;
            // Una nueva ejecución reemplaza por completo las marcas y la posición de la anterior.
            // El índice del vector representa el número; el 1 se marca aparte porque no es primo.
            anim.compuesto.assign(fin + 1, false);
            if (fin >= 1) anim.compuesto[1] = true;
            anim.primo_actual = 2;
            anim.multiplo_actual = 0;
            anim.pagina_actual = 0;
            anim.algoritmo_terminado = fin < 4;
            anim.calculado = true;
            anim.animando = fin >= 4;
            estado->resultado = anim.animando ? "Criba en curso..." : "";
            estado->tipo_resultado = anim.animando ? TipoResultado::Informacion : TipoResultado::Exito;
            if (!anim.animando) {
                // Si fin < 4 no hay compuestos que marcar; los candidatos entre 2 y fin son primos.
                std::vector<int> primos;
                for (unsigned int n = 2; n <= fin; ++n) primos.push_back(static_cast<int>(n));
                anim.al_terminar(primos);
            }
            *estado->seccion_actual = 1;
            animation::RequestAnimationFrame();
        } catch (const std::exception& e) {
            estado->resultado = e.what();
            estado->tipo_resultado = TipoResultado::Error;
            estado->animacion->calculado = false;
            estado->animacion->animando = false;
        }
    });
    auto entradas = SeccionEntrada("Datos de entrada", {entrada_fin}, ejecutar);
    auto cuadrícula = AnimacionEratostenes(estado->animacion);
    auto control_velocidad = Slider("Velocidad", &estado->animacion->velocidad, 1, 100, 1);
    auto controles_desarrollo = Container::Vertical({control_velocidad, cuadrícula});
    auto desarrollo = Renderer(controles_desarrollo, [estado, control_velocidad, cuadrícula] {
        return vbox({
            text("Velocidad: " + std::to_string(estado->animacion->velocidad) + " (1 lenta, 100 rápida)"),
            control_velocidad->Render(),
            separator(),
            cuadrícula->Render()
        });
    });

    return DisenoPantalla(
        "Criba de Eratóstenes",
        "Obtención de números primos mediante la eliminación de múltiplos.",
        entradas,
        VistaExplicacion("Fundamento teórico", {
            "La criba de Eratóstenes encuentra todos los números primos menores o iguales que un límite dado.",
            "Se enumeran los números desde 2 hasta el límite y se eliminan los múltiplos de cada primo, comenzando por su cuadrado.",
            "Cada primo p empieza a tachar desde p² porque sus múltiplos menores ya se marcaron con primos anteriores. Por ejemplo, al procesar 3 se empieza por 9: 6 ya se tachó al procesar 2.",
            "Basta procesar primos hasta la raíz cuadrada del límite: si un número es compuesto, se puede escribir como producto de dos factores. Al menos uno debe ser menor o igual que su raíz cuadrada; si ambos fueran mayores, su producto también sería mayor que el número.",
            "Por tanto, todo compuesto hasta el límite tiene un divisor primo que ya se procesa antes de llegar a la raíz cuadrada. Si al terminar no ha sido tachado, es primo.",
            "Los números que quedan sin eliminar son primos."
        }),
        desarrollo,
        Renderer([estado] { return VistaResultado(estado->resultado, estado->tipo_resultado); }),
        estado->seccion_actual
    );
}
