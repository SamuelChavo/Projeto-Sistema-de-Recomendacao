
#include "../Similaridade.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

int main() {
    vector<vector<unsigned char>> matriz = {
        {1, 1, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 0},
        {1, 1, 0, 1, 0}
    };

    auto inicioPadrao = high_resolution_clock::now();

    auto padrao = multiplicarPadrao(matriz);

    auto fimPadrao = high_resolution_clock::now();

    auto inicioOtimizado = high_resolution_clock::now();

    auto otimizado = multiplicarOtimizado(matriz);

    auto fimOtimizado = high_resolution_clock::now();

    bool iguais = padrao == otimizado;

    cout << "Matrizes iguais: "
         << (iguais ? "SIM" : "NAO") << "\n";

    cout << "Tempo padrao: "
         << duration<double, milli>(
                fimPadrao - inicioPadrao
            ).count()
         << " ms\n";

    cout << "Tempo otimizado: "
         << duration<double, milli>(
                fimOtimizado - inicioOtimizado
            ).count()
         << " ms\n";

    for (const auto &linha : padrao) {
        for (int valor : linha) {
            cout << valor << " ";
        }

        cout << "\n";
    }

    return iguais ? 0 : 1;
}
