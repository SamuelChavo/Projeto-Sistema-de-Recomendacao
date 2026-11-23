#ifndef LISTA_DE_COMPRAS_H
#define LISTA_DE_COMPRAS_H

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>

using namespace std;

struct ListaCompras {
    vector<string> clientes;
    map<string, int> mapaClientes;

    vector<string> produtos;
    map<string, int> mapaProdutos;

    vector<list<int>> compras;
};

bool carregarArquivo(ListaCompras &dados, const string &caminho);
bool carregarPasta(ListaCompras &dados, const string &pasta);
void exibirComprasCliente(const ListaCompras &dados, const string &codigoCliente);
int buscarCliente(const ListaCompras &dados, const string &codigoCliente);

#endif