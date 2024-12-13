#ifndef COMPRAS_H
#define COMPRAS_H

typedef struct {
    char fornecedor[100];
    char cnpj[15];
    char produto[100];
    int quantidade;
    float precoUnitario;
    float precoTotal;
    char recebimento[20];
    int entradaProduto;
    char dataCompra[11];  //(formato DD/MM/AAAA)
} Compra;

void registrarCompra();
void listarCompras();
void confirmacaoEntradaProduto();
void menuCompras();

#endif
