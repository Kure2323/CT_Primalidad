#include "../../../include/ui/pages/RsaPrimeUI.h"
#include "../../../include/algorithms/RsaPrimeGenerator.h"
#include "../../../include/utils/InputUtils.h"
#include "../../../include/ui/common/ScreenLayout.h"
#include "../../../include/ui/common/InputSection.h"
#include "../../../include/ui/common/ExplanationView.h"
#include "../../../include/ui/common/ResultView.h"
#include <memory>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
using namespace ftxui;


Component CrearPantallaGeneradorRsa() {
    constexpr unsigned int tamano_inicial = 512;
    // Los componentes y sus callbacks comparten estos datos durante toda la vida de la pantalla.
    auto cantidad_bits = std::make_shared<std::string>(std::to_string(tamano_inicial));
    auto resultado = std::make_shared<std::string>("Elige el tamaño y pulsa Generar.");
    auto tipo_resultado = std::make_shared<TipoResultado>(TipoResultado::Informacion);
    auto seccion = std::make_shared<int>(0);
    auto entrada = Input(&*cantidad_bits, "Bits (p. ej. 512 o 1024)");
    auto generar = Button("Generar primo probable", [cantidad_bits, resultado, tipo_resultado, seccion] {
        unsigned int tamano;
        if (!ConvertirAEnteroSinSigno(*cantidad_bits, tamano)) {
            *resultado = "Introduce un tamaño válido.";
            *tipo_resultado = TipoResultado::Error;
            return;
        }
        try {
            // Los contadores separan los descartes por factores pequeños de los descartes por Miller-Rabin.
            // La llamada es síncrona: la interfaz no se repinta hasta que termina la generación.
            const auto generado = GenerarPrimoTamanoRsa(tamano);
            *resultado = "Primo probable de " + std::to_string(tamano) + " bits:\n" + ConvertirADecimal(generado.primo) +
                "\nCandidatos probados: " + std::to_string(generado.candidatos_probados) +
                " | descartados por factor pequeño: " + std::to_string(generado.descartados_por_factor_pequeno) +
                " | descartados por Miller-Rabin: " + std::to_string(generado.descartados_por_miller_rabin) +
                "\nMiller-Rabin: 16 rondas.";
            *tipo_resultado = TipoResultado::Exito;
            *seccion = 1;
        } catch (const std::exception& error) {
            *resultado = error.what();
            *tipo_resultado = TipoResultado::Error;
        }
    });
    auto entradas = SeccionEntrada("Generación", {entrada}, generar);
    auto desarrollo = Renderer([resultado] { return paragraph(*resultado) | flex; });
    return DisenoPantalla("Generador de primos tamaño RSA",
        "Genera un probable primo aleatorio con el tamaño indicado y contabiliza los candidatos descartados.",
        entradas,
        VistaExplicacion("Fundamento teórico", {
            "Se generan candidatos aleatorios con el bit más significativo activo y el bit menos significativo activo, para asegurar el tamaño y que sean impares.",
            "Primero se descartan candidatos divisibles por primos pequeños; después se aplica Miller-Rabin durante 16 rondas.",
            "Superar Miller-Rabin indica probable primo, no una demostración matemática absoluta. El generador es educativo y no debe usarse para crear claves RSA reales."
        }), desarrollo,
        Renderer([resultado, tipo_resultado] { return VistaResultado(*resultado, *tipo_resultado); }), seccion);
}
