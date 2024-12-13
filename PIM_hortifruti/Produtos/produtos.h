#ifndef PRODUTOS_H
#define PRODUTOS_H

typedef struct {
    char nome[50];
    int codigo;
    char descricao[100];
    float preco;       // Preço por kg
    float preco_custo; // Preço de custo por kg
    float quantidade;  // Quantidade em kg
    char categoria[20];
    char data_entrada[11];
    char validade[11];
} Produto;


void cadastrarProduto();
void editarProduto();
void excluirProduto();
void listarProdutos();
void entradaProduto();
void saidaProdutos();
void menuProdutos();

#endif
