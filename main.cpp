#include "Lista_de_compras.h"
#include "Similaridade.h"
#include "Recomendacao.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    ListaCompras dados;
    DadosSimilaridade sim;

    if (!carregarPasta(dados, "dados")) {
        cout << "Erro ao carregar os arquivos CSV.\n";
        return 1;
    }

    cout << "Base carregada com sucesso!\n";
    cout << "Total de clientes: " << dados.clientes.size() << "\n";
    cout << "Total de produtos: " << dados.produtos.size() << "\n";

    construirMatrizCompras(dados, sim);

    string clientesTeste[] = {
        "99CL9Y01",
        "9O6OSM01",
        "99EF7201"
    };

    int k = 5;

    for (string codigo : clientesTeste) {
        int indice = buscarCliente(dados, codigo);

        if (indice == -1) {
            cout << "\nCliente " << codigo << " nao encontrado.\n";
            continue;
        }

        calcularSimilaridades(dados, sim, indice);

        vector<ProdutoRecomendado> recomendacoes =
            recomendarProdutos(dados, sim, indice, k);

        cout << "\n----------------------------------\n";
        cout << "Cliente: " << codigo << "\n";
        cout << "Produtos recomendados:\n";

        if (recomendacoes.empty()) {
            cout << "Nenhuma recomendacao encontrada.\n";
        }

        for (const ProdutoRecomendado &produto : recomendacoes) {
            cout << "- " << dados.produtos[produto.indiceProduto]
                 << " (ranking: " << fixed << setprecision(10)
                 << produto.ranking << ")\n";
        }
    }

    return 0;
}