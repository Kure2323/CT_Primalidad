#include "../../../include/ui/animation/CarmichaelAnimation.h"
#include "../../../include/algorithms/NumberTheory.h"

#include <chrono>
#include <numeric>
#include <utility>

#include "ftxui/component/animation.hpp"
#include "ftxui/component/component_base.hpp"

using namespace ftxui;

/** Componente que procesa progresivamente el análisis de Carmichael. */
class ComponenteAnimacionCarmichael : public ComponentBase {
public:
    explicit ComponenteAnimacionCarmichael(std::shared_ptr<EstadoAnimacionCarmichael> estado)
        : estado_(std::move(estado)) {}

    /** La pantalla representa el estado compartido; este componente no dibuja contenido. */
    Element OnRender() override { return text(""); }

    /** Avanza las bases según el tiempo transcurrido y solicita el siguiente fotograma. */
    void OnAnimation(animation::Params& params) override {
        if (!estado_->animando) return;
        tiempo_transcurrido_ += params.duration();
        constexpr auto intervalo = std::chrono::milliseconds(35);
        while (tiempo_transcurrido_ >= intervalo && estado_->animando) {
            tiempo_transcurrido_ -= intervalo;
            ProcesarSiguienteBase();
        }
        if (estado_->animando) animation::RequestAnimationFrame();
    }

    /** Indica que el componente no participa en el recorrido del foco. */
    bool Focusable() const override { return false; }

private:
    /** Procesa la siguiente base coprima o finaliza si ya no quedan bases. */
    void ProcesarSiguienteBase() {
        // El test de Fermat solo considera bases coprimas con el número analizado.
        while (estado_->siguiente_base < estado_->numero &&
               std::gcd(estado_->siguiente_base, estado_->numero) != 1) {
            ++estado_->siguiente_base;
        }
        if (estado_->siguiente_base >= estado_->numero) {
            Terminar();
            return;
        }

        const unsigned int base = estado_->siguiente_base++;
        estado_->base_actual = base;

        const unsigned int resto = PotenciaModular(base, estado_->numero - 1, estado_->numero);
        // Una base supera Fermat exactamente cuando este resto es 1.
        estado_->ultima_superada = resto == 1;
        if (estado_->ultima_superada) {
            ++estado_->bases_superadas;
        } else {
            ++estado_->bases_fallidas;
            if (estado_->base_testigo == 0) {
                estado_->base_testigo = base;
                estado_->resto_testigo = resto;
            }
        }
        ++estado_->bases_revisadas;
        if (estado_->al_progresar) estado_->al_progresar();
    }

    /** Detiene la animación y notifica a la pantalla que el análisis ha terminado. */
    void Terminar() {
        estado_->animando = false;
        estado_->terminado = true;
        if (estado_->al_terminar) estado_->al_terminar();
    }

    std::shared_ptr<EstadoAnimacionCarmichael> estado_;
    animation::Duration tiempo_transcurrido_{};
};

/** Crea el componente que prueba progresivamente las bases coprimas con el número. */
Component AnimacionCarmichael(std::shared_ptr<EstadoAnimacionCarmichael> estado) {
    return Make<ComponenteAnimacionCarmichael>(std::move(estado));
}
