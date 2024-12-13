#ifndef FORNECEDORES_H
#define FORNECEDORES_H

typedef struct {
    char cnpj[20];
    char nome[50];
    char endereco[100];
    char telefone[16];
    char email[50];
} Fornecedor;

void cadastrarFornecedor();
void editarFornecedor();
void excluirFornecedor();
void listarFornecedores();
void menuFornecedores();
void menuFornecedores();

#endif
