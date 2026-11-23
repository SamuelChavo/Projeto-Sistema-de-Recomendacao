#include "Lista_de_compras.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

int buscarCliente(const ListaCompras &dados, const string &codigoCliente) {
    auto it = dados.mapaClientes.find(codigoCliente);

    if (it == dados.mapaClientes.end()) {
        return -1;
    }

    return it->second;
}

bool carregarArquivo(ListaCompras &dados, const string &caminho) {
    ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        return false;
    }

    string linha;
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
        if (linha.empty()) {
            continue;
        }

        vector<string> campos;
        string campo;
        bool aspas = false;

        for (size_t i = 0; i < linha.size(); i++) {
            char c = linha[i];

            if (c == '"') {
                if (aspas && i + 1 < linha.size() && linha[i + 1] == '"') {
                    campo += '"';
                    i++;
                } else {
                    aspas = !aspas;
                }
            } else if (c == ',' && !aspas) {
                campos.push_back(campo);
                campo.clear();
            } else {
                campo += c;
            }
        }

        campos.push_back(campo);

        if (campos.size() < 4) {
            continue;
        }

        string codigoCliente = campos[1];
        string codigoProduto = campos[2];
        string nomeProduto = campos[3];

        if (!codigoCliente.empty() && codigoCliente.back() == '\r') {
            codigoCliente.pop_back();
        }

        if (!codigoProduto.empty() && codigoProduto.back() == '\r') {
            codigoProduto.pop_back();
        }

        while (!nomeProduto.empty() &&
               (nomeProduto.back() == '\r' || nomeProduto.back() == ' ')) {
            nomeProduto.pop_back();
        }

        if (codigoCliente.empty() || codigoProduto.empty()) {
            continue;
        }

        int indiceCliente;

        auto cliente = dados.mapaClientes.find(codigoCliente);

        if (cliente == dados.mapaClientes.end()) {
            indiceCliente = dados.clientes.size();
            dados.mapaClientes[codigoCliente] = indiceCliente;
            dados.clientes.push_back(codigoCliente);
            dados.compras.push_back(list<int>());
        } else {
            indiceCliente = cliente->second;
        }

        int indiceProduto;

        auto produto = dados.mapaProdutos.find(codigoProduto);

        if (produto == dados.mapaProdutos.end()) {
            indiceProduto = dados.produtos.size();
            dados.mapaProdutos[codigoProduto] = indiceProduto;
            dados.produtos.push_back(nomeProduto);
        } else {
            indiceProduto = produto->second;
        }

        auto &lista = dados.compras[indiceCliente];

        if (find(lista.begin(), lista.end(), indiceProduto) == lista.end()) {
            lista.push_back(indiceProduto);
        }
    }

    arquivo.close();
    return true;
}

bool carregarPasta(ListaCompras &dados, const string &pasta) {
    bool carregouArquivo = false;

    for (int i = 0; i < 21; i++) {
        string caminho = pasta + "/dados_venda_cluster_" +
                         to_string(i) + ".csv";

        if (carregarArquivo(dados, caminho)) {
            carregouArquivo = true;
        }
    }

    return carregouArquivo;
}

void exibirComprasCliente(const ListaCompras &dados, const string &codigoCliente) {
    int indice = buscarCliente(dados, codigoCliente);

    if (indice == -1) {
        cout << "Cliente nao encontrado.\n";
        return;
    }

    cout << "\nCliente: " << codigoCliente << "\n";
    cout << "Produtos comprados:\n";

    for (int produto : dados.compras[indice]) {
        cout << "- " << dados.produtos[produto] << "\n";
    }
}