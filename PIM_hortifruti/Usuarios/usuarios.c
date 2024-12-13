#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "usuarios.h"
#include "../Uteis/uteis.h"

#define MAX_USUARIOS 100

Usuario usuarios[MAX_USUARIOS];
int total_usuarios = 0;

// Função para cadastrar um usuário
void cadastrarUsuario() {
    if (total_usuarios >= MAX_USUARIOS) {
        printf("Limite de usuarios atingido!\n");
        return;
    }

    Usuario novo_usuario;
    printf("Nome de Usuario: ");
    scanf("%s", novo_usuario.username);
    printf("Senha do Usuario: ");
    scanf("%s", novo_usuario.password);

    // Solicita o CPF do funcionário associado
    printf("Digite o CPF do funcionario: ");
    scanf("%s", novo_usuario.cpf_funcionario);

    // Solicita o nível de acesso com verificação de valores entre 1 e 4
    do {
        printf("Nivel de Acesso (1 a 4): ");
        scanf("%d", &novo_usuario.nivel_acesso);
        if (novo_usuario.nivel_acesso < 1 || novo_usuario.nivel_acesso > 4) {
            printf("Nivel de acesso invalido. Insira um valor entre 1 e 4.\n");
        }
    } while (novo_usuario.nivel_acesso < 1 || novo_usuario.nivel_acesso > 4);

    // Adiciona o novo usuário ao array
    usuarios[total_usuarios] = novo_usuario;
    total_usuarios++;

    // Abre o arquivo para adicionar o novo usuário no formato desejado
    FILE *file = fopen("usuarios.txt", "a"); // "a" abre o arquivo para adicionar dados ao final
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    // Salva o usuário no arquivo com o formato especificado, incluindo CPF
    fprintf(file, "%s %s %d %s\n", novo_usuario.username, novo_usuario.password, novo_usuario.nivel_acesso, novo_usuario.cpf_funcionario);
    fclose(file);

    printf("Usuario cadastrado com sucesso!\n");
}

//Função para carregar os usuarios no sistema
void carregarUsuarios() {
    FILE *file = fopen("usuarios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    total_usuarios = 0;
    while (fscanf(file, "%s %s %d\n", usuarios[total_usuarios].username, usuarios[total_usuarios].password, &usuarios[total_usuarios].nivel_acesso) == 3) {
        total_usuarios++;
    }
    fclose(file);
}

//Função de Salvar usuario
void salvarUsuarios() {
    FILE *file = fopen("usuarios.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios para salvar.\n");
        return;
    }

    for (int i = 0; i < total_usuarios; i++) {
        fprintf(file, "%s %s %d\n", usuarios[i].username, usuarios[i].password, usuarios[i].nivel_acesso);
    }
    fclose(file);
}

// Função para editar um usuário
void editarUsuario() {
    carregarUsuarios();  // Carrega todos os usuários do arquivo

    char nomeBusca[50];
    int encontrado = -1;

    printf("Digite o nome do usuario que deseja editar: ");
    scanf("%s", nomeBusca);

    // Procura o usuário pelo nome
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].username, nomeBusca) == 0) {
            encontrado = i;
            printf("\nUsuario encontrado:\n");
            printf("Nome: %s\nSenha: %s\nNivel de Acesso: %d\n", usuarios[i].username, usuarios[i].password, usuarios[i].nivel_acesso);
            break;
        }
    }

    if (encontrado == -1) {
        printf("Usuario nao encontrado.\n");
        return;
    }

    // Confirmação para edição
    char confirmacao;
    printf("Deseja editar esse usuario? (s/n): ");
    scanf(" %c", &confirmacao);

    if (confirmacao != 's' && confirmacao != 'S') {
        printf("Edicao cancelada.\n");
        return;
    }

    // Escolha do campo para editar
    int opcao;
    printf("\nQual dado deseja editar?\n");
    printf("1. Nome\n2. Senha\n3. Nivel de Acesso\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    // Edita o campo escolhido
    switch (opcao) {
        case 1:
            printf("Novo Nome: ");
            scanf("%s", usuarios[encontrado].username);
            break;
        case 2:
            printf("Nova Senha: ");
            scanf("%s", usuarios[encontrado].password);
            break;
        case 3:
            do {
                printf("Novo Nivel de Acesso (1 a 4): ");
                scanf("%d", &usuarios[encontrado].nivel_acesso);
                if (usuarios[encontrado].nivel_acesso < 1 || usuarios[encontrado].nivel_acesso > 4) {
                    printf("Nivel de acesso invalido. Insira um valor entre 1 e 4.\n");
                }
            } while (usuarios[encontrado].nivel_acesso < 1 || usuarios[encontrado].nivel_acesso > 4);
            break;
        default:
            printf("Opcao invalida. Edicao cancelada.\n");
            return;
    }

    // Salva as mudanças no arquivo
    salvarUsuarios();
    printf("Usuario editado com sucesso!\n");
}

