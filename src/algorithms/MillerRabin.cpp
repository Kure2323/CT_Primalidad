#include "../../include/algorithms/MillerRabin.h"
#include "../../include/algorithms/NumberTheory.h"

#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// d y s cumplen n - 1 = d * 2^s.
using Descomposicion = std::pair<unsigned int, unsigned int>;

// Separa n - 1 en un número impar y una potencia de 2.
static Descomposicion DescomponerNMenosUno(unsigned int n) {
    unsigned int d = n - 1;
    unsigned int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }
    return {d, s};
}

static ResultadoMillerRabin FinalizarRonda(ResultadoMillerRabin resultado, const std::string& explicacion,
                                        std::vector<PasoExplicativo>& pasos) {
    pasos.push_back({explicacion, TipoPaso::Texto});
    return resultado;
}
/** Ejecuta una ronda y descarta los pasos detallados. */
ResultadoMillerRabin MillerRabin(unsigned int n, unsigned int base) {
    std::vector<PasoExplicativo> pasos;
    return MillerRabin(n, base, pasos);
}

/** Ejecuta una ronda y guarda su explicación en pasos. */
ResultadoMillerRabin MillerRabin(unsigned int n, unsigned int base,
                                 std::vector<PasoExplicativo>& pasos) {
    constexpr unsigned int limite = 10'000;
    pasos.clear();
    if (n > limite)
        throw std::invalid_argument("El test de Miller-Rabin está limitado a n ≤ " +
                                    std::to_string(limite) + ".");
    if (n < 2) throw std::invalid_argument("El número debe ser mayor o igual que 2.");
    // 2 y 3 son primos; no necesitan pasar la prueba.
    if (n <= 3) {
        pasos.push_back({"El número supera la ronda por ser primo.", TipoPaso::Texto});
        return ResultadoMillerRabin::SuperaRonda;
    }
    if (n % 2 == 0)
        return FinalizarRonda(ResultadoMillerRabin::Compuesto, "El número es par y mayor que 2.", pasos);
    if (base < 2 || base > n - 2)
        throw std::invalid_argument("La base debe estar entre 2 y n - 2.");

    // La base debe ser coprima con n para poder usar el test.
    pasos.push_back({"Base utilizada en esta ronda: " + std::to_string(base),
                     TipoPaso::Encabezado});
    if (std::gcd(base, n) != 1)
        return FinalizarRonda(ResultadoMillerRabin::Compuesto, "La base y el número tienen un divisor común.", pasos);

    // Primero probamos la potencia con d y luego elevamos el resultado al cuadrado.
    const auto [d, s] = DescomponerNMenosUno(n);
    pasos.push_back({"Descomposición de n-1", TipoPaso::Encabezado});
    pasos.push_back({std::to_string(n - 1) + " = 2^" + std::to_string(s) + " * " +
                     std::to_string(d), TipoPaso::Calculo});

    unsigned int x = PotenciaModular(base, d, n, pasos);
    pasos.push_back({"Potencia inicial:", TipoPaso::Encabezado});
    pasos.push_back({"x = " + std::to_string(base) + "^" + std::to_string(d) +
                     " mod " + std::to_string(n) + " = " + std::to_string(x),
                     TipoPaso::Calculo});
    if (x == 1 || x == n - 1)
        return FinalizarRonda(ResultadoMillerRabin::SuperaRonda, "La potencia inicial permite superar la ronda.", pasos);

    // Cada cuadrado duplica el exponente: de base^(d*2^(r-1)) a base^(d*2^r).
    // Si aparece n - 1, la ronda pasa; si aparece 1 antes, n es compuesto.
    for (unsigned int iteracion = 1; iteracion < s; ++iteracion) {
        const unsigned int resto_anterior = x;
        const unsigned long long cuadrado = static_cast<unsigned long long>(resto_anterior) * resto_anterior;
        x = static_cast<unsigned int>(cuadrado % n);
        pasos.push_back({"Iteración " + std::to_string(iteracion) + ":", TipoPaso::Iteracion});
        pasos.push_back({"Elevamos al cuadrado el resto anterior; así se duplica el exponente.",
                         TipoPaso::Texto});
        pasos.push_back({std::to_string(resto_anterior) + "^2 = " + std::to_string(cuadrado),
                         TipoPaso::Calculo});
        pasos.push_back({"Reducimos el cuadrado módulo " + std::to_string(n) + ": " +
                         std::to_string(cuadrado) + " mod " + std::to_string(n) +
                         " = " + std::to_string(x), TipoPaso::Calculo});
        if (x == n - 1) {
            return FinalizarRonda(ResultadoMillerRabin::SuperaRonda,
                          "Se obtiene n - 1 (equivalente a -1 módulo n). La ronda se supera.",
                          pasos);
        }
        if (x == 1) {
            return FinalizarRonda(ResultadoMillerRabin::Compuesto,
                          "Se obtiene 1 antes que n - 1: apareció una raíz cuadrada no trivial "
                          "de 1 módulo n, así que n es compuesto.", pasos);
        }
    }

    return FinalizarRonda(ResultadoMillerRabin::Compuesto,
                  "No se obtiene n - 1. El número es compuesto.", pasos);
}
