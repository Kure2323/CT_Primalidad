#include "../../include/algorithms/Fermat.h"
#include "../../include/algorithms/NumberTheory.h"
#include "../../include/utils/RandomUtils.h"

#include <numeric>
#include <stdexcept>
#include <string>

/** Evita recorridos exhaustivos demasiado largos. */
static void ValidarLimiteAnalisis(unsigned int n) {
    // Probar tantas bases tarda; por eso ponemos un máximo.
    const unsigned int limite = 2'000;
    if (n > limite)
        throw std::invalid_argument("El análisis exhaustivo de bases está limitado a n ≤ " +
                                    std::to_string(limite) + ".");
}

/** Calcula la potencia modular sin generar texto para la interfaz. */
static unsigned int PotenciaModularRapida(unsigned int base, unsigned int exponente, unsigned int modulo) {
    unsigned long long resultado = 1;
    unsigned long long factor = base % modulo;
    while (exponente > 0) {
        if (exponente & 1u) resultado = resultado * factor % modulo;
        factor = factor * factor % modulo;
        exponente >>= 1;
    }
    return static_cast<unsigned int>(resultado);
}
/** Ejecuta una ronda del test y guarda los pasos que la explican. */
bool Fermat(unsigned int n, unsigned int& base, std::vector<PasoExplicativo>& pasos, bool base_aleatoria) {
    const unsigned int limite = 10'000;
    pasos.clear();
    if (n > limite)
        throw std::invalid_argument("El test de Fermat está limitado a n ≤ " + std::to_string(limite) + ".");

    pasos.push_back({"Comprobando el número n = " + std::to_string(n), TipoPaso::Texto});
    // 2 y 3 son primos. Para 3 solo se puede usar la base 2.
    if (n < 2) return false;
    if (n == 2) return true;
    if (n == 3) {
        if (base_aleatoria) base = 2;
        else if (base != 2) throw std::invalid_argument("Para n = 3, la única base válida es 2.");
        pasos.push_back({"El número 3 es primo; se usa la única base válida 2.", TipoPaso::Texto});
        return true;
    }

    // Elegimos una base que no comparta divisores con n.
    if (base_aleatoria) {
        do { base = EnteroAleatorio(2, n - 1); }
        while (std::gcd(base, n) != 1);
    } else {
        if (base < 2 || base >= n)
            throw std::invalid_argument("La base debe estar entre 2 y n - 1.");
        if (std::gcd(base, n) != 1) {
            pasos.push_back({"La base y el número tienen un divisor común.", TipoPaso::Texto});
            return false;
        }
    }

    pasos.push_back({"Base utilizada: " + std::to_string(base), TipoPaso::Encabezado});
    const unsigned int resultado = PotenciaModular(base, n - 1, n, pasos);
    pasos.push_back({std::to_string(base) + "^" + std::to_string(n - 1) + " mod " +
                     std::to_string(n) + " = " + std::to_string(resultado),
                     TipoPaso::Calculo});
    return resultado == 1;
}

/** Revisa las bases coprimas, clasifica n y guarda un posible contraejemplo. */
AnalisisBasesFermat AnalizarBasesFermat(unsigned int n) {
    ValidarLimiteAnalisis(n);
    AnalisisBasesFermat analisis;
    if (n < 2) return analisis;

    // Buscamos un divisor. Si no hay ninguno hasta la raíz, n es primo.
    for (unsigned int factor = 2; factor <= n / factor; ++factor) {
        if (n % factor == 0) {
            analisis.factor_primo = factor;
            break;
        }
    }
    // Si encontramos un divisor, n es compuesto; si no, es primo.
    analisis.es_primo = analisis.factor_primo == 0;

    // Probamos todas las bases que no comparten divisores con n.
    for (unsigned int base = 2; base < n; ++base) {
        if (std::gcd(base, n) != 1) continue;
        ++analisis.bases_coprimas;
        const unsigned int resto = PotenciaModularRapida(base, n - 1, n);
        if (resto == 1) {
            ++analisis.bases_superadas;
        } else if (analisis.base_testigo == 0) {
            analisis.base_testigo = base;
            analisis.resto_testigo = resto;
        }
    }

    // Los números de Carmichael son compuestos que satisfacen la congruencia para todas las bases coprimas.
    analisis.es_carmichael = n >= 561 && !analisis.es_primo &&
        analisis.bases_coprimas > 0 && analisis.bases_superadas == analisis.bases_coprimas;
    return analisis;
}

/** Resume el análisis como la proporción de bases que superan el test. */
double proporcion_bases_fermat(unsigned int n) {
    const auto analisis = AnalizarBasesFermat(n);
    if (n < 3) return 0.0;
    return static_cast<double>(analisis.bases_superadas) / analisis.bases_coprimas;
}

/** Decide si n es de Carmichael usando el análisis exhaustivo. */
bool es_carmichael(unsigned int n) {
    if (n < 561) {
        ValidarLimiteAnalisis(n);
        return false;
    }
    return AnalizarBasesFermat(n).es_carmichael;
}
