#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "Usuarios/usuarios.h"
#include "Uteis/uteis.h"
#include "Menus/menus.h"
#include "Login/login.h"

// Função para verificar se o arquivo de usuários está vazio
int arquivo_usuarios_vazio() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 1; // Arquivo não existe, precisa criar
    }

    fseek(arquivo, 0, SEEK_END);  // Vai para o final do arquivo
    int tamanho = ftell(arquivo); // Obtém o tamanho do arquivo
    fclose(arquivo);

    return tamanho == 0; // Se o tamanho for zero, o arquivo está vazio
}


// Função para cadastrar o primeiro usuário administrador
void cadastrar_primeiro_usuario() {
    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para cadastro do primeiro usuario!\n");
        return;
    }

    Usuario novo_usuario;
    printf("Cadastro do primeiro usuario (administrador):\n");
    printf("Nome de Usuario: ");
    scanf("%s", novo_usuario.username);
    printf("Senha do Usuario: ");
    scanf("%s", novo_usuario.password);
    printf("Nivel de Acesso (1 para administrador): ");
    scanf("%d", &novo_usuario.nivel_acesso);
    printf("CPF do Funcionario: ");
    scanf("%s", novo_usuario.cpf_funcionario);  // Adicionando CPF

    // Escreve no arquivo
    fprintf(arquivo, "%s %s %d %s\n", novo_usuario.username, novo_usuario.password, novo_usuario.nivel_acesso, novo_usuario.cpf_funcionario);
    fclose(arquivo);

    printf("Primeiro usuario cadastrado com sucesso!\n");
}


int main() {
    char usuario[50], senha[50];
    int nivel_acesso;
    char cpf[12];
    char nome[50];
    char codigoProduto[20];
    int quantidade;
    float preco;

    nomehort();

    // Verifica se o arquivo de usuários está vazio
    if (arquivo_usuarios_vazio()) {
        cadastrar_primeiro_usuario(); // Permite cadastrar o primeiro usuário
    }

    // Solicita o login
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("=====================================\n");
    printf("               LOGIN                 \n");
    printf("=====================================\n");
    printf(" Usuario: ");
    scanf("%s", usuario);
    printf(" Senha: ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    int i = 0;
    char ch;

    // Lê cada caractere da senha sem exibir
    while ((ch = _getch()) != '\r') { // '\r' captura o Enter
        if (ch == '\b' && i > 0) { // Permite apagar com o Backspace
            i--;
            printf("\b \b"); // Apaga o último caractere exibido
        } else if (ch != '\b') {
            senha[i++] = ch;
            printf("*"); // Exibe um asterisco no lugar do caractere
        }
    }
    senha[i] = '\0'; // Finaliza a string da senha

    // Valida o login
    nivel_acesso = login(usuario, senha);

    if (nivel_acesso != -1) {
        // Chama o menu principal de acordo com o nível de acesso
        limpa_tela();
        menu_principal(nivel_acesso);
    } else {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("Login invalido!.\n"); // Debug
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        getche();
    }

    return 0;
}
