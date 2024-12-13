#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

typedef struct {
    char cpf[15];
    char nome[100];
    char endereco[100];
    char telefone[15];
    char genero[10];
    char dataNascimento[11]; // Formato DD/MM/AAAA
    char cargo[50];
    float salario;
} Funcionario;

void cadastrarFuncionario();
void editarFuncionario();
void excluirFuncionario();
void listarFuncionarios();
void menuFuncionarios();

#endif
