#include "Lista_de_compras.h"
#include "Similaridade.h"
#include "Recomendacao.h"
#include "MatrizEsparsa.h"
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

    cout << "Base carregada com sucesso!\n"
         << "Total de clientes: " << dados.clientes.size() << "\n"
         << "Total de produtos: " << dados.produtos.size() << "\n";

    cout << "\nConstruindo matriz de compras CSR...\n";

    MatrizCSR compras = converterComprasParaCSR(dados);
    MatrizCSR linhaTeste = multiplicarLinhaCSR(compras, 0);

    exibirComparacaoMemoria(compras, linhaTeste);

    const string clientesTeste[] = {
        "99CL9Y01",
        "9O6OSM01",
        "99EF7201"
    };

    const int k = 5;

    for (const string &codigo : clientesTeste) {
        int indice = buscarCliente(dados, codigo);

        if (indice == -1) {
            cout << "\nCliente " << codigo << " nao encontrado.\n";
            continue;
        }

        cout << "\nCalculando intersecoes CSR...\n";

        MatrizCSR intersecoes = multiplicarLinhaCSR(compras, indice);

        sim.similaridades = calcularDistanciasCSR(
            compras, intersecoes, indice
        );

        int vizinho = clienteMaisSimilar(sim, indice);

        cout << "\n----------------------------------\n"
             << "Cliente: " << codigo << "\n";

        if (vizinho != -1) {
            cout << "Cliente mais similar: "
                 << dados.clientes[vizinho] << "\n";
        }

        cout << "Produtos recomendados:\n";

        for (const auto &produto : recomendarProdutos(dados, sim, indice, k)) {
            cout << "- " << dados.produtos[produto.indiceProduto]
                 << " (ranking: " << fixed << setprecision(10)
                 << produto.ranking << ")\n";
        }
    }

    return 0;
}