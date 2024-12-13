#ifndef VENDAS_H
#define VENDAS_H

typedef struct {
    char dataHora[20];  // Formato de data e hora
    char cpf[12];       // CPF do cliente
    int codigoProduto;
    char nomeProduto[100];
    float quantidadeVendida;
    float precoTotal;
} Venda;

typedef struct {
    int codigo;
    char nome[50];
    float quantidade;
    float precoUnitario;
    float precoTotal;
} ItemVendido;

void registrarVendaCliente();
void listarVendas();
void listarVendasPorCliente();
void menuVendas();

#endif
