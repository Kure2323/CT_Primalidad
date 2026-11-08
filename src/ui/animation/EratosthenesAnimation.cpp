#include "../../../include/ui/animation/EratosthenesAnimation.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <utility>

#include "ftxui/component/animation.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/terminal.hpp"

using namespace ftxui;

namespace {
/** Presenta y anima el marcado de compuestos en una cuadrícula paginada. */
class CuadriculaAnimada : public ComponentBase {
public:
    explicit CuadriculaAnimada(std::shared_ptr<EstadoAnimacionEratostenes> estado)
        : estado_(std::move(estado)) {}

    /** Dibuja la página actual y colorea primos y compuestos. */
    Element OnRender() override {
        const unsigned int mostrados = NumerosMostrados();
        const unsigned int columnas = Columnas();
        const unsigned int tamano_pagina = TamanoPagina(columnas);
        const unsigned int cantidad_paginas = CantidadPaginas(mostrados, tamano_pagina);
        estado_->pagina_actual = std::min(estado_->pagina_actual, cantidad_paginas - 1);
        const unsigned int primero = estado_->pagina_actual * tamano_pagina + 1;
        const unsigned int ultimo = std::min(mostrados, primero + tamano_pagina - 1);
        Elements filas;
        const unsigned int primera_fila = ((primero - 1) / columnas) * columnas + 1;
        for (unsigned int fila = primera_fila; fila <= ultimo; fila += columnas) {
            Elements celdas;
            for (unsigned int numero = fila; numero < fila + columnas && numero <= ultimo; ++numero) {
                Element celda = text(std::to_string(numero)) | center | size(WIDTH, EQUAL, 5) | border;
                if (estado_->calculado && numero < estado_->compuesto.size()) {
                    if (numero == 1 || estado_->compuesto[numero]) celda |= color(Color::Red);
                    else if (numero >= 2) celda |= color(Color::Green) | bold;
                }
                celdas.push_back(celda);
            }
            filas.push_back(hbox(std::move(celdas)));
        }
        return vbox({
            text("Representación gráfica (" + std::to_string(primero) + "-" + std::to_string(ultimo) + " de " + std::to_string(mostrados) + ")") | bold,
            text("Página " + std::to_string(estado_->pagina_actual + 1) + " de " + std::to_string(cantidad_paginas) + " — Re Pág / Av Pág para cambiar"),
            separator(), paragraph("Verde: primos | Rojo: compuestos y 1"), vbox(std::move(filas))
        });
    }

    /** Cambia de página al recibir las teclas Av Pág o Re Pág. */
    bool OnEvent(Event event) override {
        const unsigned int mostrados = NumerosMostrados();
        const unsigned int tamano_pagina = TamanoPagina(Columnas());
        const unsigned int cantidad_paginas = CantidadPaginas(mostrados, tamano_pagina);
        if (event == Event::PageDown && estado_->pagina_actual + 1 < cantidad_paginas) {
            ++estado_->pagina_actual;
            return true;
        }
        if (event == Event::PageUp && estado_->pagina_actual > 0) {
            --estado_->pagina_actual;
            return true;
        }
        return false;
    }

