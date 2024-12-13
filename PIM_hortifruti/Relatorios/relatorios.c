#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <direct.h>
#include "relatorios.h"
#include "../Uteis/uteis.h"
#include <time.h>
#include <stdlib.h>

//Função de comparação de datas
int compararDatas(const char* data1, const char* data2) {
    int dia1, mes1, ano1, dia2, mes2, ano2;
    sscanf(data1, "%d/%d/%d", &dia1, &mes1, &ano1);
    sscanf(data2, "%d/%d/%d", &dia2, &mes2, &ano2);

    if (ano1 != ano2) return ano1 - ano2;
    if (mes1 != mes2) return mes1 - mes2;
    return dia1 - dia2;
}

//Função para validar o formato da data levando em consideração a quantidade de dias dos meses e ano bissexto
int validarFormatoData(const char* data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;

    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;

    // Validação básica dos valores
    if (dia < 1 || dia > 31) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (ano < 1900) return 0;

    // Validação de meses com 30 dias
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;

    // Validação de fevereiro e ano bissexto
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > (bissexto ? 29 : 28)) return 0;
    }

    return 1;
}

// Função auxiliar para converter data string para inteiros
void quebrarData(const char* data, int* dia, int* mes, int* ano) {
    sscanf(data, "%d/%d/%d", dia, mes, ano);
}

// Função para converter data em um número para comparação
long dataParaNumero(const char* data) {
    int dia, mes, ano;
    quebrarData(data, &dia, &mes, &ano);
    return (long)ano * 10000 + mes * 100 + dia;
}

// Função para verificar se uma data está dentro de um período
int dataNoIntervalo(const char* data, const char* inicio, const char* fim) {
    long dataNum = dataParaNumero(data);
    long inicioNum = dataParaNumero(inicio);
    long fimNum = dataParaNumero(fim);

    return (dataNum >= inicioNum && dataNum <= fimNum);
}

//Função de inicialização do filtro de relatórios
void inicializarFiltro(FiltroRelatorio *filtro) {
    memset(filtro, 0, sizeof(FiltroRelatorio));
}

//Função de escolha de filtro (tipo do reátorio) podendo ser Por cliente/ fornecedor, por data, por periodo e todos os registros
void obterFiltro(FiltroRelatorio *filtro, int tipoRelatorio) {
    int opcao;
    printf("Escolha o tipo de relatorio:\n");
    printf("1. Por %s\n", tipoRelatorio == 1 ? "Cliente" : "Fornecedor");
    printf("2. Por Data Especifica\n");
    printf("3. Por Periodo\n");
    printf("4. Todos os Registros\n");
    scanf("%d", &opcao);
    getchar(); // Limpar o buffer

    switch(opcao) {
        case 1:
            strcpy(filtro->tipo, tipoRelatorio == 1 ? "PorCliente" : "PorFornecedor");
            printf("Digite o %s: ", tipoRelatorio == 1 ? "CPF do cliente" : "CNPJ do fornecedor");
            fgets(filtro->filtro, sizeof(filtro->filtro), stdin);
            filtro->filtro[strcspn(filtro->filtro, "\n")] = 0;
            break;
        case 2:
            strcpy(filtro->tipo, "PorData");
            printf("Digite a data (DD/MM/AAAA): ");
            fgets(filtro->filtro, sizeof(filtro->filtro), stdin);
            filtro->filtro[strcspn(filtro->filtro, "\n")] = 0;
            break;
        case 3:
            strcpy(filtro->tipo, "PorPeriodo");
            obterDataValida("Digite a data de inicio (DD/MM/AAAA): ", filtro->dataInicio);
            obterDataValida("Digite a data de fim (DD/MM/AAAA): ", filtro->dataFim);
            break;
        case 4:
            strcpy(filtro->tipo, "Todos");
            break;
        default:
            printf("Opcao invalida.\n");
            strcpy(filtro->tipo, "Invalido");
            return;
    }
}

