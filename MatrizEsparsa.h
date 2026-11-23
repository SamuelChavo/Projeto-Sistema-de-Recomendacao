#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include "Lista_de_compras.h"

using namespace std;

struct MatrizCSR {
    int linhas;
    int colunas;
    vector<uint16_t> values;
    vector<uint16_t> col_index;
    vector<int> row_ptr;
};

struct LeitorCSRDisco {
    ifstream arquivoValues;
    ifstream arquivoColunas;
    vector<int> row_ptr;

    bool abrir(const string &prefixoArquivo);
    bool lerLinha(int linha, vector<uint16_t> &colunas, vector<uint16_t> &valores);
};

MatrizCSR converterComprasParaCSR(const ListaCompras &dados);

bool multiplicarCSRParaDisco(const MatrizCSR &matriz, const string &prefixoArquivo);

vector<double> distanciasDaLinha(
    const MatrizCSR &compras,
    int indiceCliente,
    const vector<uint16_t> &colunas,
    const vector<uint16_t> &valores
);

#endif
