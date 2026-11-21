#include "Similaridade.h"

using namespace std;

void construirMatrizCompras(
    const ListaCompras &dados,
    DadosSimilaridade &sim
) {
    int n = dados.clientes.size();
    int m = dados.produtos.size();

    sim.matrizCompras.assign(n, vector<unsigned char>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int produto : dados.compras[i]) {
            if (produto >= 0 && produto < m) {
                sim.matrizCompras[i][produto] = 1;
            }
        }
    }
}

vector<vector<int>> multiplicarPadrao(
    const vector<vector<unsigned char>> &matriz
) {
    int n = matriz.size();

    vector<vector<int>> resultado(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;

            for (int k = 0; k < matriz[i].size(); k++) {
                soma += matriz[i][k] * matriz[j][k];
            }

            resultado[i][j] = soma;
        }
    }

    return resultado;
}

vector<vector<int>> multiplicarOtimizado(
    const vector<vector<unsigned char>> &matriz
) {
    int n = matriz.size();

    vector<vector<int>> resultado(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int soma = 0;

            for (int k = 0; k < matriz[i].size(); k++) {
                soma += matriz[i][k] * matriz[j][k];
            }

            resultado[i][j] = soma;
            resultado[j][i] = soma;
        }
    }

    return resultado;
}

void calcularMatrizIntersecao(
    DadosSimilaridade &sim,
    bool otimizado
) {
    if (otimizado) {
        sim.matrizIntersecao = multiplicarOtimizado(sim.matrizCompras);
    } else {
        sim.matrizIntersecao = multiplicarPadrao(sim.matrizCompras);
    }
}

void calcularSimilaridades(
    const ListaCompras &dados,
    DadosSimilaridade &sim,
    int indiceCliente
) {
    int n = dados.clientes.size();

    sim.similaridades.assign(n, 1.0);

    if (indiceCliente < 0 || indiceCliente >= n) {
        return;
    }

    if (sim.matrizIntersecao.size() != (size_t)n) {
        calcularMatrizIntersecao(sim, false);
    }

    int totalCompras = dados.compras[indiceCliente].size();

    if (totalCompras == 0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        if (i == indiceCliente) {
            continue;
        }

        int intersecao = sim.matrizIntersecao[indiceCliente][i];

        sim.similaridades[i] =
            1.0 - (double)intersecao / totalCompras;
    }
}

int clienteMaisSimilar(
    const DadosSimilaridade &sim,
    int indiceCliente
) {
    if (indiceCliente < 0 ||
        indiceCliente >= (int)sim.similaridades.size()) {
        return -1;
    }

    int maisSimilar = -1;
    double menorDistancia = 1.0;

    for (int i = 0; i < sim.similaridades.size(); i++) {
        if (i == indiceCliente) {
            continue;
        }

        if (sim.similaridades[i] < menorDistancia) {
            menorDistancia = sim.similaridades[i];
            maisSimilar = i;
        }
    }

    return maisSimilar;
}
