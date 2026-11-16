#include "Similaridade.h"
#include <iostream>

using namespace std;

void construirMatrizCompras(const ListaCompras &dados, DadosSimilaridade &sim) {
    int quantidadeClientes = dados.clientes.size();
    int quantidadeProdutos = dados.produtos.size();

    sim.matrizCompras.assign(
        quantidadeClientes,
        vector<unsigned char>(quantidadeProdutos, 0)
    );

    for (int i = 0; i < quantidadeClientes; i++) {
        for (int produto : dados.compras[i]) {
            sim.matrizCompras[i][produto] = 1;
        }
    }
}

void calcularSimilaridades(
    const ListaCompras &dados,
    DadosSimilaridade &sim,
    int indiceCliente
) {
    int quantidadeClientes = dados.clientes.size();

    sim.similaridades.assign(quantidadeClientes, 1.0);

    if (indiceCliente < 0 || indiceCliente >= quantidadeClientes) {
        return;
    }

    int totalCompras = dados.compras[indiceCliente].size();

    if (totalCompras == 0) {
        return;
    }

    for (int i = 0; i < quantidadeClientes; i++) {
        if (i == indiceCliente) {
            continue;
        }

        int intersecao = 0;

        for (int j = 0; j < sim.matrizCompras[i].size(); j++) {
            if (sim.matrizCompras[indiceCliente][j] == 1 &&
                sim.matrizCompras[i][j] == 1) {
                intersecao++;
            }
        }

        sim.similaridades[i] =
            1.0 - (double) intersecao / totalCompras;
    }
}

int clienteMaisSimilar(const DadosSimilaridade &sim, int indiceCliente) {
    int maisSimilar = -1;
    double menorSimilaridade = 1.0;

    for (int i = 0; i < sim.similaridades.size(); i++) {
        if (i == indiceCliente) {
            continue;
        }

        if (sim.similaridades[i] < menorSimilaridade) {
            menorSimilaridade = sim.similaridades[i];
            maisSimilar = i;
        }
    }

    return maisSimilar;
}