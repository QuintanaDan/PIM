#ifndef ORCAMENTO_H
#define ORCAMENTO_H

typedef struct {
    char clienteNome[50];
    char data[11]; // Data no formato dd/mm/aaaa
    float valorTotal;
    char produtos[200]; // Lista de produtos orçados
} Orcamento;

void gerarOrcamentoCliente();
void menuOrcamentos();

#endif // ORCAMENTO_H
// Estrutura para armazenar informações do orçamento
