#include "MatrizEsparsa.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

MatrizCSR converterComprasParaCSR(const ListaCompras &dados) {
    MatrizCSR csr;
    csr.linhas = dados.clientes.size();
    csr.colunas = dados.produtos.size();
    csr.row_ptr.push_back(0);

    for (const auto &compra : dados.compras) {
        vector<int> produtos(compra.begin(), compra.end());

        sort(produtos.begin(), produtos.end());
        produtos.erase(unique(produtos.begin(), produtos.end()), produtos.end());

        for (int produto : produtos) {
            if (produto >= 0 && produto < csr.colunas) {
                csr.values.push_back(1);
                csr.col_index.push_back(produto);
            }
        }

        csr.row_ptr.push_back(csr.values.size());
    }

    return csr;
}

MatrizCSR multiplicarLinhaCSR(const MatrizCSR &matriz, int linha) {
    MatrizCSR resultado;
    resultado.linhas = 1;
    resultado.colunas = matriz.linhas;
    resultado.row_ptr.push_back(0);

    if (linha < 0 || linha >= matriz.linhas) {
        resultado.row_ptr.push_back(0);
        return resultado;
    }

    vector<vector<int>> clientesPorProduto(matriz.colunas);

    for (int cliente = 0; cliente < matriz.linhas; cliente++) {
        for (int p = matriz.row_ptr[cliente]; p < matriz.row_ptr[cliente + 1]; p++) {
            clientesPorProduto[matriz.col_index[p]].push_back(cliente);
        }
    }

    vector<int> contagem(matriz.linhas, 0);
    vector<int> tocados;

    for (int p = matriz.row_ptr[linha]; p < matriz.row_ptr[linha + 1]; p++) {
        int produto = matriz.col_index[p];

        for (int cliente : clientesPorProduto[produto]) {
            if (contagem[cliente] == 0) {
                tocados.push_back(cliente);
            }

            contagem[cliente]++;
        }
    }

    sort(tocados.begin(), tocados.end());

    for (int cliente : tocados) {
        resultado.values.push_back(contagem[cliente]);
        resultado.col_index.push_back(cliente);
    }

    resultado.row_ptr.push_back(resultado.values.size());

    return resultado;
}

vector<double> calcularDistanciasCSR(
    const MatrizCSR &compras,
    const MatrizCSR &intersecoes,
    int indiceCliente
) {
    vector<double> distancias(compras.linhas, 1.0);

    if (indiceCliente < 0 || indiceCliente >= compras.linhas ||
        intersecoes.linhas != 1) {
        return distancias;
    }

    int totalCompras =
        compras.row_ptr[indiceCliente + 1] -
        compras.row_ptr[indiceCliente];

    if (totalCompras == 0) {
        return distancias;
    }

    for (int p = intersecoes.row_ptr[0];
         p < intersecoes.row_ptr[1]; p++) {

        int cliente = intersecoes.col_index[p];

        if (cliente != indiceCliente) {
            distancias[cliente] =
                1.0 - (double)intersecoes.values[p] / totalCompras;
        }
    }

    return distancias;
}

size_t memoriaCSR(const MatrizCSR &csr) {
    return csr.values.size() * sizeof(int)
         + csr.col_index.size() * sizeof(int)
         + csr.row_ptr.size() * sizeof(int);
}

size_t memoriaDensa(int linhas, int colunas, size_t tamanhoElemento) {
    return (size_t)linhas * colunas * tamanhoElemento;
}

void exibirComparacaoMemoria(
    const MatrizCSR &compras,
    const MatrizCSR &intersecoes
) {
    size_t densa = memoriaDensa(
        compras.linhas,
        compras.colunas,
        sizeof(unsigned char)
    );

    size_t esparsa = memoriaCSR(compras);

    cout << fixed << setprecision(2)
         << "\n--- Uso de memoria ---\n"
         << "Compras - densa: " << densa / 1048576.0
         << " MB | CSR: " << esparsa / 1048576.0 << " MB\n"
         << "Uma linha de intersecoes CSR: "
         << memoriaCSR(intersecoes) / 1024.0 << " KB\n"
         << "Elementos nao nulos na linha: "
         << intersecoes.values.size() << "\n";

    if (densa > 0) {
        cout << "Reducao nas compras: "
             << 100.0 * (1.0 - (double)esparsa / densa)
             << "%\n";
    }
}