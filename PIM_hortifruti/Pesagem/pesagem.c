#include <stdio.h>
#include "pesagem.h"
#include <windows.h>
#include "../Produtos/produtos.h"

// Função para registrar uma nova pesagem
void registrarPesagem() {
    int codigoProduto;
    float peso;
    Produto produto;
    int encontrado = 0;
    char confirmar;

    do {
        // Solicita o código do produto
        printf("Digite o codigo do produto: ");
        scanf("%d", &codigoProduto);
        getchar(); // Limpar o buffer do stdin

        // Abre o arquivo de produtos para verificar se o produto existe
        FILE *fileProdutos = fopen("produtos.txt", "r");
        if (fileProdutos == NULL) {
            printf("Erro ao abrir o arquivo de produtos.\n");
            return;
        }

        // Procura o produto pelo código
        while (fscanf(fileProdutos, "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                      &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo, &produto.quantidade, produto.categoria, produto.data_entrada, produto.validade) == 9) {
            if (produto.codigo == codigoProduto) {
                encontrado = 1;
                break;
            }
        }
        fclose(fileProdutos);

        if (!encontrado) {
            printf("Produto com codigo %d nao encontrado.\n", codigoProduto);
            return;
        }

        // Mostra os detalhes do produto encontrado
        printf("\nProduto encontrado:\n");
        printf("Codigo: %d\n", produto.codigo);
        printf("Nome: %s\n", produto.nome);
        printf("Descricao: %s\n", produto.descricao);


        // Pergunta ao usuário se é o produto correto
        printf("\nO produto esta correto? (S/N): ");
        scanf(" %c", &confirmar);
        getchar(); // Limpar o buffer do stdin

        if (confirmar == 'N' || confirmar == 'n') {
            encontrado = 0; // Reseta a variável para permitir nova tentativa
        }
    } while (!encontrado);

    // Solicita o peso do produto
    printf("Digite o peso em kg: ");
    scanf("%f", &peso);
    getchar(); // Limpar o buffer do stdin

    // Calcula o valor total
    float valorTotal = peso * produto.preco;

    // Abre o arquivo para registrar a pesagem
    FILE *filePesagens = fopen("pesagens.txt", "a");
    if (filePesagens != NULL) {
        fprintf(filePesagens, "Codigo do Produto: %d\nPeso: %.2f kg\nPreco por Kg: %.2f\nValor Total: %.2f\n\n", codigoProduto, peso, produto.preco, valorTotal);
        fclose(filePesagens);
        printf("Pesagem registrada com sucesso.\n");
        printf("Codigo do Produto: %d\nPeso: %.2f kg\nPreco por Kg: %.2f\nValor Total: %.2f\n", codigoProduto, peso, produto.preco, valorTotal);
    } else {
        printf("Erro ao abrir o arquivo para registro de pesagem.\n");
    }

    // Espera o usuário pressionar Enter para voltar ao menu
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

// Função para visualizar o histórico de pesagens
void listarPesagens() {
    char linha[256];
    FILE *file = fopen("pesagens.txt", "r");

    if (file != NULL) {
        printf("\n=== Historico de Pesagens ===\n");
        printf("+------------------+------------+---------------+---------------+\n");
        printf("| Codigo do Produto| Peso (kg)  | Preco por Kg  | Valor Total   |\n");
        printf("+------------------+------------+---------------+---------------+\n");

        int codigoProduto;
        float peso, precoPorKg, valorTotal;

        while (fgets(linha, sizeof(linha), file)) {
            // Verifica se a linha contém "Código do Produto"
            if (sscanf(linha, "Codigo do Produto: %d", &codigoProduto) == 1) {
                // Lê as próximas linhas para peso, preço por kg e valor total
                fgets(linha, sizeof(linha), file);
                sscanf(linha, "Peso: %f kg", &peso);

                fgets(linha, sizeof(linha), file);
                sscanf(linha, "Preco por Kg: %f", &precoPorKg);

                fgets(linha, sizeof(linha), file);
                sscanf(linha, "Valor Total: %f", &valorTotal);

                // Exibe os dados formatados em grid
                printf("| %-16d | %-10.2f | %-13.2f | %-13.2f |\n", codigoProduto, peso, precoPorKg, valorTotal);
                printf("+------------------+------------+---------------+---------------+\n");
            }
        }
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo de pesagens.\n");
    }

    // Espera o usuário pressionar Enter para voltar ao menu
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}
