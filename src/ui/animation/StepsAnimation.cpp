#include "../../../include/ui/animation/StepsAnimation.h"
#include "../../../include/ui/animation/StepsView.h"

#include <algorithm>
#include <chrono>
#include <utility>

#include "ftxui/component/animation.hpp"
#include "ftxui/component/component_base.hpp"

namespace {
/** Revela progresivamente los pasos de la explicación. */
class PasosAnimados : public ftxui::ComponentBase {
public:
    explicit PasosAnimados(std::shared_ptr<EstadoAnimacionPasos> estado)
        : estado_(std::move(estado)) {}

    /** Representa únicamente la parte de la explicación ya revelada. */
    ftxui::Element OnRender() override {
        const auto cantidad = std::min(estado_->pasos_visibles, estado_->pasos.size());
        return VistaPasos(std::span<const PasoExplicativo>(estado_->pasos).first(cantidad));
    }

    /** Hace visibles nuevos pasos y solicita otro fotograma mientras falten. */
    void OnAnimation(ftxui::animation::Params& params) override {
        if (estado_->pasos_visibles >= estado_->pasos.size()) return;
        // Se conserva el tiempo sobrante para que fotogramas lentos no alteren la velocidad.
        tiempo_transcurrido_ += params.duration();
        constexpr auto intervalo = std::chrono::milliseconds(90);
        // Puede hacerse visible más de un paso si transcurrieron varios intervalos.
        while (tiempo_transcurrido_ >= intervalo &&
               estado_->pasos_visibles < estado_->pasos.size()) {
            ++estado_->pasos_visibles;
            tiempo_transcurrido_ -= intervalo;
        }
        if (estado_->pasos_visibles < estado_->pasos.size())
            ftxui::animation::RequestAnimationFrame();
    }

    /** Indica que el componente no necesita recibir el foco. */
    bool Focusable() const override { return false; }

private:
    std::shared_ptr<EstadoAnimacionPasos> estado_;
    ftxui::animation::Duration tiempo_transcurrido_{};
};
}

/** Crea el componente que revela progresivamente los pasos del algoritmo. */
ftxui::Component AnimacionPasos(std::shared_ptr<EstadoAnimacionPasos> estado) {
    return ftxui::Make<PasosAnimados>(std::move(estado));
}
