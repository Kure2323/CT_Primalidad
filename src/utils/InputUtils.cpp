#include "../../include/utils/InputUtils.h"
#include <charconv>
#include <system_error>

bool ConvertirAEnteroSinSigno(const std::string& texto, unsigned int& numero) {
    return AnalizarEnteroSinSigno(texto, numero) == EstadoEntradaNumerica::Valido;
}

EstadoEntradaNumerica AnalizarEnteroSinSigno(const std::string& texto, unsigned int& numero) {
    if (texto.empty()) return EstadoEntradaNumerica::Vacio;
    unsigned int valor;
    auto [ptr, ec] = std::from_chars(
        texto.data(),
        texto.data() + texto.size(),
        valor
    );
    if (ec == std::errc::result_out_of_range) return EstadoEntradaNumerica::FueraDeRango;
    if (ec != std::errc{} || ptr != texto.data() + texto.size()) return EstadoEntradaNumerica::Invalido;
    numero = valor;
    return EstadoEntradaNumerica::Valido;
}
