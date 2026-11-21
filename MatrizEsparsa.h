
#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

#include <vector>
#include <cstddef>

using namespace std;

struct MatrizCSR {
    int linhas;
    int colunas;

    vector<int> values;
    vector<int> col_index;
    vector<int> row_ptr;
};

MatrizCSR converterParaCSR(
    const vector<vector<unsigned char>> &matriz
);

int produtoInternoCSR(
    const MatrizCSR &matriz,
    int linhaA,
    int linhaB
);

MatrizCSR multiplicarCSR(
    const MatrizCSR &matriz
);

int obterValorCSR(
    const MatrizCSR &matriz,
    int linha,
    int coluna
);

vector<double> calcularDistanciasCSR(
    const MatrizCSR &compras,
    const MatrizCSR &intersecoes,
    int indiceCliente
);

size_t memoriaCSR(const MatrizCSR &matriz);

#endif