//Função para obter e validar a data
void obterDataValida(const char *prompt, char *data) {
    int dataValida = 0;
    while (!dataValida) {
        printf("%s", prompt);
        fgets(data, 11, stdin);
        data[strcspn(data, "\n")] = 0;

        if (strlen(data) != 10 || !validarFormatoData(data)) {
            printf("Formato de data invalido. Use DD/MM/AAAA.\n");
        } else {
            dataValida = 1;
        }
    }
}

//Função de criação do nome do arquivo .txt, colocando o tipo de relátorio e a data invertida como nome, para melhor organização
void gerarNomeArquivo(const char *prefixo, const char *tipoRelatorio, char *nomeArquivo) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char dataAtual[11];
    strftime(dataAtual, sizeof(dataAtual), "%Y-%m-%d", tm);

    sprintf(nomeArquivo, "relatorios/%s%s_%s.txt", prefixo, tipoRelatorio, dataAtual);

    for (int i = 0; nomeArquivo[i]; i++) {
        if (nomeArquivo[i] == ':' || nomeArquivo[i] == '*' ||
            nomeArquivo[i] == '?' || nomeArquivo[i] == '"' ||
            nomeArquivo[i] == '<' || nomeArquivo[i] == '>' ||
            nomeArquivo[i] == '|') {
            nomeArquivo[i] = '_';
        }
    }
}

// Função para salvar o relatório
void salvarRelatorio(const char *nomeArquivo, const char *cabecalho, const char *conteudo) {
    #ifdef _WIN32
        _mkdir("relatorios");
    #else
        mkdir("relatorios", 0777);
    #endif

    FILE *arquivoRelatorio = fopen(nomeArquivo, "w");
    if (arquivoRelatorio == NULL) {
        printf("Erro ao criar o arquivo de relatorio.\n");
        printf("Nome do arquivo: %s\n", nomeArquivo);
        perror("Erro");
    } else {
        fprintf(arquivoRelatorio, "%s\n%s", cabecalho, conteudo);
        fclose(arquivoRelatorio);
        printf("Relatorio salvo com sucesso no arquivo: %s\n", nomeArquivo);
    }
}

