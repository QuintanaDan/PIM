#include <locale.h>
#include <stdio.h>>
#include <string.h>
#include <conio.h>
#include "login.h"

//Função de login
int login(char *usuario, char *senha) {
    setlocale(LC_ALL, " ");
    FILE *arquivo = fopen("usuarios.txt", "r");
    char linha[100], usr[50], pwd[50];
    int nivel_acesso;
    char cpf_funcionario[12];

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de usuarios!\n");
        return -1;
    }

    // Lê cada linha e tenta fazer a correspondência
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%s %s %d %s", usr, pwd, &nivel_acesso, cpf_funcionario) == 4) { // Agora lê o CPF também
            printf("Lendo usuario: %s, senha: %s, nivel: %d, CPF: %s\n", usr, pwd, nivel_acesso, cpf_funcionario); // Debug

            // Verifica credenciais
            if (strcmp(usr, usuario) == 0 && strcmp(pwd, senha) == 0) {
                // Verifica se o CPF do usuário existe no arquivo de funcionários
                if (verificarFuncionarioExistente(cpf_funcionario)) {
                    fclose(arquivo);
                    printf("Login bem-sucedido! Nivel de acesso: %d\n", nivel_acesso); // Debug
                    return nivel_acesso;
                } else {
                    printf("Erro: CPF não encontrado no cadastro de funcionarios.\n");
                    fclose(arquivo);
                    return -1;
                }
            }
        } else {
            printf("Formato inesperado na linha do arquivo.\n"); // Debug
        }
    }

    fclose(arquivo);
    printf("Nenhum usuario encontrado com as credenciais fornecidas.\n"); // Debug
    return -1;
}

//Função para ocultar a senha
void ocultarSenha(char *senha, int tamanho) {
    int i = 0;
    char ch;

    while (i < tamanho - 1) {
        ch = _getch();
        if (ch == '\r') {
            break;
        } else if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else {
            senha[i++] = ch;
            printf("*");
        }
    }
    senha[i] = '\0';
    printf("\n");
}

//Função para Verificar se o funcionário que será ligado ao usuário existe
int verificarFuncionarioExistente(char *cpf) {
    FILE *arquivo = fopen("funcionarios.txt", "r");
    char linha[200], cpf_funcionario[12];

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de funcionarios!\n");
        return 0; // Retorna falso se o arquivo não abrir
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "CPF: %s", cpf_funcionario) == 1) {
            if (strcmp(cpf_funcionario, cpf) == 0) {
                fclose(arquivo);
                return 1; // CPF encontrado no arquivo de funcionários
            }
        }
    }

    fclose(arquivo);
    return 0; // CPF não encontrado
}
