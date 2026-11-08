#include "../../include/utils/RandomUtils.h"

#include <random>
#include <stdexcept>

unsigned int EnteroAleatorio(unsigned int minimo, unsigned int maximo) {
    if (minimo > maximo) throw std::invalid_argument("El rango aleatorio no es válido.");
    // Se reutiliza un generador por hilo para no inicializarlo en cada llamada.
    static thread_local std::mt19937 generador(std::random_device{}());
    return std::uniform_int_distribution<unsigned int>(minimo, maximo)(generador);
}
