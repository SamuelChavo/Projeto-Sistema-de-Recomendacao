#include "MatrizEsparsa.h"
#include <algorithm>
#include <climits>
#include <iostream>

using namespace std;

MatrizCSR converterComprasParaCSR(const ListaCompras &dados) {
    MatrizCSR matriz;
    matriz.linhas = dados.clientes.size();
    matriz.colunas = dados.produtos.size();
    matriz.row_ptr.assign(matriz.linhas + 1, 0);

    for (int i = 0; i < matriz.linhas; i++) {
        vector<int> produtos(dados.compras[i].begin(), dados.compras[i].end());
        sort(produtos.begin(), produtos.end());

        for (int produto : produtos) {
            matriz.col_index.push_back(produto);
            matriz.values.push_back(1);
        }

        matriz.row_ptr[i + 1] = matriz.col_index.size();
    }

    return matriz;
}

bool multiplicarCSRParaDisco(const MatrizCSR &matriz, const string &prefixoArquivo) {
    if (matriz.linhas > 65536) {
        cout << "Clientes demais para col_index uint16_t.\n";
        return false;
    }

    ofstream arquivoValues(prefixoArquivo + "_values.bin", ios::binary);
    ofstream arquivoColunas(prefixoArquivo + "_col_index.bin", ios::binary);
    ofstream arquivoLinhas(prefixoArquivo + "_row_ptr.bin", ios::binary);

    if (!arquivoValues || !arquivoColunas || !arquivoLinhas) {
        cout << "Erro ao criar os arquivos CSR.\n";
        return false;
    }

    vector<vector<int>> clientesPorProduto(matriz.colunas);

    for (int cliente = 0; cliente < matriz.linhas; cliente++) {
        for (int p = matriz.row_ptr[cliente]; p < matriz.row_ptr[cliente + 1]; p++) {
            clientesPorProduto[matriz.col_index[p]].push_back(cliente);
        }
    }

    vector<int> contagem(matriz.linhas, 0);
    vector<int> tocados;
    vector<uint16_t> valoresLinha, colunasLinha;
    vector<int> row_ptr(matriz.linhas + 1, 0);
    long long total = 0;

    for (int i = 0; i < matriz.linhas; i++) {
        tocados.clear();

        for (int p = matriz.row_ptr[i]; p < matriz.row_ptr[i + 1]; p++) {
            for (int cliente : clientesPorProduto[matriz.col_index[p]]) {
                if (contagem[cliente]++ == 0) {
                    tocados.push_back(cliente);
                }
            }
        }

        sort(tocados.begin(), tocados.end());
        valoresLinha.clear();
        colunasLinha.clear();

        for (int cliente : tocados) {
            valoresLinha.push_back(contagem[cliente]);
            colunasLinha.push_back(cliente);
            contagem[cliente] = 0;
        }

        streamsize bytes = valoresLinha.size() * sizeof(uint16_t);
        arquivoValues.write(reinterpret_cast<const char *>(valoresLinha.data()), bytes);
        arquivoColunas.write(reinterpret_cast<const char *>(colunasLinha.data()), bytes);

        total += tocados.size();

        if (total > INT_MAX || !arquivoValues || !arquivoColunas) {
            cout << "Erro ao gravar a matriz na linha " << i << ".\n";
            return false;
        }

        row_ptr[i + 1] = total;

        if (i % 5000 == 0) {
            cout << "Linhas processadas: " << i << " / " << matriz.linhas
                 << " | Elementos gravados: " << total << "\n";
        }
    }

    arquivoLinhas.write(reinterpret_cast<const char *>(row_ptr.data()), row_ptr.size() * sizeof(int));

    cout << "\nMatriz CSR gravada em disco com sucesso!\n";
    cout << "Elementos nao nulos: " << total << "\n";

    return static_cast<bool>(arquivoLinhas);
}

bool LeitorCSRDisco::abrir(const string &prefixoArquivo) {
    arquivoValues.open(prefixoArquivo + "_values.bin", ios::binary);
    arquivoColunas.open(prefixoArquivo + "_col_index.bin", ios::binary);
    ifstream arquivoLinhas(prefixoArquivo + "_row_ptr.bin", ios::binary | ios::ate);

    if (!arquivoValues || !arquivoColunas || !arquivoLinhas) {
        return false;
    }

    streamoff bytes = arquivoLinhas.tellg();
    row_ptr.resize(bytes / sizeof(int));
    arquivoLinhas.seekg(0);
    arquivoLinhas.read(reinterpret_cast<char *>(row_ptr.data()), bytes);

    return row_ptr.size() > 1 && arquivoLinhas;
}

bool LeitorCSRDisco::lerLinha(int linha, vector<uint16_t> &colunas, vector<uint16_t> &valores) {
    if (linha < 0 || linha + 1 >= (int)row_ptr.size()) {
        return false;
    }

    int quantidade = row_ptr[linha + 1] - row_ptr[linha];
    streamoff inicio = (streamoff)row_ptr[linha] * sizeof(uint16_t);
    streamsize bytes = quantidade * sizeof(uint16_t);

    colunas.resize(quantidade);
    valores.resize(quantidade);

    arquivoColunas.clear();
    arquivoValues.clear();
    arquivoColunas.seekg(inicio);
    arquivoValues.seekg(inicio);
    arquivoColunas.read(reinterpret_cast<char *>(colunas.data()), bytes);
    arquivoValues.read(reinterpret_cast<char *>(valores.data()), bytes);

    return arquivoColunas && arquivoValues;
}

vector<double> distanciasDaLinha(
    const MatrizCSR &compras,
    int indiceCliente,
    const vector<uint16_t> &colunas,
    const vector<uint16_t> &valores
) {
    vector<double> distancias(compras.linhas, 1.0);
    int totalCompras = compras.row_ptr[indiceCliente + 1] - compras.row_ptr[indiceCliente];

    if (totalCompras == 0) {
        return distancias;
    }

    for (size_t k = 0; k < colunas.size(); k++) {
        if (colunas[k] != indiceCliente) {
            distancias[colunas[k]] = 1.0 - (double)valores[k] / totalCompras;
        }
    }

    return distancias;
}
