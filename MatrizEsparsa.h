#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

#include <vector>
#include <cstddef>
#include "Lista_de_compras.h"

using namespace std;

struct MatrizCSR {
    int linhas;
    int colunas;
    vector<int> values;
    vector<int> col_index;
    vector<int> row_ptr;
};

MatrizCSR converterComprasParaCSR(const ListaCompras &dados);

MatrizCSR multiplicarLinhaCSR(const MatrizCSR &matriz, int linha);

vector<double> calcularDistanciasCSR(
    const MatrizCSR &compras,
    const MatrizCSR &intersecoes,
    int indiceCliente
);

size_t memoriaCSR(const MatrizCSR &matriz);

size_t memoriaDensa(int linhas, int colunas, size_t tamanhoElemento);

void exibirComparacaoMemoria(
    const MatrizCSR &compras,
    const MatrizCSR &intersecoes
);

#endif