// Função para excluir um usuário pelo nome
void excluirUsuario() {
    carregarUsuarios();  // Carrega todos os usuários do arquivo

    char nomeBusca[50];
    int encontrado = -1;

    printf("Digite o nome do usuario que deseja excluir: ");
    scanf("%s", nomeBusca);

    // Procura o usuário pelo nome
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].username, nomeBusca) == 0) {
            encontrado = i;
            printf("\nUsuario encontrado:\n");
            printf("Nome: %s\nSenha: %s\nNivel de Acesso: %d\n", usuarios[i].username, usuarios[i].password, usuarios[i].nivel_acesso);
            break;
        }
    }

    if (encontrado == -1) {
        printf("Usuario nao encontrado.\n");
        return;
    }

    // Confirmação para exclusão
    char confirmacao;
    printf("Deseja realmente excluir esse usuario? (s/n): ");
    scanf(" %c", &confirmacao);

    if (confirmacao != 's' && confirmacao != 'S') {
        printf("Exclusao cancelada.\n");
        return;
    }

    // Move todos os usuários seguintes uma posição para cima
    for (int i = encontrado; i < total_usuarios - 1; i++) {
        usuarios[i] = usuarios[i + 1];
    }

    total_usuarios--;  // Decrementa o total de usuários

    // Salva os usuários atualizados no arquivo
    salvarUsuarios();
    printf("Usuario excluido com sucesso!\n");
}

// Função para listar todos os usuários
void listarUsuarios() {
    FILE *file = fopen("usuarios.txt", "r");  // Abre o arquivo para leitura
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    // Imprime o cabeçalho do grid
    printf("\n--- Lista de Usuarios ---\n");
    printf("+-----------------+-----------------+----------------+-------------------+\n");
    printf("| %-15s | %-15s | %-14s | %-17s |\n", "Nome", "Senha", "Nivel de Acesso", "CPF");
    printf("+-----------------+-----------------+----------------+-------------------+\n");

    // Variáveis para armazenar os dados de cada usuário
    char username[50], password[50], cpf[12];
    int nivel_acesso;

    // Lê cada linha do arquivo e exibe os dados em formato de grid
    while (fscanf(file, "%s %s %d %s\n", username, password, &nivel_acesso, cpf) == 4) {
        printf("| %-15s | %-15s | %-14d | %-17s |\n", username, password, nivel_acesso, cpf);
        printf("+-----------------+-----------------+----------------+-------------------+\n");
    }

    fclose(file);  // Fecha o arquivo

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
    getchar();
}

// Menu de gerenciamento de usuários
void menuUsuarios() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE USUARIOS             \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("| 1. Cadastrar Usuario                        |\n");
        printf("| 2. Editar Usuario                           |\n");
        printf("| 3. Excluir Usuario                          |\n");
        printf("| 4. Listar Usuarios                          |\n");
        printf("| 0. Sair                                     |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarUsuario(); break;
            case 2: editarUsuario(); break;
            case 3: excluirUsuario(); break;
            case 4: listarUsuarios(); break;
            case 0: printf("Saindo do gerenciamento de usuarios...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}
