#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "uteis.h"
#include <stdbool.h>

//Função para limpar a tela
void limpa_tela() {
    system("cls");
}

//Função nome do hortifruti
void nomehort(){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    printf(" ____          ____   __                 ____            _     \n");
    printf("|  _ \\  ___   |  _ \\ /_/    __ _  ___   |  _ \\ _ __ __ _| |_ ___\n");
    printf("| | | |/ _ \\  | |_) / _ \\  / _` |/ _ \\  | |_) | '__/ _` | __/ _ \\ \n");
    printf("| |_| | (_) | |  __/  __/ | (_| | (_) | |  __/| | | (_| | || (_) |\n");
    printf("|____/ \\___/  |_|   \\___|  \\__,_|\\___/  |_|   |_|  \\__,_|\\__\\___/\n\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    char dataHora[20];
    strftime(dataHora, sizeof(dataHora), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("                                  Data e Hora: %s\n\n", dataHora);

}

//Função para ler e aplicar máscara no telefone digitado
void ler_telefone(char telefone[]) {
    int i = 0;
    char ch;
    char telefone_temp[15]; // Para armazenar os números temporariamente sem formatação

    printf("Digite o telefone (somente numeros com DDD): ");

    while (i < 11) {
        ch = _getch();

        if (ch >= '0' && ch <= '9') {
            telefone_temp[i] = ch; // Armazenando o número sem formatação
            if (i == 0) {
                printf("(%c", ch); // Adiciona um parêntese no início
            } else if (i == 1) {
                printf("%c) ", ch); // Fecha o parêntese após o DDD
            } else if (i == 6) {
                printf("%c-", ch); // Adiciona um traço após o sexto dígito
            } else {
                printf("%c", ch); // Apenas exibe o dígito
            }
            i++;
        } else if (ch == '\b' && i > 0) {  // Permite apagar com backspace
            i--;
            printf("\b \b");
        }
    }
    telefone_temp[11] = '\0'; // Finaliza o telefone com \0

    // Copia o número para o array telefone, sem formatação
    strcpy(telefone, telefone_temp);
    printf("\n");
}

// Função para aplicar a máscara no telefone salvo
void aplicarMascaraTelefone(char telefone[]) {
    // Verifica se o número de telefone tem 11 dígitos
    if (strlen(telefone) == 11) {
        char telefone_formatado[16];
        snprintf(telefone_formatado, sizeof(telefone_formatado), "(%c%c) %c%c%c%c%c-%c%c%c%c",
                 telefone[0], telefone[1], telefone[2], telefone[3], telefone[4], telefone[5],
                 telefone[6], telefone[7], telefone[8], telefone[9], telefone[10]);

        strcpy(telefone, telefone_formatado);
    } else {
        printf("Erro: o número de telefone deve ter 11 dígitos.\n");
    }
}

// Função para aplicar a máscara no CPF salvo
void aplicarMascaraCpf(char cpf[]) {
    char cpf_formatado[15];
    snprintf(cpf_formatado, sizeof(cpf_formatado), "%c%c%c.%c%c%c.%c%c%c-%c%c",
             cpf[0], cpf[1], cpf[2], cpf[3], cpf[4], cpf[5],
             cpf[6], cpf[7], cpf[8], cpf[9], cpf[10]);
    strcpy(cpf, cpf_formatado);
}

//Função para ler e aplicar mascara no CPF digitado
void ler_cpf(char cpf[]) {
    int i = 0;
    char ch;
    char cpf_temp[12];  // Para armazenar apenas os números temporariamente

    printf("Digite o CPF (somente numeros): ");

    while (i < 11) {
        ch = _getch(); // Captura o caractere sem exibir na tela

        if (ch >= '0' && ch <= '9') {  // Verifica se é um número
            cpf_temp[i] = ch;  // Armazena o número
            if (i == 2 || i == 5) {
                printf("%c.", ch);  // Adiciona um ponto após o terceiro e o sexto dígitos
            } else if (i == 8) {
                printf("%c-", ch);  // Adiciona um traço após o nono dígito
            } else {
                printf("%c", ch);  // Apenas exibe o dígito
            }
            i++;
        } else if (ch == '\b' && i > 0) {  // Permite apagar com backspace
            i--;
            printf("\b \b");  // Apaga o último caractere exibido
        }
    }
    cpf_temp[11] = '\0';  // Finaliza a string com o caractere nulo

    // Copia o número para o array cpf, sem formatação
    strcpy(cpf, cpf_temp);
    printf("\n");
}

//Função para ler e aplicar mascara no CNPJ digitado
void ler_cnpj(char cnpj[]) {
    int i = 0;
    char ch;
    char cnpj_temp[14];  // Para armazenar apenas os números temporariamente

    printf("Digite o CNPJ (somente numeros): ");

    while (i < 14) {
        ch = _getch(); // Captura o caractere sem exibir na tela

        if (ch >= '0' && ch <= '9') {  // Verifica se é um número
            cnpj_temp[i] = ch;  // Armazena o número
            if (i == 2 || i == 5) {
                printf("%c.", ch);  // Adiciona um ponto após o segundo e o quinto dígito
            } else if (i == 8) {
                printf("%c/", ch);  // Adiciona uma barra após o oitavo dígito
            } else if (i == 12) {
                printf("%c-", ch);  // Adiciona um traço após o décimo segundo dígito
            } else {
                printf("%c", ch);  // Apenas exibe o dígito
            }
            i++;
        } else if (ch == '\b' && i > 0) {  // Permite apagar com backspace
            i--;
            printf("\b \b");  // Apaga o último caractere exibido
        }
    }
    cnpj_temp[14] = '\0';  // Finaliza a string com o caractere nulo

    // Copia o número para o array cnpj, sem formatação
    strcpy(cnpj, cnpj_temp);
    printf("\n");
}

//Função para aplicar mascara no CNPJ salvo
void aplicarMascaraCnpj(char cnpj[]) {
    snprintf(cnpj, 20, "%c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c",
             cnpj[0], cnpj[1], cnpj[2], cnpj[3], cnpj[4], cnpj[5], cnpj[6], cnpj[7],
             cnpj[8], cnpj[9], cnpj[10], cnpj[11], cnpj[12], cnpj[13]);
}

//Função para ler e aplicar mascara na data de nascimento digitada
void ler_data_nascimento(char *data) {
    char input[9];
    int dataValida = 0;

    while (!dataValida) {
        printf("Digite a data de nascimento (DDMMAAAA): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        // Verifica se a data tem 8 dígitos
        if (strlen(input) == 8) {
            // Formata a data no formato DD/MM/AAAA
            snprintf(data, 11, "%c%c/%c%c/%c%c%c%c",
                     input[0], input[1],
                     input[2], input[3],
                     input[4], input[5], input[6], input[7]);
            dataValida = 1; // Define como válido para sair do loop
        } else {
            printf("Data invalida! Certifique-se de digitar 8 dígitos.\n");
        }
    }
}

// Função para limpar o buffer de entrada
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para obter a data atual
void obter_data_atual(char *data_str) {
    time_t t = time(NULL);
    struct tm data_atual = *localtime(&t);
    strftime(data_str, 11, "%d/%m/%Y", &data_atual);
}

// Função para obter a validade (7 dias após a data atual)
void obter_data_validade(char *validade_str) {
    time_t t = time(NULL);
    t += 7 * 24 * 60 * 60; // Adiciona 7 dias em segundos
    struct tm data_validade = *localtime(&t);
    strftime(validade_str, 11, "%d/%m/%Y", &data_validade);
}

// Função para comparar datas no formato dd/mm/aaaa
int comparaDataVenda(char *dataVenda, char *dataInicio, char *dataFim) {
    int diaVenda, mesVenda, anoVenda;
    int diaInicio, mesInicio, anoInicio;
    int diaFim, mesFim, anoFim;

    sscanf(dataVenda, "%d/%d/%d", &diaVenda, &mesVenda, &anoVenda);
    sscanf(dataInicio, "%d/%d/%d", &diaInicio, &mesInicio, &anoInicio);
    sscanf(dataFim, "%d/%d/%d", &diaFim, &mesFim, &anoFim);

    if (anoVenda > anoInicio && anoVenda < anoFim) return 1;
    if (anoVenda == anoInicio && (mesVenda > mesInicio || (mesVenda == mesInicio && diaVenda >= diaInicio))) return 1;
    if (anoVenda == anoFim && (mesVenda < mesFim || (mesVenda == mesFim && diaVenda <= diaFim))) return 1;

    return 0;
}

// Função de comparação para qsort
int compareProdutoVenda(const void *a, const void *b) {
    const ProdutoVenda *produtoA = (const ProdutoVenda *)a;
    const ProdutoVenda *produtoB = (const ProdutoVenda *)b;

    // Ordena em ordem decrescente de quantidade vendida
    if (produtoA->quantidadeVendida > produtoB->quantidadeVendida) return -1;
    if (produtoA->quantidadeVendida < produtoB->quantidadeVendida) return 1;

    // Se as quantidades forem iguais, ordena pelo código do produto
    return produtoA->codigo - produtoB->codigo;
}
