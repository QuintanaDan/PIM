#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <direct.h>
#include "orcamentos.h"
#include "../Uteis/uteis.h"
#include <time.h>
#include "../Clientes/clientes.h"
#include "../Produtos/produtos.h"

//Função para fazer Orçamento
void gerarOrcamentoCliente() {
    char cpf[12];
    int id;
    float quantidadeDesejada;
    Produto produto;
    Cliente cliente;
    float precoTotal = 0.0f;
    char continuar;
    char nomeCliente[100];
    char resposta;
    char confirmarProduto;

    printf("\nPressione ESC a qualquer momento para cancelar e voltar ao menu.\n");

    FILE *fileProdutos = fopen("produtos.txt", "r");
    FILE *fileClientes = fopen("clientes.txt", "r");

    if (fileProdutos == NULL || fileClientes == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (fileProdutos) fclose(fileProdutos);
        if (fileClientes) fclose(fileClientes);
        return;
    }

    char temCadastro;
    printf("\nO cliente possui cadastro? (S/N): ");
    temCadastro = _getch();
    if (temCadastro == 27) { // 27 é o código ASCII da tecla ESC
        fclose(fileProdutos);
        fclose(fileClientes);
        printf("\nOperacao cancelada pelo usuario.\n");
        printf("Pressione qualquer tecla para voltar ao menu...");
        _getch();
        return;
    }
    printf("%c\n", temCadastro);
    limpar_buffer();

    int clienteEncontrado = 0;
    if (temCadastro == 'S' || temCadastro == 's') {
        printf("Digite o CPF do cliente: ");
        int i = 0;
        char c;
        while (i < sizeof(cpf) - 1) {
            c = _getch();
            if (c == 27) { // ESC
                fclose(fileProdutos);
                fclose(fileClientes);
                printf("\nOperacao cancelada pelo usuario.\n");
                printf("Pressione qualquer tecla para voltar ao menu...");
                _getch();
                return;
            }
            if (c == '\r' || c == '\n') break;
            cpf[i++] = c;
            printf("%c", c);
        }
        cpf[i] = '\0';
        printf("\n");

        while (fscanf(fileClientes,
                      "CPF: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
                      cliente.cpf, cliente.nome, cliente.endereco, cliente.telefone, cliente.email) == 5) {
            if (strcmp(cliente.cpf, cpf) == 0) {
                clienteEncontrado = 1;
                strcpy(nomeCliente, cliente.nome);
                break;
            }
        }

        if (!clienteEncontrado) {
            printf("Cliente com CPF %s nao encontrado.\n", cpf);
            strcpy(cpf, "Indefinido");
            printf("Prosseguindo com o orcamento como cliente nao cadastrado.\n");
        }
    } else {
        strcpy(cpf, "Indefinido");
        printf("Digite o nome do cliente: ");
        int i = 0;
        char c;
        while (i < sizeof(nomeCliente) - 1) {
            c = _getch();
            if (c == 27) { // ESC
                fclose(fileProdutos);
                fclose(fileClientes);
                printf("\nOperacao cancelada pelo usuario.\n");
                printf("Pressione qualquer tecla para voltar ao menu...");
                _getch();
                return;
            }
            if (c == '\r' || c == '\n') break;
            nomeCliente[i++] = c;
            printf("%c", c);
        }
        nomeCliente[i] = '\0';
        printf("\n");
    }

    Produto itensOrcamento[100];
    float quantidades[100];
    int numItens = 0;

    do {
        int produtoValido = 0;
        while (!produtoValido) {
            printf("\nDigite o Codigo do produto para orcamento (ou ESC para cancelar): ");
            char input[10];
            int i = 0;
            char c;
            while (i < sizeof(input) - 1) {
                c = _getch();
                if (c == 27) { // ESC
                    fclose(fileProdutos);
                    fclose(fileClientes);
                    printf("\nOperacao cancelada pelo usuario.\n");
                    printf("Pressione qualquer tecla para voltar ao menu...");
                    _getch();
                    return;
                }
                if (c == '\r' || c == '\n') break;
                input[i++] = c;
                printf("%c", c);
            }
            input[i] = '\0';
            printf("\n");
            id = atoi(input);

            int produtoEncontrado = 0;
            rewind(fileProdutos);

            while (fscanf(fileProdutos,
                          "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                          &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo,
                          &produto.quantidade, produto.categoria, produto.data_entrada, produto.validade) == 9) {
                if (produto.codigo == id) {
                    produtoEncontrado = 1;
                    printf("\nProduto encontrado: %s\n\nEste eh o produto desejado? (S/N): ", produto.nome);
                    confirmarProduto = _getch();
                    if (confirmarProduto == 27) { // ESC
                        fclose(fileProdutos);
                        fclose(fileClientes);
                        printf("\nOperacao cancelada pelo usuario.\n");
                        printf("Pressione qualquer tecla para voltar ao menu...");
                        _getch();
                        return;
                    }
                    printf("%c\n", confirmarProduto);

                    if (confirmarProduto == 'S' || confirmarProduto == 's') {
                        printf("\nDigite a quantidade desejada (em kg): ");
                        char qtd[10];
                        i = 0;
                        while (i < sizeof(qtd) - 1) {
                            c = _getch();
                            if (c == 27) { // ESC
                                fclose(fileProdutos);
                                fclose(fileClientes);
                                printf("\nOperacao cancelada pelo usuario.\n");
                                printf("Pressione qualquer tecla para voltar ao menu...");
                                _getch();
                                return;
                            }
                            if (c == '\r' || c == '\n') break;
                            qtd[i++] = c;
                            printf("%c", c);
                        }
                        qtd[i] = '\0';
                        printf("\n");
                        quantidadeDesejada = atof(qtd);

                        if (quantidadeDesejada > produto.quantidade) {
                            printf("Quantidade solicitada maior do que a disponivel em estoque.\n");
                        } else {
                            float precoAtual = produto.preco * quantidadeDesejada;
                            precoTotal += precoAtual;
                            itensOrcamento[numItens] = produto;
                            quantidades[numItens] = quantidadeDesejada;
                            numItens++;
                            produtoValido = 1;
                            printf("\nOrcamento: %.2f kg de %s por R$ %.2f.\n", quantidadeDesejada, produto.nome, precoAtual);
                        }
                    }
                    break;
                }
            }

            if (!produtoEncontrado) {
                printf("Produto com Codigo %d nao encontrado.\n", id);
            }
        }

        printf("\nDeseja adicionar mais itens ao orcamento? (S/N): ");
        continuar = _getch();
        if (continuar == 27) { // ESC
            fclose(fileProdutos);
            fclose(fileClientes);
            printf("\nOperacao cancelada pelo usuario.\n");
            printf("Pressione qualquer tecla para voltar ao menu...");
            _getch();
            return;
        }
        printf("%c\n", continuar);
        limpar_buffer();

    } while (continuar == 'S' || continuar == 's');

    fclose(fileClientes);
    fclose(fileProdutos);

    // Exibir o orçamento final
    printf("\n======================================== ORCAMENTO ========================================\n");
    printf("Cliente: %s\n", nomeCliente);
    printf("%-8s %-15s %-30s %-10s %-12s %-12s\n",
           "Codigo", "Produto", "Descricao", "Preco/Kg", "Qtd (kg)", "Total (R$)");
    printf("-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-8d %-15s %-30s %-10.2f %-12.2f %-12.2f\n",
               itensOrcamento[i].codigo,
               itensOrcamento[i].nome,
               itensOrcamento[i].descricao,
               itensOrcamento[i].preco,
               quantidades[i],
               itensOrcamento[i].preco * quantidades[i]);
    }
    printf("-------------------------------------------------------------------------------------------\n");
    printf("%-77s %-12.2f\n", "TOTAL:", precoTotal);
    printf("===========================================================================================\n");

    // Perguntar se deseja salvar em arquivo
    printf("\nDeseja salvar o orcamento em arquivo? (S/N): ");
    resposta = _getch();
    if (resposta == 27) { // ESC
        printf("\nOperacao cancelada pelo usuario.\n");
        printf("Pressione qualquer tecla para voltar ao menu...");
        _getch();
        return;
    }
    printf("%c\n", resposta);
    limpar_buffer();

     if (resposta == 'S' || resposta == 's') {
        // Criar diretório 'orcamentos' se não existir
        #ifdef _WIN32
            _mkdir("orcamentos");
        #else
            mkdir("orcamentos", 0777);
        #endif

        // Obter data atual
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char data[11];
        sprintf(data, "%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

        // Criar nome do arquivo com o caminho completo
        char nomeArquivo[256];
        #ifdef _WIN32
            sprintf(nomeArquivo, "orcamentos\\orcamento_%s_%s.txt", nomeCliente, data);
        #else
            sprintf(nomeArquivo, "orcamentos/orcamento_%s_%s.txt", nomeCliente, data);
        #endif

        // Remover caracteres inválidos do nome do arquivo
        for (int i = 0; nomeArquivo[i]; i++) {
            if (nomeArquivo[i] == ':' || nomeArquivo[i] == '*' ||
                nomeArquivo[i] == '?' || nomeArquivo[i] == '"' ||
                nomeArquivo[i] == '<' || nomeArquivo[i] == '>' ||
                nomeArquivo[i] == '|' || nomeArquivo[i] == ' ') {
                nomeArquivo[i] = '_';
            }
        }

        FILE *arquivoOrcamento = fopen(nomeArquivo, "w");
        if (arquivoOrcamento != NULL) {
            fprintf(arquivoOrcamento, "======================================== ORCAMENTO ========================================\n");
            fprintf(arquivoOrcamento, "Data: %s\n", data);
            fprintf(arquivoOrcamento, "Cliente: %s\n", nomeCliente);
            fprintf(arquivoOrcamento, "%-8s %-15s %-30s %-10s %-12s %-12s\n",
                    "Codigo", "Produto", "Descricao", "Preco/Kg", "Qtd (kg)", "Total (R$)");
            fprintf(arquivoOrcamento, "-------------------------------------------------------------------------------------------\n");

            for (int i = 0; i < numItens; i++) {
                fprintf(arquivoOrcamento, "%-8d %-15s %-30s %-10.2f %-12.2f %-12.2f\n",
                        itensOrcamento[i].codigo,
                        itensOrcamento[i].nome,
                        itensOrcamento[i].descricao,
                        itensOrcamento[i].preco,
                        quantidades[i],
                        itensOrcamento[i].preco * quantidades[i]);
            }

            fprintf(arquivoOrcamento, "-------------------------------------------------------------------------------------------\n");
            fprintf(arquivoOrcamento, "%-77s %-12.2f\n", "TOTAL:", precoTotal);
            fprintf(arquivoOrcamento, "===========================================================================================\n");

            fclose(arquivoOrcamento);
            printf("Orcamento salvo com sucesso no arquivo: %s\n", nomeArquivo);
        } else {
            printf("Erro ao criar arquivo do orcamento.\n");
            perror("Erro");
        }
    }

    printf("\nPressione Enter para voltar ao menu...");
    limpar_buffer();
}

//Menu de Orçamento
void menuOrcamentos() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE ORCAMENTOS             \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Cadastrar Orcamento               |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do stdin

        switch (opcao) {
            case 1: gerarOrcamentoCliente(); break;
            case 0: printf("Saindo do menu de orcamentos.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}