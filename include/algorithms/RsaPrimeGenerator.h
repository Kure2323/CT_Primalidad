#ifndef PRIMOS_RSA_PRIME_GENERATOR_H
#define PRIMOS_RSA_PRIME_GENERATOR_H

#include <boost/multiprecision/cpp_int.hpp>
#include <string>

/** Primo probable generado y resumen de los candidatos descartados. */
struct PrimoRsaGenerado {
    boost::multiprecision::cpp_int primo;
    unsigned long long candidatos_probados = 0;
    unsigned long long descartados_por_factor_pequeno = 0;
    unsigned long long descartados_por_miller_rabin = 0;
};

/** Genera un primo probable del tamaño indicado tras las rondas de Miller-Rabin. */
PrimoRsaGenerado GenerarPrimoTamanoRsa(unsigned int bits, unsigned int rondas = 16);

/** Convierte un entero grande a su representación decimal. */
std::string ConvertirADecimal(const boost::multiprecision::cpp_int& numero);

#endif
