#include "Lista_de_compras.h"
#include "Similaridade.h"
#include "Recomendacao.h"
#include "MatrizEsparsa.h"
#include <iostream>

using namespace std;

int main() {
    ListaCompras dados;

    if (!carregarPasta(dados, "dados")) {
        cout << "Erro ao carregar os arquivos CSV.\n";
        return 1;
    }

    cout << "Base carregada com sucesso!\n"
         << "Total de clientes: " << dados.clientes.size() << "\n"
         << "Total de produtos: " << dados.produtos.size() << "\n";

    MatrizCSR compras = converterComprasParaCSR(dados);

    exibirComparacaoMemoria(compras);

    cout << "\nConstruindo matriz de intersecoes CSR em disco...\n";

    if (!multiplicarCSRParaDisco(compras, "intersecoes")) {
        cout << "Falha na construcao da matriz CSR.\n";
        return 1;
    }

    LeitorCSRDisco leitor;

    if (!leitor.abrir("intersecoes")) {
        cout << "Erro ao abrir a matriz gravada.\n";
        return 1;
    }

    cout << "\n--- A3: Recomendacoes para tres clientes ---\n";

    int clientesDemo[3] = {0, 1000, 2000};

    for (int indiceCliente : clientesDemo) {
        vector<uint16_t> colunas, valores;

        if (!leitor.lerLinha(indiceCliente, colunas, valores)) {
            cout << "Erro ao ler a linha do cliente " << indiceCliente << ".\n";
            continue;
        }

        DadosSimilaridade sim;
        sim.similaridades = distanciasDaLinha(compras, indiceCliente, colunas, valores);

        cout << "\nCliente: " << dados.clientes[indiceCliente] << "\n";

        int similar = clienteMaisSimilar(sim, indiceCliente);

        if (similar >= 0) {
            cout << "Cliente mais similar: " << dados.clientes[similar] << "\n";
        } else {
            cout << "Nenhum cliente similar encontrado.\n";
        }

        cout << "Produtos recomendados:\n";

        for (const auto &p : recomendarProdutos(dados, sim, indiceCliente, 5)) {
            cout << "- " << dados.produtos[p.indiceProduto] << " (ranking: " << p.ranking << ")\n";
        }
    }

    cout << "\nProcessamento concluido!\n";

    return 0;
}
