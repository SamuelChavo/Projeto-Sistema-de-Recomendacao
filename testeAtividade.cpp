#include "Similaridade.h"
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

int main() {
    const vector<int> tamanhos = {40, 80, 120, 160};
    const int produtos = 100;

    cout << "Comparacao da multiplicacao padrao e otimizada (A4)\n";
    cout << "Clientes | Padrao (ms) | Otimizado (ms) | Resultados iguais\n";

    for (int n : tamanhos) {
        vector<vector<unsigned char>> matriz(
            n, vector<unsigned char>(produtos, 0)
        );

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < produtos; j++) {
                matriz[i][j] = ((i * 17 + j * 13) % 11 < 2) ? 1 : 0;
            }
        }

        auto inicioPadrao = steady_clock::now();
        auto padrao = multiplicarPadrao(matriz);
        auto fimPadrao = steady_clock::now();

        auto inicioOtimizado = steady_clock::now();
        auto otimizado = multiplicarOtimizado(matriz);
        auto fimOtimizado = steady_clock::now();

        bool iguais = padrao == otimizado;

        double tempoPadrao =
            duration<double, milli>(fimPadrao - inicioPadrao).count();

        double tempoOtimizado =
            duration<double, milli>(fimOtimizado - inicioOtimizado).count();

        cout << n << " x " << produtos << " | "
             << tempoPadrao << " | "
             << tempoOtimizado << " | "
             << (iguais ? "SIM" : "NAO") << "\n";

        if (!iguais) {
            return 1;
        }
    }

    return 0;
}