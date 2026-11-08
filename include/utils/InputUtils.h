#ifndef PRIMOS_INPUTUTILS_H
#define PRIMOS_INPUTUTILS_H

#include <string>

/** Estado obtenido al interpretar un entero sin signo desde texto. */
enum class EstadoEntradaNumerica {
    Valido,
    Vacio,
    Invalido,
    FueraDeRango,
};

/**
 * Convierte texto en un entero sin signo.
 * @param texto Texto decimal que se quiere convertir.
 * @param numero Recibe el valor convertido si la conversión tiene éxito.
 * @return true si todo el texto representa un valor válido; false en caso contrario.
 */
bool ConvertirAEnteroSinSigno(const std::string& texto, unsigned int& numero);

/**
 * Interpreta texto como un entero sin signo e informa del motivo si no es válido.
 * @param texto Texto decimal que se quiere interpretar.
 * @param numero Recibe el valor convertido si la conversión tiene éxito.
 * @return Estado que indica si el texto está vacío, es inválido, excede el rango o es válido.
 */
EstadoEntradaNumerica AnalizarEnteroSinSigno(const std::string& texto, unsigned int& numero);

#endif //PRIMOS_INPUTUTILS_H