//Função de relatório de Vendas
void gerarRelatorioVendas() {
    FiltroRelatorio filtro;
    inicializarFiltro(&filtro);
    obterFiltro(&filtro, 1); // 1 para relatório de vendas

    FILE *fileVendas = fopen("vendas.txt", "r");
    if (fileVendas == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    char relatorioCompleto[50000] = "";
    ResumoRelatorio resumo = {0, 0};
    char linha[256];
    int vendaID;
    char dataHora[20], clienteCPF[15];
    int encontrouRegistros = 0;

    strcat(relatorioCompleto, "\nRelatorio de Vendas:\n");
    strcat(relatorioCompleto, "==================================================================\n");

    while (fgets(linha, sizeof(linha), fileVendas)) {
        if (sscanf(linha, "ID da Venda: %d", &vendaID) == 1) {
            fgets(linha, sizeof(linha), fileVendas);
            sscanf(linha, "Data e Hora: %[^\n]", dataHora);
            fgets(linha, sizeof(linha), fileVendas);
            sscanf(linha, "Cliente CPF: %[^\n]", clienteCPF);

            char dataVenda[11];
            strncpy(dataVenda, dataHora, 10);
            dataVenda[10] = '\0';

            char *ptr = clienteCPF;
            while (*ptr == ' ') ptr++;

            int mostrarVenda = 0;

            if (strcmp(filtro.tipo, "PorCliente") == 0) {
                mostrarVenda = (strcmp(ptr, filtro.filtro) == 0);
            } else if (strcmp(filtro.tipo, "PorData") == 0) {
                mostrarVenda = (strcmp(dataVenda, filtro.filtro) == 0);
            } else if (strcmp(filtro.tipo, "PorPeriodo") == 0) {
                mostrarVenda = dataNoIntervalo(dataVenda, filtro.dataInicio, filtro.dataFim);
            } else if (strcmp(filtro.tipo, "Todos") == 0) {
                mostrarVenda = 1;
            }

            if (mostrarVenda) {
                encontrouRegistros = 1;
                char vendaInfo[2000] = "";
                sprintf(vendaInfo, "VENDA ID: %d\n", vendaID);
                sprintf(vendaInfo + strlen(vendaInfo), "Data e Hora: %s\n", dataHora);
                sprintf(vendaInfo + strlen(vendaInfo), "Cliente CPF: %s\n\n", clienteCPF);
                sprintf(vendaInfo + strlen(vendaInfo), "%-8s %-20s %-10s %-12s %-12s\n",
                        "Codigo", "Produto", "Preco/Kg", "Qtd (kg)", "Total (R$)");
                strcat(vendaInfo, "------------------------------------------------------------------\n");

                float totalVenda = 0.0;
                fgets(linha, sizeof(linha), fileVendas);

                while (fgets(linha, sizeof(linha), fileVendas) && strstr(linha, "- Produto:")) {
                    char produtoNome[50];
                    int produtoCodigo;
                    float quantidade, precoTotal;
                    sscanf(linha, "- Produto: %[^,], Codigo: %d, Quantidade: %f kg, Preco: R$ %f",
                           produtoNome, &produtoCodigo, &quantidade, &precoTotal);

                    float precoPorKg = precoTotal / quantidade;
                    char itemInfo[256];
                    sprintf(itemInfo, "%-8d %-20s %-10.2f %-12.2f %-12.2f\n",
                            produtoCodigo, produtoNome, precoPorKg, quantidade, precoTotal);
                    strcat(vendaInfo, itemInfo);

                    totalVenda += precoTotal;
                }

                strcat(vendaInfo, "------------------------------------------------------------------\n");
                char totalInfo[100];
                sprintf(totalInfo, "%-52s %-12.2f\n", "TOTAL:", totalVenda);
                strcat(vendaInfo, totalInfo);
                strcat(vendaInfo, "==================================================================\n\n");

                strcat(relatorioCompleto, vendaInfo);
                resumo.totalGeral += totalVenda;
                resumo.numeroRegistros++;
            }
        }
    }

    fclose(fileVendas);

    if (!encontrouRegistros) {
        strcat(relatorioCompleto, "Nenhum registro encontrado para os criterios especificados.\n");
    } else {
        char resumoInfo[256];
        sprintf(resumoInfo, "\nTotal geral de vendas: R$ %.2f\nNúmero total de vendas: %d\n",
                resumo.totalGeral, resumo.numeroRegistros);
        strcat(relatorioCompleto, resumoInfo);
    }

    printf("%s", relatorioCompleto);

    char criarArquivo;
    printf("\nDeseja gerar um arquivo com o relatorio? (S/N): ");
    scanf(" %c", &criarArquivo);
    getchar();

    if (criarArquivo == 'S' || criarArquivo == 's') {
        char nomeArquivo[256];
        gerarNomeArquivo("relatorioVendas", filtro.tipo, nomeArquivo);
        salvarRelatorio(nomeArquivo, "Relatorio de Vendas:", relatorioCompleto);
    }

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Função de relatório de Compras
void gerarRelatorioCompras() {
    FiltroRelatorio filtro;
    inicializarFiltro(&filtro);
    obterFiltro(&filtro, 2); // 2 for compras report

    FILE *file = fopen("compras.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de compras.\n");
        return;
    }

    char relatorioCompleto[50000] = "";
    ResumoRelatorio resumo = {0, 0};
    char linha[256];
    int registrosEncontrados = 0;

    strcat(relatorioCompleto, "\nRelatorio de Compras:\n");
    strcat(relatorioCompleto, "+--------------+---------------+----------------+------------------+------------+---------------+---------------+-------------+----------------+\n");
    strcat(relatorioCompleto, "| Data         | Fornecedor    | CNPJ           | Produto          | Quantidade | Preco Unit.   | Preco Total   | Recebimento | Entrada Produto|\n");
    strcat(relatorioCompleto, "+--------------+---------------+----------------+------------------+------------+---------------+---------------+-------------+----------------+\n");

    while (fgets(linha, sizeof(linha), file) != NULL) {
        char fornecedor[50], cnpj[20], produto[50], recebimento[20], dataCompra[11];
        int quantidade, entradaProduto;
        float precoUnitario, precoTotal;

        if (sscanf(linha, "Fornecedor: %[^\n]", fornecedor) == 1) {
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "CNPJ: %[^\n]", cnpj);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Produto: %[^\n]", produto);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Quantidade: %d kg", &quantidade);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco Unitario: %f por kg", &precoUnitario);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Preco Total: %f", &precoTotal);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Recebimento: %[^\n]", recebimento);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Entrada Produto: %d", &entradaProduto);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "Data da Compra: %[^\n]", dataCompra);

            int incluirCompra = 0;
            if (strcmp(filtro.tipo, "PorFornecedor") == 0) {
                incluirCompra = (strcmp(cnpj, filtro.filtro) == 0);
            } else if (strcmp(filtro.tipo, "PorData") == 0) {
                incluirCompra = (strcmp(dataCompra, filtro.filtro) == 0);
            } else if (strcmp(filtro.tipo, "PorPeriodo") == 0) {
                incluirCompra = dataNoIntervalo(dataCompra, filtro.dataInicio, filtro.dataFim);
            } else if (strcmp(filtro.tipo, "Todos") == 0) {
                incluirCompra = 1;
            }

            if (incluirCompra) {
                char linhaRelatorio[256];
                sprintf(linhaRelatorio, "| %-12s | %-13.13s | %-14.14s | %-16.16s | %10d | %13.2f | %13.2f | %-11.11s | %14d |\n",
                        dataCompra, fornecedor, cnpj, produto, quantidade,
                        precoUnitario, precoTotal, recebimento, entradaProduto);

                strcat(relatorioCompleto, linhaRelatorio);

                resumo.totalGeral += precoTotal;
                resumo.numeroRegistros++;
                registrosEncontrados = 1;
            }
        }
    }

    fclose(file);

    if (!registrosEncontrados) {
        strcpy(relatorioCompleto, "\nNenhum registro encontrado para os criterios especificados.\n");
    } else {
        strcat(relatorioCompleto, "+--------------+---------------+----------------+------------------+------------+---------------+---------------+-------------+----------------+\n");

        char resumoStr[256];
        sprintf(resumoStr, "\nTotal de compras: R$ %.2f\nNumero de compras: %d\n", resumo.totalGeral, resumo.numeroRegistros);
        strcat(relatorioCompleto, resumoStr);
    }

    printf("%s", relatorioCompleto);

    if (registrosEncontrados) {
        char criarArquivo;
        printf("\nDeseja gerar um arquivo com o relatorio? (S/N): ");
        scanf(" %c", &criarArquivo);
        getchar(); // Clear buffer

        if (criarArquivo == 'S' || criarArquivo == 's') {
            char nomeArquivo[256];
            gerarNomeArquivo("relatorioCompras", filtro.tipo, nomeArquivo);
            salvarRelatorio(nomeArquivo, "Relatorio de Compras:", relatorioCompleto);
        }
    }

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}


//Menu de relatórios
void menuRelatorios() {
    int opcao;
    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE RELATORIOS           \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Relatorio de Vendas               |\n");
        printf("|      2. Relatorio de Compras              |\n");
        printf("|      0. Voltar ao Menu Principal          |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {

            case 1: gerarRelatorioVendas(); break;
            case 2: gerarRelatorioCompras(); break;
            case 0: printf("Retornando ao menu principal...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}
