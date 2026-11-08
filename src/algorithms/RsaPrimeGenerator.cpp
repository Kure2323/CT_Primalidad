#include "../../include/algorithms/RsaPrimeGenerator.h"

#include <boost/multiprecision/miller_rabin.hpp>
#include <array>
#include <limits>
#include <random>
#include <stdexcept>
#include <utility>

using boost::multiprecision::cpp_int;

/** Genera un candidato impar del tamaño indicado. */
static cpp_int GenerarCandidato(unsigned int bits, std::random_device& fuente) {
    cpp_int candidato = 0;
    std::random_device::result_type bloque = 0;
    unsigned int bits_restantes = 0;
    const unsigned int bits_por_bloque = std::numeric_limits<std::random_device::result_type>::digits;

    for (unsigned int bit = 0; bit < bits; ++bit) {
        if (bits_restantes == 0) {
            bloque = fuente();
            bits_restantes = bits_por_bloque;
        }
        // Consumir un bit por iteración permite generar candidatos de más bits que el tipo de la fuente aleatoria.
        candidato <<= 1;
        candidato |= bloque & 1u;
        bloque >>= 1;
        --bits_restantes;
    }
    candidato |= cpp_int(1) << (bits - 1);
    // El bit superior fija el tamaño solicitado y el inferior garantiza que el candidato sea impar.
    candidato |= 1;
    return candidato;
}

/** Comprueba si el candidato tiene factores primos pequeños. */
static bool TieneFactorPequeno(const cpp_int& candidato) {
    constexpr std::array<unsigned int, 15> factores = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
    };
    for (const auto factor : factores) {
        if (candidato != factor && candidato % factor == 0) return true;
    }
    return false;
}

/** Convierte el primo probable a texto decimal para mostrarlo. */
std::string ConvertirADecimal(const cpp_int& numero) {
    return numero.convert_to<std::string>();
}

/** Genera candidatos hasta que uno supera el filtro y Miller-Rabin. */
PrimoRsaGenerado GenerarPrimoTamanoRsa(unsigned int bits, unsigned int rondas) {
    constexpr unsigned int limite_maximo_bits = 4'096;
    if (bits < 16 || bits > limite_maximo_bits)
        throw std::invalid_argument("El tamaño debe estar entre 16 y " + std::to_string(limite_maximo_bits) + " bits.");
    if (rondas == 0 || rondas > 64) throw std::invalid_argument("Usa entre 1 y 64 rondas.");

    std::random_device rd;
    PrimoRsaGenerado resultado;
    for (;;) {
        ++resultado.candidatos_probados;
        cpp_int candidato = GenerarCandidato(bits, rd);
        if (TieneFactorPequeno(candidato)) {
            ++resultado.descartados_por_factor_pequeno;
            continue;
        }
        if (!boost::multiprecision::miller_rabin_test(candidato, rondas, rd)) {
            ++resultado.descartados_por_miller_rabin;
            continue;
        }
        resultado.primo = std::move(candidato);
        return resultado;
    }
}
