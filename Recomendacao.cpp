#include "Recomendacao.h"
#include <algorithm>
#include <cmath>

using namespace std;

vector<ProdutoRecomendado> recomendarProdutos(
    const ListaCompras &dados,
    const DadosSimilaridade &sim,
    int indiceCliente,
    int k
) {
    vector<ProdutoRecomendado> recomendacoes;

    if (indiceCliente < 0 ||
        indiceCliente >= (int)dados.clientes.size() ||
        indiceCliente >= (int)sim.similaridades.size() ||
        k <= 0) {
        return recomendacoes;
    }

    vector<double> logRanking(dados.produtos.size(), 0.0);
    vector<bool> tocado(dados.produtos.size(), false);
    vector<bool> jaComprou(dados.produtos.size(), false);

    for (int produto : dados.compras[indiceCliente]) {
        jaComprou[produto] = true;
    }

    for (int i = 0; i < (int)dados.clientes.size(); i++) {
        if (i == indiceCliente || sim.similaridades[i] >= 1.0) {
            continue;
        }

        double logDistancia = log(sim.similaridades[i]);

        for (int produto : dados.compras[i]) {
            if (!jaComprou[produto]) {
                logRanking[produto] += logDistancia;
                tocado[produto] = true;
            }
        }
    }

    vector<pair<double, int>> ordem;

    for (int i = 0; i < (int)logRanking.size(); i++) {
        if (!jaComprou[i] && tocado[i]) {
            ordem.push_back({logRanking[i], i});
        }
    }

    sort(ordem.begin(), ordem.end());

    if ((int)ordem.size() > k) {
        ordem.resize(k);
    }

    for (const auto &item : ordem) {
        recomendacoes.push_back({item.second, exp(item.first)});
    }

    return recomendacoes;
}
