#ifndef UTEIS_H
#define UTEIS_H

#include <stdbool.h>

void limpa_tela();
void nomehort();
void ler_cpf(char cpf[]);
void ler_telefone();
void aplicarMascaraTelefone(char telefone[]);
void aplicarMascaraCpf(char cpf[]);
void ler_cnpj(char cnpj[]);
void aplicarMascaraCnpj(char cnpj[]);
void ler_data_nascimento(char *data);
void limpar_buffer();
void obter_data_atual(char *data_str);
void obter_data_validade(char *validade_str);
int comparaDataVenda(char *dataVenda, char *dataInicio, char *dataFim);

typedef struct {
    int codigo;
    char nome[100];
    float quantidadeVendida;
} ProdutoVenda;

int compareProdutoVenda(const void *a, const void *b);
bool verificarESC();

#endif //UTEIS_H
