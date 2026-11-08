#include "../../include/algorithms/Prime.h"
#include "../../include/algorithms/Eratosthenes.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>

/** Calcula la cantidad de primos y su diferencia respecto a la aproximación. */
ResultadoPrimos CalcularPrimos(unsigned int limite) {
    constexpr unsigned int limite_maximo = 10'000;
    if (limite < 2 || limite > limite_maximo) {
        throw std::invalid_argument("El límite debe estar entre 2 y " +
                                    std::to_string(limite_maximo) + ".");
    }

    const auto primos = CribaEratostenes(limite);
    const std::size_t cantidad = primos.size();
    const double aproximacion = limite / std::log(limite);
    const double diferencia = std::abs(cantidad - aproximacion);
    std::vector<PuntoComparacionPrimos> puntos_grafico;
    puntos_grafico.reserve(limite - 1);
    std::size_t indice_primo = 0;
    std::size_t primos_hasta_x = 0;
    for (unsigned int x = 2; x <= limite; ++x) {
        while (indice_primo < primos.size() && primos[indice_primo] <= static_cast<int>(x)) {
            ++indice_primo;
            ++primos_hasta_x;
        }
        puntos_grafico.push_back({x, primos_hasta_x, x / std::log(x)});
    }
    return {limite, cantidad, aproximacion, diferencia, std::move(puntos_grafico)};
}
