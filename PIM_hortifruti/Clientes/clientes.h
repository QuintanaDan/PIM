#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct {
    char cpf[15];
    char nome[50];
    char endereco[100];
    char telefone[16];
    char email[50];
} Cliente;


void cadastrarCliente();
void editarCliente();
void excluirCliente();
void listarClientes();
void menuClientes();

#endif
