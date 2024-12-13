#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "compras.h"
#include "../Uteis/uteis.h"
#include "../Fornecedores/fornecedores.h"
#include "../Relatorios/relatorios.h"
#include "../Produtos/produtos.h"

#define MAX_LINE 256

//Função de Registrar Compra para o Hortifruti
void registrarCompra() {
    Compra compra;
    Fornecedor fornecedor;
    FILE *file = fopen("compras.txt", "a");
    FILE *fileFornecedores = fopen("fornecedores.txt", "r");
    int fornecedorEncontrado = 0;
    char confirmacao;

    if (file == NULL || fileFornecedores == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    limpar_buffer();

    printf("CNPJ do Fornecedor: ");
    fgets(compra.cnpj, sizeof(compra.cnpj), stdin);
    compra.cnpj[strcspn(compra.cnpj, "\n")] = 0;

    // Buscar fornecedor no arquivo fornecedores.txt
    while (fscanf(fileFornecedores, "CNPJ: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
                  fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email) != EOF) {
        if (strcmp(fornecedor.cnpj, compra.cnpj) == 0) {
            fornecedorEncontrado = 1;
            break;
        }
    }
    fclose(fileFornecedores);

    if (!fornecedorEncontrado) {
        printf("Fornecedor nao encontrado. A compra nao pode ser registrada.\n");
        fclose(file);
        return;
    }

    printf("Fornecedor encontrado: %s\n", fornecedor.nome);
    printf("Continuar a compra com este fornecedor? (S/N): ");
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Operação cancelada.\n");
        fclose(file);
        return;
    }

    strcpy(compra.fornecedor, fornecedor.nome);

    printf("Produto: ");
    if (fgets(compra.produto, sizeof(compra.produto), stdin) != NULL) {
        compra.produto[strcspn(compra.produto, "\n")] = 0;
    } else {
        printf("Erro ao ler o produto.\n");
        fclose(file);
        return;
    }

    printf("Quantidade (em kg): ");
    if (scanf("%d", &compra.quantidade) != 1) {
        printf("Erro ao ler a quantidade.\n");
        fclose(file);
        return;
    }
    limpar_buffer();

    printf("Preco Unitario (por kg): ");
    if (scanf("%f", &compra.precoUnitario) != 1) {
        printf("Erro ao ler o preco unitario.\n");
        fclose(file);
        return;
    }
    limpar_buffer();

    compra.precoTotal = compra.quantidade * compra.precoUnitario;

    // Define o recebimento como "Aguardando carga" automaticamente
    strcpy(compra.recebimento, "Aguardando carga");

    printf("A entrada do produto ja foi confirmada? (S/N): ");
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao == 'S' || confirmacao == 's') {
        compra.entradaProduto = 1;
        strcpy(compra.recebimento, "Confirmada");
    } else {
        compra.entradaProduto = 0;
    }

    // Obter a data atual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(compra.dataCompra, sizeof(compra.dataCompra), "%d/%m/%Y", tm);

    fprintf(file, "Fornecedor: %s\nCNPJ: %s\nProduto: %s\nQuantidade: %d kg\nPreco Unitario: %.2f por kg\nPreco Total: %.2f\nRecebimento: %s\nEntrada Produto: %d\nData da Compra: %s\n\n",
            compra.fornecedor, compra.cnpj, compra.produto, compra.quantidade, compra.precoUnitario,
            compra.precoTotal, compra.recebimento, compra.entradaProduto, compra.dataCompra);

    fclose(file);
    printf("Compra registrada com sucesso.\n");
}

//Função para atualizar a entrada dos produtos comprados que chegaram
void atualizarEntradaProduto(const char* fornecedor, const char* produto, const char* dataCompra) {
    FILE *file, *temp;
    char line[MAX_LINE];
    char tempFileName[] = "temp_compras.txt";
    int found = 0;

    file = fopen("compras.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de compras.\n");
        return;
    }

    temp = fopen(tempFileName, "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(file);
        return;
    }

    while (fgets(line, MAX_LINE, file) != NULL) {
        if (strstr(line, fornecedor) && !found) {
            fputs(line, temp);

            while (fgets(line, MAX_LINE, file) != NULL) {
                if (strstr(line, produto) && strstr(line, dataCompra)) {
                    found = 1;
                }

                if (strstr(line, "Entrada Produto:")) {
                    fputs("Entrada Produto: 1\n", temp);
                    found = 2;
                    break;
                } else {
                    fputs(line, temp);
                }
            }
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found == 2) {
        remove("compras.txt");
        rename(tempFileName, "compras.txt");
        printf("Entrada do produto atualizada com sucesso.\n");
    } else {
        remove(tempFileName);
        printf("Compra nao encontrada ou ja atualizada.\n");
    }
}

