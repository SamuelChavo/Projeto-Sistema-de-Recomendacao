#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "Lista_de_compras.h"
#include <vector>

using namespace std;

struct DadosSimilaridade {
    vector<vector<unsigned char>> matrizCompras;
    vector<double> similaridades;
};

void construirMatrizCompras(const ListaCompras &dados, DadosSimilaridade &sim);
void calcularSimilaridades(const ListaCompras &dados, DadosSimilaridade &sim, int indiceCliente);
int clienteMaisSimilar(const DadosSimilaridade &sim, int indiceCliente);

#endif