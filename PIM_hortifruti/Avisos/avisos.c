#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include"../Uteis/uteis.h"
#include "../Compras/compras.h"
#include <time.h>
#include "../Produtos/produtos.h"

//Função para verificar os produtos com menos de 20 Kg no estoque
void verificarEstoqueBaixo() {
    FILE *file = fopen("produtos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    // Obter a data atual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char data[11];
    sprintf(data, "%04d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

    // Criar nome do arquivo
    char nomeArquivo[100];
    sprintf(nomeArquivo, "EstoqueBaixo_%s.txt", data);

    FILE *arquivoEstoqueBaixo = fopen(nomeArquivo, "w");
    if (arquivoEstoqueBaixo == NULL) {
        printf("Erro ao criar arquivo de estoque baixo.\n");
        fclose(file);
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("\n   *Aviso de produtos com estoque baixo*   \n");
    fprintf(arquivoEstoqueBaixo, "\n   *Aviso de produtos com estoque baixo*   \n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    const char *cabecalho = "+-------------------------------------------+\n"
                            "| %-15s | %-10s | %-10s |\n"
                            "+-------------------------------------------+\n";
    printf(cabecalho, "Nome", "Codigo", "Quantidade");
    fprintf(arquivoEstoqueBaixo, cabecalho, "Nome", "Codigo", "Quantidade");

    char nome[50], categoria[30], descricao[100], dataEntrada[11], validade[11];
    int codigo;
    float preco, precoCusto, quantidade;
    int produtosEncontrados = 0;

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "Codigo: %d", &codigo) == 1) {
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Nome: %[^\n]", nome);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Descricao: %[^\n]", descricao);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco por Kg: %f", &preco);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco de Custo por Kg: %f", &precoCusto);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Quantidade (kg): %f", &quantidade);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Categoria: %[^\n]", categoria);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Data de Entrada: %[^\n]", dataEntrada);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Validade: %[^\n]", validade);

            // Pula a linha em branco
            fgets(linha, sizeof(linha), file);

            if (quantidade < 20) {
                const char *formato = "| %-15.15s | %-10d | %-10.2f |\n";
                printf(formato, nome, codigo, quantidade);
                fprintf(arquivoEstoqueBaixo, formato, nome, codigo, quantidade);
                produtosEncontrados++;
            }
        }
    }

    const char *rodape = "+-------------------------------------------+\n";
    printf(rodape);
    fprintf(arquivoEstoqueBaixo, rodape);

    if (produtosEncontrados == 0) {
        const char *mensagem = "Nenhum produto com estoque baixo encontrado.\n";
        printf("%s", mensagem);
        fprintf(arquivoEstoqueBaixo, "%s", mensagem);
    }

    fclose(file);
    fclose(arquivoEstoqueBaixo);

    printf("\nRelatorio de estoque baixo salvo em: %s\n", nomeArquivo);

    printf("\nOpcoes:\n");
    printf("1. Ir para o menu de compras\n");
    printf("2. Voltar ao menu de avisos\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);
    getchar(); // Limpar o buffer

    switch (opcao) {
        case 1:
            menuCompras();
            break;
        case 2:
            // Apenas retorna
            break;
        default:
            printf("Opcao invalida. Voltando ao menu principal.\n");
    }
}

//Função para verificar os produtos que ainda não foram dado a entrada no estoque
void listarProdutosSemEntrada() {
    Compra compra;
    FILE *file = fopen("compras.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de compras.\n");
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("=== AVISO: Produtos Sem Entrada Registrada ===\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("+--------------------------------------------------+\n");
    printf("| %-30s | %-15s |\n", "Produto", "Quantidade (kg)");
    printf("+--------------------------------------------------+\n");

    char linha[256];
    int produtosSemEntrada = 0;

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "Fornecedor: %[^\n]", compra.fornecedor) == 1) {
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "CNPJ: %[^\n]", compra.cnpj);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Produto: %[^\n]", compra.produto);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Quantidade: %d kg", &compra.quantidade);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco Unitario: %f por kg", &compra.precoUnitario);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco Total: %f", &compra.precoTotal);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Recebimento: %[^\n]", compra.recebimento);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Entrada Produto: %d", &compra.entradaProduto);

            // Pula a linha em branco
            fgets(linha, sizeof(linha), file);

            if (compra.entradaProduto == 0) {
                printf("| %-30.30s | %-15d |\n", compra.produto, compra.quantidade);
                printf("+--------------------------------------------------+\n");
                produtosSemEntrada++;
            }
        }
    }

    if (produtosSemEntrada == 0) {
        printf("Nao ha produtos sem entrada registrada.\n");
    } else {
        printf("\nTotal de produtos sem entrada: %d\n", produtosSemEntrada);
    }

    fclose(file);

    printf("\nOpcoes:\n");
    printf("1. Ir para o menu de compras\n");
    printf("2. Voltar ao menu de avisos\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);
    getchar(); // Limpar o buffer

    switch (opcao) {
        case 1:
            menuCompras();
            break;
        case 2:
            // Apenas retorna
            break;
        default:
            printf("Opcao invalida. Voltando ao menu principal.\n");
    }
}


