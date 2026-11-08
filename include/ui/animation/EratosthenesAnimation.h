#ifndef PRIMOS_ERATOSTHENESANIMATION_H
#define PRIMOS_ERATOSTHENESANIMATION_H

#include <memory>
#include <functional>
#include <vector>
#include "ftxui/component/component.hpp"

/**
 * Estado compartido del marcado y la cuadrícula de la criba animada.
 *
 * La pantalla prepara el límite y el vector de compuestos antes de iniciar
 * el cálculo. La animación actualiza los múltiplos y notifica el resultado.
 */
struct EstadoAnimacionEratostenes {
    unsigned int limite = 30;                                ///< Mayor número que se muestra y analiza.
    std::vector<bool> compuesto;                             ///< Marca los números que no son primos.
    unsigned int primo_actual = 2;                           ///< Primo cuyos múltiplos se están marcando.
    unsigned int multiplo_actual = 0;                        ///< Próximo múltiplo que se marcará; cero inicia otro primo.
    unsigned int pagina_actual = 0;                           ///< Página visible de la cuadrícula.
    int velocidad = 60;                                      ///< Velocidad elegida por el usuario, entre 1 y 100.
    bool algoritmo_terminado = false;                        ///< Indica que la criba ha terminado.
    bool calculado = false;                                  ///< Indica que el vector de compuestos está inicializado.
    bool animando = false;                                   ///< Indica que quedan múltiplos por marcar.
    std::function<void(const std::vector<int>&)> al_terminar; ///< Recibe la lista final de primos.
};

/**
 * Crea la cuadrícula que muestra el marcado progresivo de la criba.
 * @param estado Estado compartido de cálculo, paginación y velocidad.
 * @return Componente FTXUI interactivo con la cuadrícula animada.
 */
ftxui::Component AnimacionEratostenes(std::shared_ptr<EstadoAnimacionEratostenes> estado);

#endif
