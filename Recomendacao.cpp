#include "Recomendacao.h"
#include <algorithm>

using namespace std;

vector<ProdutoRecomendado> recomendarProdutos(
    const ListaCompras &dados,
    const DadosSimilaridade &sim,
    int indiceCliente,
    int k
) {
    vector<ProdutoRecomendado> recomendacoes;

    if (indiceCliente < 0 ||
        indiceCliente >= dados.clientes.size() ||
        indiceCliente >= sim.similaridades.size() ||
        k <= 0) {
        return recomendacoes;
    }

    vector<double> ranking(dados.produtos.size(), 1.0);
    vector<bool> jaComprou(dados.produtos.size(), false);

    for (int produto : dados.compras[indiceCliente]) {
        jaComprou[produto] = true;
    }

    for (int i = 0; i < dados.clientes.size(); i++) {
        if (i == indiceCliente || sim.similaridades[i] >= 1.0) {
            continue;
        }

        for (int produto : dados.compras[i]) {
            if (!jaComprou[produto]) {
                ranking[produto] *= sim.similaridades[i];
            }
        }
    }

    for (int i = 0; i < ranking.size(); i++) {
        if (!jaComprou[i] && ranking[i] < 1.0) {
            ProdutoRecomendado produto;
            produto.indiceProduto = i;
            produto.ranking = ranking[i];
            recomendacoes.push_back(produto);
        }
    }

    sort(
        recomendacoes.begin(),
        recomendacoes.end(),
        [](const ProdutoRecomendado &a, const ProdutoRecomendado &b) {
            return a.ranking < b.ranking;
        }
    );

    if (recomendacoes.size() > k) {
        recomendacoes.resize(k);
    }

    return recomendacoes;
}