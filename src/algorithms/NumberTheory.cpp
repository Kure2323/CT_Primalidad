#include "../../include/algorithms/NumberTheory.h"

#include <string>
#include <stdexcept>

/** Comprueba la primalidad probando divisores impares hasta la raíz. */
bool EsPrimo(unsigned long numero) {
    if (numero < 2) return false;

    if (numero % 2 == 0) return numero == 2;

    for (unsigned long divisor = 3; divisor <= numero / divisor; divisor += 2) {
        if (numero % divisor == 0) {
            return false;
        }
    }
    return true;
}

/** Calcula una potencia modular mediante exponenciación rápida. */
unsigned int PotenciaModular(unsigned int base, unsigned int exponente, unsigned int modulo) {
    if (modulo == 0) throw std::invalid_argument("El módulo debe ser mayor que cero.");
    unsigned long long resultado = 1 % modulo;
    unsigned long long factor = base % modulo;
    while (exponente > 0) {
        // Cada bit activo del exponente incorpora la potencia actual al resultado.
        if (exponente & 1u) resultado = resultado * factor % modulo;
        // Al cuadrar la base y reducir el exponente a la mitad, se conserva la potencia buscada.
        factor = factor * factor % modulo;
        exponente >>= 1;
    }
    return static_cast<unsigned int>(resultado);
}

/** Calcula la potencia modular y guarda una explicación de cada paso. */
unsigned int PotenciaModular(unsigned int base, unsigned int exponente, unsigned int modulo,
                             std::vector<PasoExplicativo>& pasos) {
    if (modulo == 0) {
        throw std::invalid_argument("El módulo debe ser mayor que cero.");
    }
    unsigned long long resultado = 1 % modulo;

    pasos.push_back({"Calculamos " + std::to_string(base) + "^" +
                     std::to_string(exponente) + " mod " +
                     std::to_string(modulo), TipoPaso::Texto});

    unsigned int iteracion = 1;

    while (exponente > 0) {
        pasos.push_back({"Iteración de exponenciación modular " +
                         std::to_string(iteracion) +
                         " (exponente = " + std::to_string(exponente) + ")",
                         TipoPaso::Iteracion});

        if (exponente % 2 == 1) {
            unsigned long long anterior = resultado;
            const unsigned long long producto = anterior * base;
            resultado = producto % modulo;

            pasos.push_back({"El exponente es impar, multiplicamos:", TipoPaso::Texto});
            pasos.push_back({"Producto: " + std::to_string(anterior) + " × " +
                             std::to_string(base) + " = " + std::to_string(producto),
                             TipoPaso::Calculo});
            pasos.push_back({"Resto: " + std::to_string(producto) + " ÷ " +
                             std::to_string(modulo) + " deja " + std::to_string(resultado),
                             TipoPaso::Calculo});
        }

        unsigned int exponente_anterior = exponente;
        exponente /= 2;

        pasos.push_back({"Dividimos el exponente entre 2: " +
                         std::to_string(exponente_anterior) + " / 2 = " +
                         std::to_string(exponente), TipoPaso::Calculo});

        if (exponente > 0) {
            unsigned int base_anterior = base;
            const unsigned long long cuadrado =
                static_cast<unsigned long long>(base) * base;
            base = cuadrado % modulo;

            pasos.push_back({"Cuadrado: " + std::to_string(base_anterior) + "² = " +
                             std::to_string(cuadrado) + "; resto al dividir entre " +
                             std::to_string(modulo) + " = " + std::to_string(base),
                             TipoPaso::Calculo});
        }
        iteracion++;
    }

    pasos.push_back({"El exponente ha llegado a 0. Finaliza la exponenciación modular.",
                     TipoPaso::Texto});
    pasos.push_back({"Resultado final: " + std::to_string(resultado), TipoPaso::Calculo});

    return static_cast<unsigned int>(resultado);
}
