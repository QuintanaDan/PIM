#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "vendas.h"
#include "../Clientes/clientes.h"
#include "../Produtos/produtos.h"
#include "../Uteis/uteis.h"
#include "../Orçamentos/orcamentos.h"

// Função para registrar uma venda
void registrarVendaCliente() {
    char cpf[12];
    int id;
    float quantidadeVendida;
    Produto produto;
    Cliente cliente;
    float precoTotal = 0.0f;
    char continuar;
    char confirmarProduto;
    ItemVendido itensVendidos[100];
    int numItensVendidos = 0;

    FILE *fileProdutos = fopen("produtos.txt", "r+");
    FILE *tempProdutos = fopen("temp_produtos.txt", "w");
    FILE *fileClientes = fopen("clientes.txt", "r");
    FILE *fileVendas = fopen("vendas.txt", "a+");

    if (fileProdutos == NULL || tempProdutos == NULL || fileClientes == NULL || fileVendas == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (fileProdutos) fclose(fileProdutos);
        if (tempProdutos) fclose(tempProdutos);
        if (fileClientes) fclose(fileClientes);
        if (fileVendas) fclose(fileVendas);
        return;
    }

    // Gerar ID único para a venda
    int vendaID = 1;
    char linha[256];
    while (fgets(linha, sizeof(linha), fileVendas) != NULL) {
        if (sscanf(linha, "ID da Venda: %d", &vendaID) == 1) {
            vendaID++;
        }
    }

    char temCadastro;
    printf("O cliente possui cadastro? (S/N): ");
    scanf(" %c", &temCadastro);
    limpar_buffer();

    int clienteEncontrado = 0;
    if (temCadastro == 'S' || temCadastro == 's') {
        printf("Digite o CPF do cliente: ");
        fgets(cpf, sizeof(cpf), stdin);
        cpf[strcspn(cpf, "\n")] = 0;

        rewind(fileClientes);
        while (fscanf(fileClientes,
              "CPF: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
              cliente.cpf, cliente.nome, cliente.endereco, cliente.telefone, cliente.email) == 5) {
            if (strcmp(cliente.cpf, cpf) == 0) {
                clienteEncontrado = 1;
                break;
            }
        }

        if (!clienteEncontrado) {
            printf("Cliente com CPF %s nao encontrado.\n", cpf);
            strcpy(cpf, "Indefinido");
            printf("Prosseguindo com o registro da venda como cliente nao cadastrado.\n");
        }
    } else {
        strcpy(cpf, "Indefinido");
        printf("Prosseguindo com o registro da venda como cliente nao cadastrado.\n");
    }

    Produto produtos[100];
    int numProdutos = 0;

    rewind(fileProdutos);
    while (fscanf(fileProdutos,
                  "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                  &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo,
                  &produto.quantidade, produto.categoria, produto.data_entrada, produto.validade) == 9) {
        produtos[numProdutos++] = produto;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char dataHoraCompra[20];
    sprintf(dataHoraCompra, "%02d/%02d/%04d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    // Iniciar o registro da venda no arquivo
    fprintf(fileVendas, "ID da Venda: %d\n", vendaID);
    fprintf(fileVendas, "Data e Hora: %s\n", dataHoraCompra);
    fprintf(fileVendas, "Cliente CPF: %s\n", cpf);
    fprintf(fileVendas, "Itens:\n");

    do {
        int produtoEncontrado = 0;
        do {
            printf("\nDigite o Código do produto a ser vendido: ");
            scanf("%d", &id);
            limpar_buffer();

            for (int i = 0; i < numProdutos; i++) {
                if (produtos[i].codigo == id) {
                    produtoEncontrado = 1;
                    printf("\nProduto encontrado:\n");
                    printf("Codigo: %d\n", produtos[i].codigo);
                    printf("Nome: %s\n", produtos[i].nome);
                    printf("Descricao: %s\n", produtos[i].descricao);
                    printf("Preco por Kg: R$ %.2f\n", produtos[i].preco);
                    printf("Quantidade em estoque: %.2f kg\n", produtos[i].quantidade);

                    printf("\nO produto esta correto? (S/N): ");
                    scanf(" %c", &confirmarProduto);
                    limpar_buffer();

                    if (confirmarProduto == 'S' || confirmarProduto == 's') {
                        printf("Digite a quantidade a ser vendida (em kg): ");
                        scanf("%f", &quantidadeVendida);
                        limpar_buffer();

                        if (quantidadeVendida > produtos[i].quantidade) {
                            printf("Quantidade solicitada maior do que a disponivel em estoque.\n");
                            produtoEncontrado = 0;
                        } else {
                            produtos[i].quantidade -= quantidadeVendida;
                            float precoAtual = produtos[i].preco * quantidadeVendida;
                            precoTotal += precoAtual;

                            printf("Venda registrada: %.2f kg de %s vendidas por R$ %.2f.\n", quantidadeVendida, produtos[i].nome, precoAtual);

                            // Registrar item no arquivo de vendas
                            fprintf(fileVendas, "- Produto: %s, Codigo: %d, Quantidade: %.2f kg, Preco: R$ %.2f\n",
                                    produtos[i].nome, produtos[i].codigo, quantidadeVendida, precoAtual);

                            // Adicionar o item ao array de itens vendidos
                            itensVendidos[numItensVendidos].codigo = produtos[i].codigo;
                            strcpy(itensVendidos[numItensVendidos].nome, produtos[i].nome);
                            itensVendidos[numItensVendidos].quantidade = quantidadeVendida;
                            itensVendidos[numItensVendidos].precoUnitario = produtos[i].preco;
                            itensVendidos[numItensVendidos].precoTotal = precoAtual;
                            numItensVendidos++;
                        }
                    } else {
                        produtoEncontrado = 0;
                    }
                    break;
                }
            }

            if (!produtoEncontrado) {
                printf("Produto nao encontrado ou nao confirmado. Por favor, tente novamente.\n");
            }
        } while (!produtoEncontrado);

        printf("\nDeseja adicionar mais itens a venda? (S/N): ");
        scanf(" %c", &continuar);
        limpar_buffer();

    } while (continuar == 'S' || continuar == 's');

    // Finalizar o registro da venda no arquivo
    fprintf(fileVendas, "Total da Venda: R$ %.2f\n\n", precoTotal);

    // Atualizar o arquivo de produtos
    rewind(fileProdutos);
    for (int i = 0; i < numProdutos; i++) {
        fprintf(tempProdutos, "Codigo: %d\nNome: %s\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n\n",
                produtos[i].codigo, produtos[i].nome, produtos[i].descricao, produtos[i].preco, produtos[i].preco_custo,
                produtos[i].quantidade, produtos[i].categoria, produtos[i].data_entrada, produtos[i].validade);
    }

    fclose(fileClientes);
    fclose(fileProdutos);
    fclose(tempProdutos);
    fclose(fileVendas);

    if (remove("produtos.txt") != 0) {
        printf("Erro ao remover produtos.txt.\n");
    }
    if (rename("temp_produtos.txt", "produtos.txt") != 0) {
        printf("Erro ao renomear temp_produtos.txt para produtos.txt.\n");
    }

    // Exibir resumo da compra
    printf("\n========================= RESUMO DA VENDA =========================\n");
    printf("ID da Venda: %d\n", vendaID);
    printf("Data e Hora: %s\n", dataHoraCompra);
    printf("Cliente CPF: %s\n", cpf);
    printf("\n%-8s %-20s %-10s %-12s %-12s\n",
           "Codigo", "Produto", "Preco/Kg", "Qtd (kg)", "Total (R$)");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < numItensVendidos; i++) {
        printf("%-8d %-20s %-10.2f %-12.2f %-12.2f\n",
               itensVendidos[i].codigo,
               itensVendidos[i].nome,
               itensVendidos[i].precoUnitario,
               itensVendidos[i].quantidade,
               itensVendidos[i].precoTotal);
    }

    printf("------------------------------------------------------------------\n");
    printf("%-52s %-12.2f\n", "TOTAL:", precoTotal);
    printf("==================================================================\n");

    printf("\nVenda ID %d registrada com sucesso.\n", vendaID);
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Função para listar vendas
void listarVendas() {
    FILE *file = fopen("vendas.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de vendas. Verifique se o arquivo 'vendas.txt' existe.\n");
        return;
    }

    char linha[200];
    char clienteCPF[50];
    char dataHora[50];
    int vendaID;
    float precoTotal;
    int vendasEncontradas = 0;

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "ID da Venda: %d", &vendaID) == 1) {
            vendasEncontradas++;

            printf("\n========================= VENDA ID: %d =========================\n", vendaID);

            // Lê a data e hora
            if (fgets(linha, sizeof(linha), file) && strstr(linha, "Data e Hora:")) {
                sscanf(linha, "Data e Hora: %[^\n]", dataHora);
                printf("Data e Hora: %s\n", dataHora);
            }

            // Lê o CPF do cliente
            if (fgets(linha, sizeof(linha), file) && strstr(linha, "Cliente CPF:")) {
                sscanf(linha, "Cliente CPF: %[^\n]", clienteCPF);
                printf("Cliente CPF: %s\n", clienteCPF);
            }

            // Pula a linha "Itens:"
            fgets(linha, sizeof(linha), file);

            // Cabeçalho da tabela de itens
            printf("\n%-8s %-20s %-10s %-12s %-12s\n",
                   "Codigo", "Produto", "Preco/Kg", "Qtd (kg)", "Total (R$)");
            printf("-------------------------------------------------------------------------------------------\n");

            // Lê os itens
            while (fgets(linha, sizeof(linha), file) && strstr(linha, "- Produto:")) {
                char produtoNome[50];
                int produtoCodigo;
                float quantidade, precoItem;

                sscanf(linha, "- Produto: %[^,], Codigo: %d, Quantidade: %f kg, Preco: R$ %f",
                       produtoNome, &produtoCodigo, &quantidade, &precoItem);

                float precoPorKg = precoItem / quantidade;

                printf("%-8d %-20s %-10.2f %-12.2f %-12.2f\n",
                       produtoCodigo, produtoNome, precoPorKg, quantidade, precoItem);
            }

            // Lê o total da venda
            sscanf(linha, "Total da Venda: R$ %f", &precoTotal);
            printf("-------------------------------------------------------------------------------------------\n");
            printf("%-52s %-12.2f\n", "TOTAL:", precoTotal);

            printf("===========================================================================================\n");
        }
    }

    if (vendasEncontradas == 0) {
        printf("Nenhuma venda registrada.\n");
    } else {
        printf("\nTotal de vendas registradas: %d\n", vendasEncontradas);
    }

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Função para listar as Vendas por Cliente atravez do CPF
void listarVendasPorCliente() {
    char cpfBusca[12];
    char linha[200];
    int clienteEncontrado = 0;
    int vendaID;

    printf("Digite o CPF do cliente para listar as vendas: ");
    fgets(cpfBusca, sizeof(cpfBusca), stdin);
    cpfBusca[strcspn(cpfBusca, "\n")] = 0; // Remove o '\n'

    FILE *file = fopen("vendas.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    printf("\n=== Vendas do Cliente CPF: %s ===\n", cpfBusca);

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "ID da Venda: %d", &vendaID) == 1) {
            char clienteCPF[50], dataHora[50];
            float precoTotal;

            // Lê a data e hora
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Data e Hora: %[^\n]", dataHora);

            // Lê o CPF do cliente
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Cliente CPF: %[^\n]", clienteCPF);

            // Verifica se é o cliente buscado
            if (strcmp(clienteCPF, cpfBusca) == 0) {
                clienteEncontrado = 1;

                printf("\n========================= VENDA ID: %d =========================\n", vendaID);
                printf("Data e Hora: %s\n", dataHora);
                printf("Cliente CPF: %s\n", clienteCPF);

                // Cabeçalho da tabela de itens
                printf("\n%-8s %-20s %-10s %-12s %-12s\n",
                       "Codigo", "Produto", "Preco/Kg", "Qtd (kg)", "Total (R$)");
                printf("-------------------------------------------------------------------------------------------\n");

                // Pula a linha "Itens:"
                fgets(linha, sizeof(linha), file);

                // Lê os itens
                while (fgets(linha, sizeof(linha), file) && strstr(linha, "- Produto:")) {
                    char produtoNome[50];
                    int produtoCodigo;
                    float quantidade, precoItem;

                    sscanf(linha, "- Produto: %[^,], Codigo: %d, Quantidade: %f kg, Preco: R$ %f",
                           produtoNome, &produtoCodigo, &quantidade, &precoItem);

                    float precoPorKg = precoItem / quantidade;

                    printf("%-8d %-20s %-10.2f %-12.2f %-12.2f\n",
                           produtoCodigo, produtoNome, precoPorKg, quantidade, precoItem);
                }

                // Lê o total da venda
                sscanf(linha, "Total da Venda: R$ %f", &precoTotal);
                printf("-------------------------------------------------------------------------------------------\n");
                printf("%-52s %-12.2f\n", "TOTAL:", precoTotal);

                printf("===========================================================================================\n");
            }
        }
    }

    if (!clienteEncontrado) {
        printf("Nenhuma venda encontrada para o cliente com CPF: %s.\n", cpfBusca);
    }

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
    getchar();
}

//Menu de vendas
void menuVendas() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE VENDAS               \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Registrar Venda                   |\n");
        printf("|      2. Listar Vendas                     |\n");
        printf("|      3. Listar por Cliente                |\n");
        printf("|      4. Orcamento                         |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do stdin

        switch (opcao) {
            case 1: registrarVendaCliente(); break;
            case 2: listarVendas(); break;
            case 3: listarVendasPorCliente(); break;
            case 4: gerarOrcamentoCliente(); break;
            case 0: printf("Saindo do menu de vendas.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}
