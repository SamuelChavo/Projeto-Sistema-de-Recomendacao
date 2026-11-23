#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "Lista_de_compras.h"
#include <vector>

using namespace std;

struct DadosSimilaridade {
    vector<vector<unsigned char>> matrizCompras;
    vector<vector<int>> matrizIntersecao;
    vector<double> similaridades;
};

void construirMatrizCompras(const ListaCompras &dados, DadosSimilaridade &sim);

vector<vector<int>> multiplicarPadrao(
    const vector<vector<unsigned char>> &matriz
);

vector<vector<int>> multiplicarOtimizado(
    const vector<vector<unsigned char>> &matriz
);

void calcularMatrizIntersecao(
    DadosSimilaridade &sim,
    bool otimizado
);

void calcularSimilaridades(
    const ListaCompras &dados,
    DadosSimilaridade &sim,
    int indiceCliente
);

int clienteMaisSimilar(
    const DadosSimilaridade &sim,
    int indiceCliente
);

#endif