// Função auxiliar para converter string de data em time_t
time_t converterData(const char *data) {
    struct tm tm_data = {0};
    sscanf(data, "%d/%d/%d", &tm_data.tm_mday, &tm_data.tm_mon, &tm_data.tm_year);
    tm_data.tm_mon -= 1;  // Mês começa do 0
    tm_data.tm_year -= 1900;  // Anos desde 1900
    return mktime(&tm_data);
}

//Função para verificar produtos proximo ao vencimento
void verificarProdutosVencimento() {
    FILE *file = fopen("produtos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    // Obtém a data atual
    time_t hoje = time(NULL);
    struct tm *tm_hoje = localtime(&hoje);
    // Avança para o dia seguinte
    tm_hoje->tm_mday += 1;
    time_t amanha = mktime(tm_hoje);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("\n   *Aviso de produtos próximos ao vencimento*   \n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("+------------------------------------------------------------------+\n");
    printf("| %-15s | %-10s | %-15s | %-15s |\n", "Nome", "Codigo", "Quantidade", "Validade");
    printf("+------------------------------------------------------------------+\n");

    char nome[50], categoria[30], descricao[100], dataEntrada[11], validade[11];
    int codigo;
    float preco, precoCusto, quantidade;
    int produtosEncontrados = 0;

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "Codigo: %d", &codigo) == 1) {
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Nome: %[^\n]", nome);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Descricao: %[^\n]", descricao);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco por Kg: %f", &preco);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco de Custo por Kg: %f", &precoCusto);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Quantidade (kg): %f", &quantidade);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Categoria: %[^\n]", categoria);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Data de Entrada: %[^\n]", dataEntrada);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Validade: %[^\n]", validade);

            // Pula a linha em branco
            fgets(linha, sizeof(linha), file);

            // Converte a data de validade para time_t
            time_t data_validade = converterData(validade);

            // Verifica se o produto vence amanhã
            if (data_validade <= amanha && data_validade >= hoje) {
                printf("| %-15.15s | %-10d | %-15.2f | %-15s |\n",
                       nome, codigo, quantidade, validade);
                produtosEncontrados++;
            }
        }
    }

    if (produtosEncontrados == 0) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("| Nenhum produto proximo ao vencimento encontrado                        |\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    printf("+------------------------------------------------------------------+\n");

    fclose(file);
    printf("\nOpcoes:\n");
    printf("1. Ir para o menu de produtos\n");
    printf("2. Voltar ao menu de avisos\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);
    getchar(); // Limpar o buffer

    switch (opcao) {
        case 1:
            menuProdutos();
            break;
        case 2:
            // Apenas retorna
            break;
        default:
            printf("Opcao invalida. Voltando ao menu principal.\n");
    }
}

//Menu de Avisos
void menuAvisos() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Defina cor para o título em verde brilhante
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n=============================================\n");
        printf("                 MENU DE AVISOS              \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Estoque Baixo                     |\n");
        printf("|      2. Listar Produtos Sem Entrada       |\n");
        printf("|      3. Produtos Perto do Vencimento      |\n");
        printf("|      0. Voltar ao Menu Principal          |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer(); // Limpar o buffer

        switch (opcao) {
            case 1: verificarEstoqueBaixo(); break;
            case 2: listarProdutosSemEntrada(); break;
            case 3: verificarProdutosVencimento(); break;
            case 0: printf("Voltando ao menu principal.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}
