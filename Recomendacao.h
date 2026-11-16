#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include "Similaridade.h"

struct ProdutoRecomendado {
    int indiceProduto;
    double ranking;
};

vector<ProdutoRecomendado> recomendarProdutos(
    const ListaCompras &dados,
    const DadosSimilaridade &sim,
    int indiceCliente,
    int k
);

#endif