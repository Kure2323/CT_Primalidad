#ifndef PRIMOS_CARMICHAELANIMATION_H
#define PRIMOS_CARMICHAELANIMATION_H

#include <functional>
#include <memory>

#include "ftxui/component/component.hpp"

/**
 * Estado compartido entre la pantalla y la animación de Carmichael.
 *
 * La pantalla inicializa los datos antes de activar la animación y recibe
 * notificaciones cuando se procesa una base o termina el análisis.
 */
struct EstadoAnimacionCarmichael {
    unsigned int numero = 561;              ///< Número analizado.
    unsigned int siguiente_base = 2;         ///< Próxima base candidata.
    unsigned int base_actual = 0;            ///< Última base coprima procesada.
    unsigned int bases_totales = 0;           ///< Cantidad de bases coprimas del intervalo.
    unsigned int bases_revisadas = 0;         ///< Bases coprimas procesadas hasta el momento.
    unsigned int bases_superadas = 0;         ///< Bases que satisfacen la congruencia de Fermat.
    unsigned int bases_fallidas = 0;          ///< Bases que no satisfacen la congruencia.
    unsigned int base_testigo = 0;             ///< Primera base que demuestra el fallo, si existe.
    unsigned int resto_testigo = 0;            ///< Resto obtenido con la base testigo.
    unsigned int factor_primo = 0;             ///< Factor usado para identificar que el número es compuesto.
    bool es_primo = false;                     ///< Indica si el número analizado es primo.
    bool ultima_superada = false;              ///< Resultado de la última base procesada.
    bool animando = false;                     ///< Indica si quedan bases por procesar.
    bool terminado = false;                    ///< Indica si el análisis llegó a su fin.
    std::function<void()> al_progresar;        ///< Notificación tras procesar una base.
    std::function<void()> al_terminar;         ///< Notificación al finalizar el análisis.
};

/**
 * Crea el componente que prueba progresivamente las bases coprimas con el número.
 * @param estado Estado compartido que la pantalla configura y presenta.
 * @return Componente FTXUI que actualiza el análisis mediante fotogramas.
 */
ftxui::Component AnimacionCarmichael(std::shared_ptr<EstadoAnimacionCarmichael> estado);

#endif