    /** Marca los siguientes múltiplos de la criba en función de la velocidad. */
    void OnAnimation(animation::Params& params) override {
        if (!estado_->animando) return;
        tiempo_transcurrido_ += params.duration();
        const auto intervalo = IntervaloActual();
        const unsigned int lote = MarcasPorActualizacion();
        const unsigned int tamano_pagina = TamanoPagina(Columnas());
        while (tiempo_transcurrido_ >= intervalo && estado_->animando) {
            tiempo_transcurrido_ -= intervalo;
            for (unsigned int indice = 0; indice < lote && estado_->animando; ++indice) {
                if (estado_->multiplo_actual == 0) {
                    while (estado_->primo_actual <= estado_->limite / estado_->primo_actual &&
                           estado_->compuesto[estado_->primo_actual]) {
                        ++estado_->primo_actual;
                    }
                    if (estado_->primo_actual > estado_->limite / estado_->primo_actual) {
                        Completar();
                        break;
                    }
                    // Los múltiplos menores que p² ya se marcaron con factores primos menores.
                    estado_->multiplo_actual = estado_->primo_actual * estado_->primo_actual;
                }

                const unsigned int marcado = estado_->multiplo_actual;
                estado_->compuesto[marcado] = true;
                estado_->pagina_actual = (marcado - 1) / tamano_pagina;
                // Comprueba antes de sumar para evitar que el siguiente múltiplo desborde o supere el límite.
                if (estado_->limite - estado_->multiplo_actual < estado_->primo_actual) {
                    estado_->multiplo_actual = 0;
                    ++estado_->primo_actual;
                } else {
                    estado_->multiplo_actual += estado_->primo_actual;
                }
            }
        }
        if (estado_->animando) animation::RequestAnimationFrame();
    }

    /** Permite que la cuadrícula reciba las teclas de paginación. */
    bool Focusable() const override { return true; }

private:
    static constexpr unsigned int filas_por_pagina_ = 10;

    /** Devuelve cuántos números deben aparecer en la cuadrícula. */
    unsigned int NumerosMostrados() const {
        return estado_->calculado ? estado_->limite : 30u;
    }

    /** Calcula la capacidad de una página para el número de columnas dado. */
    unsigned int TamanoPagina(unsigned int columnas) const {
        return columnas * filas_por_pagina_;
    }

    /** Calcula cuántas páginas hacen falta para mostrar todos los números. */
    unsigned int CantidadPaginas(unsigned int mostrados, unsigned int tamano_pagina) const {
        return std::max(1u, (mostrados + tamano_pagina - 1) / tamano_pagina);
    }

    /** Convierte el control de velocidad en el intervalo entre marcas. */
    std::chrono::milliseconds IntervaloActual() const {
        const int velocidad = std::clamp(estado_->velocidad, 1, 100);
        const int velocidad_temporal = std::min(velocidad, 70);
        const double exponente = 3.0 - 1.52 * (velocidad_temporal - 1) / 69.0;
        const auto milisegundos = static_cast<long long>(std::round(std::pow(10.0, exponente)));
        return std::chrono::milliseconds(milisegundos);
    }

    /** Calcula cuántos múltiplos se marcan por actualización a alta velocidad. */
    unsigned int MarcasPorActualizacion() const {
        const int velocidad = std::clamp(estado_->velocidad, 1, 100);
        if (velocidad <= 70) return 1;
        const double progreso = static_cast<double>(velocidad - 70) / 30.0;
        return static_cast<unsigned int>(std::round(std::pow(10000.0, progreso)));
    }

    /** Reúne los primos encontrados y notifica a la pantalla que la criba terminó. */
    void Completar() {
        estado_->algoritmo_terminado = true;
        estado_->animando = false;
        std::vector<int> primos;
        for (unsigned int numero = 2; numero <= estado_->limite; ++numero) {
            if (!estado_->compuesto[numero]) primos.push_back(static_cast<int>(numero));
        }
        if (estado_->al_terminar) estado_->al_terminar(primos);
    }

    /** Estima cuántas columnas caben en el ancho actual del terminal. */
    unsigned int Columnas() const {
        return static_cast<unsigned int>(std::clamp((Terminal::Size().dimx - 8) / 7, 10, 30));
    }

    std::shared_ptr<EstadoAnimacionEratostenes> estado_;
    animation::Duration tiempo_transcurrido_{};
};
}

/** Crea la cuadrícula animada de la criba de Eratóstenes. */
Component AnimacionEratostenes(std::shared_ptr<EstadoAnimacionEratostenes> estado) {
    return Make<CuadriculaAnimada>(std::move(estado));
}
