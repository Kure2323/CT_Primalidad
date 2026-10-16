#include "Eratosthenes.h"
#include <vector>
//
// Created by polete on 10/14/26.
//
std::vector<int> Eratosthenes(const unsigned int n) {
    if (n < 2) return std::vector<int>();
    // Vector completo de true
    std::vector _primos(n+1, true);
    _primos[1] = false;

    for (int i = 2; i*i <= n; i++) {
        if (_primos[i] == false) {
            continue;
        }

        for (int j = 2; i*j <= n; j++) {
            _primos[i*j] = false;
        }
    }

    std::vector<int> primos;
    for (int i = 1; i <= n; i++) {
        if (_primos[i] == true) {
            primos.push_back(i);
        }
    }

    return primos;
}