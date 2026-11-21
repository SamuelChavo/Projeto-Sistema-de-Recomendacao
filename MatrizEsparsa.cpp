
#include "MatrizEsparsa.h"

MatrizCSR converterParaCSR(
    const vector<vector<unsigned char>> &matriz
) {
    MatrizCSR resultado;

    resultado.linhas = matriz.size();
    resultado.colunas = matriz.empty() ? 0 : matriz[0].size();

    resultado.row_ptr.push_back(0);

    for (int i = 0; i < resultado.linhas; i++) {
        for (int j = 0; j < resultado.colunas; j++) {
            if (matriz[i][j] != 0) {
                resultado.values.push_back(matriz[i][j]);
                resultado.col_index.push_back(j);
            }
        }

        resultado.row_ptr.push_back(resultado.values.size());
    }

    return resultado;
}

int produtoInternoCSR(
    const MatrizCSR &matriz,
    int linhaA,
    int linhaB
) {
    if (linhaA < 0 || linhaA >= matriz.linhas ||
        linhaB < 0 || linhaB >= matriz.linhas) {
        return 0;
    }

    int pa = matriz.row_ptr[linhaA];
    int fimA = matriz.row_ptr[linhaA + 1];

    int pb = matriz.row_ptr[linhaB];
    int fimB = matriz.row_ptr[linhaB + 1];

    int soma = 0;

    while (pa < fimA && pb < fimB) {
        int colunaA = matriz.col_index[pa];
        int colunaB = matriz.col_index[pb];

        if (colunaA == colunaB) {
            soma += matriz.values[pa] * matriz.values[pb];

            pa++;
            pb++;
        } else if (colunaA < colunaB) {
            pa++;
        } else {
            pb++;
        }
    }

    return soma;
}

MatrizCSR multiplicarCSR(const MatrizCSR &matriz) {
    MatrizCSR resultado;

    resultado.linhas = matriz.linhas;
    resultado.colunas = matriz.linhas;

    resultado.row_ptr.push_back(0);

    for (int i = 0; i < matriz.linhas; i++) {
        for (int j = 0; j < matriz.linhas; j++) {
            int valor = produtoInternoCSR(matriz, i, j);

            if (valor != 0) {
                resultado.values.push_back(valor);
                resultado.col_index.push_back(j);
            }
        }

        resultado.row_ptr.push_back(resultado.values.size());
    }

    return resultado;
}

int obterValorCSR(
    const MatrizCSR &matriz,
    int linha,
    int coluna
) {
    if (linha < 0 || linha >= matriz.linhas ||
        coluna < 0 || coluna >= matriz.colunas) {
        return 0;
    }

    for (int p = matriz.row_ptr[linha];
         p < matriz.row_ptr[linha + 1];
         p++) {

        if (matriz.col_index[p] == coluna) {
            return matriz.values[p];
        }

        if (matriz.col_index[p] > coluna) {
            break;
        }
    }

    return 0;
}

vector<double> calcularDistanciasCSR(
    const MatrizCSR &compras,
    const MatrizCSR &intersecoes,
    int indiceCliente
) {
    vector<double> distancias(compras.linhas, 1.0);

    if (indiceCliente < 0 || indiceCliente >= compras.linhas) {
        return distancias;
    }

    int totalCompras =
        compras.row_ptr[indiceCliente + 1] -
        compras.row_ptr[indiceCliente];

    if (totalCompras == 0) {
        return distancias;
    }

    for (int i = 0; i < compras.linhas; i++) {
        if (i == indiceCliente) {
            continue;
        }

        int intersecao = obterValorCSR(
            intersecoes,
            indiceCliente,
            i
        );

        distancias[i] =
            1.0 - (double)intersecao / totalCompras;
    }

    return distancias;
}

size_t memoriaCSR(const MatrizCSR &matriz) {
    return matriz.values.size() * sizeof(int)
         + matriz.col_index.size() * sizeof(int)
         + matriz.row_ptr.size() * sizeof(int);
}