/*Função para Dar entrada / cadastrar produto
Função para dar Entrada nos Produtos que foram comprados, está sendo reutilizada a mesma função de cadstro de produtos por motivo de validade com algumas alteraçoes*/
void cadastrarProdutoEntrada() {
    Produto produto;
    FILE *file = fopen("produtos.txt", "r+"); // Abre para leitura e escrita

    if (file == NULL) {
        file = fopen("produtos.txt", "w"); // Cria o arquivo caso ele não exista
        if (file == NULL) {
            printf("Erro ao abrir o arquivo para escrita.\n");
            return;
        }
    }

    int ultimoCodigo = 0;

    // Lê o último código no arquivo, se houver produtos
    while (fscanf(file, "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                  &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo, &produto.quantidade, produto.categoria, produto.data_entrada, produto.validade) == 9) {
        ultimoCodigo = produto.codigo; // Armazena o último código lido
    }

    // Incrementa o último código para o novo produto
    produto.codigo = ultimoCodigo + 1;

    // Solicita os dados do novo produto
    printf("Nome: ");
    fgets(produto.nome, 50, stdin);
    produto.nome[strcspn(produto.nome, "\n")] = 0;

    printf("Descricao: ");
    fgets(produto.descricao, 100, stdin);
    produto.descricao[strcspn(produto.descricao, "\n")] = 0;

    printf("Preco por Kg: ");
    scanf("%f", &produto.preco);
    getchar();

    printf("Preco de custo por Kg: ");
    scanf("%f", &produto.preco_custo);
    getchar();

    printf("Quantidade em kg: ");
    scanf("%f", &produto.quantidade);
    getchar();

    printf("Categoria: ");
    fgets(produto.categoria, 20, stdin);
    produto.categoria[strcspn(produto.categoria, "\n")] = 0;

    obter_data_atual(produto.data_entrada);
    obter_data_validade(produto.validade);

    // Move para o final do arquivo para adicionar o novo produto
    fseek(file, 0, SEEK_END);

    // Gravação no arquivo com a quantidade em kg
    fprintf(file, "Codigo: %d\nNome: %s\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n\n",
            produto.codigo, produto.nome, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade, produto.categoria, produto.data_entrada, produto.validade);

    fclose(file);
    printf("Produto cadastrado com sucesso! Codigo atribuido: %d\n", produto.codigo);

    // Atualiza a entrada do produto no arquivo de compras
    char fornecedor[50];
    printf("Nome do fornecedor: ");
    fgets(fornecedor, 50, stdin);
    fornecedor[strcspn(fornecedor, "\n")] = 0;

    atualizarEntradaProduto(fornecedor, produto.nome, produto.data_entrada);
}

//Função de Listar as Compras Feitas pelo Hortifruti
void listarCompras() {
    Compra compra;
    FILE *file = fopen("compras.txt", "r");
    char linha[256];

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de compras.\n");
        return;
    }

    printf("=== REGISTRO DE COMPRAS ===\n\n");
    printf("+--------------+---------------+---------------+------------------+------------+---------------+---------------+-------------+----------------+\n");
    printf("| Data         | Fornecedor    | CNPJ          | Produto          | Quantidade | Preco Unit.   | Preco Total   | Recebimento | Entrada Produto|\n");
    printf("+--------------+---------------+---------------+------------------+------------+---------------+---------------+-------------+----------------+\n");

    while (1) {
        // Lê o fornecedor
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Fornecedor: %[^\n]", compra.fornecedor);

        // Lê o CNPJ
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "CNPJ: %[^\n]", compra.cnpj);

        // Lê o produto
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Produto: %[^\n]", compra.produto);

        // Lê a quantidade
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Quantidade: %d kg", &compra.quantidade);

        // Lê o preço unitário
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Preco Unitario: %f por kg", &compra.precoUnitario);

        // Lê o preço total
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Preco Total: %f", &compra.precoTotal);

        // Lê o recebimento
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Recebimento: %[^\n]", compra.recebimento);

        // Lê a entrada do produto
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Entrada Produto: %d", &compra.entradaProduto);

        // Lê a data da compra
        if (fgets(linha, sizeof(linha), file) == NULL) break;
        sscanf(linha, "Data da Compra: %[^\n]", compra.dataCompra);

        // Pula a linha em branco
        fgets(linha, sizeof(linha), file);

        printf("| %-12s | %-13.13s | %-13.13s | %-16.16s | %10d | %13.2f | %13.2f | %-11.11s | %14d |\n",
               compra.dataCompra, compra.fornecedor, compra.cnpj, compra.produto, compra.quantidade,
               compra.precoUnitario, compra.precoTotal, compra.recebimento, compra.entradaProduto);
    }

    printf("+--------------+---------------+---------------+------------------+------------+---------------+---------------+-------------+----------------+\n");

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Menu De Compras
void menuCompras() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE COMPRAS              \n");
        printf("=============================================\n");

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Registrar Compra                  |\n");
        printf("|      2. Listar Compras                    |\n");
        printf("|      3. Entrada de Produtos               |\n");
        printf("|      4. Gerar Relatorio de Compras        |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: registrarCompra(); break;
            case 2: listarCompras(); break;
            case 3: cadastrarProdutoEntrada(); break;
            case 4: gerarRelatorioCompras(); break;
            case 0: printf("Saindo do menu de compras.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}
