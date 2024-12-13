#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "clientes.h"
#include "../Uteis/uteis.h"

// Função para cadastrar um cliente
void cadastrarCliente() {
    Cliente cliente;

    // Chamada para ler e salvar o CPF sem formatação
    ler_cpf(cliente.cpf);

    printf("Digite o nome: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = 0; // Remove a nova linha do final

    printf("Digite o endereco: ");
    fgets(cliente.endereco, sizeof(cliente.endereco), stdin);
    cliente.endereco[strcspn(cliente.endereco, "\n")] = 0; // Remove a nova linha do final

    // Chamada para ler e salvar o telefone sem formatação
    ler_telefone(cliente.telefone);

    printf("Digite o email: ");
    fgets(cliente.email, sizeof(cliente.email), stdin);
    cliente.email[strcspn(cliente.email, "\n")] = 0; // Remove a nova linha do final

    // Abrindo o arquivo para salvar o cliente em modo de adição
    FILE *file = fopen("clientes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }


    fprintf(file, "CPF: %s\n", cliente.cpf);
    fprintf(file, "Nome: %s\n", cliente.nome);
    fprintf(file, "Endereco: %s\n", cliente.endereco);
    fprintf(file, "Telefone: %s\n", cliente.telefone);
    fprintf(file, "Email: %s\n\n", cliente.email); // Linha em branco entre clientes

    // Fechando o arquivo
    fclose(file);

    printf("Cliente cadastrado com sucesso!\n");
}

//Função para editar cliente
void editarCliente() {
    char cpf[20], novoValor[100];
    int campo, encontrado = 0;
    FILE *file = fopen("clientes.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Cliente cliente;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Solicita o CPF para encontrar o cliente
    printf("Digite o CPF do cliente a ser editado: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0; // Remove o '\n' que pode vir do fgets

    // Busca o cliente no arquivo
    while (fscanf(file,
                  "CPF: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
                  cliente.cpf, cliente.nome, cliente.endereco, cliente.telefone, cliente.email) == 5) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            encontrado = 1;

            // Exibe informações do cliente encontrado
            printf("\nCliente encontrado:\n");
            printf("CPF: %s\n", cliente.cpf);
            printf("Nome: %s\n", cliente.nome);
            printf("Endereco: %s\n", cliente.endereco);
            printf("Telefone: %s\n", cliente.telefone);
            printf("Email: %s\n", cliente.email);

            // Solicita o campo a ser editado
            printf("\nEscolha um campo para editar:\n");
            printf("1. Nome\n2. Endereco\n3. Telefone\n4. Email\n");
            printf("Digite o numero do campo: ");
            scanf("%d", &campo);
            getchar(); // Limpa o '\n' deixado pelo scanf

            // Solicita o novo valor para o campo escolhido
            printf("Digite o novo valor: ");
            fgets(novoValor, sizeof(novoValor), stdin);
            novoValor[strcspn(novoValor, "\n")] = 0; // Remove o '\n' do final

            // Atualiza o campo selecionado
            switch (campo) {
                case 1: strcpy(cliente.nome, novoValor); break;
                case 2: strcpy(cliente.endereco, novoValor); break;
                case 3: strcpy(cliente.telefone, novoValor); break;
                case 4: strcpy(cliente.email, novoValor); break;
                default: printf("Opção invalida!\n"); return;
            }
        }

        // Escreve no arquivo temporário
        fprintf(tempFile, "CPF: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nEmail: %s\n\n",
                cliente.cpf, cliente.nome, cliente.endereco, cliente.telefone, cliente.email);
    }

    fclose(file);
    fclose(tempFile);

    // Atualiza o arquivo de clientes
    if (encontrado) {
        remove("clientes.txt");
        rename("temp.txt", "clientes.txt");
        printf("Cliente atualizado com sucesso.\n");
    } else {
        remove("temp.txt");
        printf("Cliente com CPF %s nao encontrado.\n", cpf);
    }

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Função para excluir um cliente atravéz do CPF
void excluirCliente() {
    char cpf[20], linha[200];
    int encontrado = 0;
    FILE *file = fopen("clientes.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o CPF do cliente a ser excluido: ");
    scanf("%s", cpf);
    getchar();  // Limpa o '\n' deixado pelo scanf

    // Processa cada linha do arquivo
    while (fgets(linha, sizeof(linha), file)) {
        // Verifica se a linha contém o CPF buscado
        if (strstr(linha, cpf) && !encontrado) {
            encontrado = 1;
            printf("\nCliente encontrado:\n");

            // Exibe as 4 linhas anteriores ao CPF (Nome, Endereço, Telefone, Email)
            char dadosCliente[4][200];
            for (int i = 0; i < 4; i++) {
                fgets(dadosCliente[i], sizeof(dadosCliente[i]), file); // Lê as linhas do cliente
                printf("%s", dadosCliente[i]); // Exibe as informações que serão excluídas
            }
            printf("%s", linha); // Exibe o CPF (última linha)

            // Pergunta se o usuário realmente deseja excluir o cliente
            char resposta;
            printf("\nTem certeza que deseja excluir este cliente? (S/N): ");
            resposta = getchar();
            getchar(); // Limpa o '\n' deixado pelo getchar

            if (resposta == 'S' || resposta == 's') {
                printf("Cliente excluido com sucesso.\n");
            } else {
                printf("Exclusao cancelada.\n");
                // Adiciona o cliente de volta ao arquivo temporário
                fprintf(tempFile, "%s", linha);
                for (int i = 0; i < 4; i++) {
                    fprintf(tempFile, "%s", dadosCliente[i]);
                }
                continue; // Retorna para o próximo cliente
            }

        } else {
            // Se não for o cliente a ser excluído, apenas copia para o arquivo temporário
            fprintf(tempFile, "%s", linha);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("clientes.txt"); // Remove o arquivo original
        rename("temp.txt", "clientes.txt"); // Renomeia o arquivo temporário para o nome original
    } else {
        remove("temp.txt"); // Remove o arquivo temporário em caso de erro
        printf("Cliente com CPF %s nao encontrado.\n", cpf);
    }

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Função de listar os Clintes
void listarClientes() {
    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    Cliente cliente;
    char linha[256];

    // Cabeçalho da tabela
    printf("+-----------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");
    printf("| CPF             | Nome                                             | Endereco                                 | Telefone        | Email                       |\n");
    printf("+-----------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");

    int count = 0;
    memset(&cliente, 0, sizeof(Cliente)); // Limpa a estrutura

    while (1) {
    memset(&cliente, 0, sizeof(Cliente)); // Limpa a estrutura

    char temp_cpf[15] = {0};
    char temp_nome[50] = {0};
    char temp_endereco[100] = {0};
    char temp_telefone[16] = {0};
    char temp_email[50] = {0};

    // Lê CPF
    if (!fgets(linha, sizeof(linha), file)) break;
    if (sscanf(linha, "CPF: %[^\n]", temp_cpf) != 1) continue;
    strncpy(cliente.cpf, temp_cpf, sizeof(cliente.cpf) - 1);
    cliente.cpf[sizeof(cliente.cpf) - 1] = '\0';

    // Lê Nome
    if (!fgets(linha, sizeof(linha), file)) break;
    if (sscanf(linha, "Nome: %[^\n]", temp_nome) != 1) continue;
    strncpy(cliente.nome, temp_nome, sizeof(cliente.nome) - 1);
    cliente.nome[sizeof(cliente.nome) - 1] = '\0';

    // Lê Endereço
    if (!fgets(linha, sizeof(linha), file)) break;
    if (sscanf(linha, "Endereco: %[^\n]", temp_endereco) != 1) continue;
    strncpy(cliente.endereco, temp_endereco, sizeof(cliente.endereco) - 1);
    cliente.endereco[sizeof(cliente.endereco) - 1] = '\0';

    // Lê Telefone
    if (!fgets(linha, sizeof(linha), file)) break;
    if (sscanf(linha, "Telefone: %[^\n]", temp_telefone) != 1) continue;
    strncpy(cliente.telefone, temp_telefone, sizeof(cliente.telefone) - 1);
    cliente.telefone[sizeof(cliente.telefone) - 1] = '\0';

    // Lê Email
    if (!fgets(linha, sizeof(linha), file)) break;
    if (sscanf(linha, "Email: %[^\n]", temp_email) != 1) continue;
    strncpy(cliente.email, temp_email, sizeof(cliente.email) - 1);
    cliente.email[sizeof(cliente.email) - 1] = '\0';

    // Pula a linha em branco
    if (!fgets(linha, sizeof(linha), file) && !feof(file)) break;

    // Aplica as máscaras apenas se os dados estiverem no formato correto
    if (strlen(cliente.cpf) == 11) {
        aplicarMascaraCpf(cliente.cpf);
    }
    if (strlen(cliente.telefone) == 11) {
        aplicarMascaraTelefone(cliente.telefone);
    }

    // Imprime o cliente formatado
    printf("| %-15.15s | %-48.48s | %-40.40s | %-15.15s | %-27.27s |\n",
           cliente.cpf,
           cliente.nome,
           cliente.endereco,
           cliente.telefone,
           cliente.email);

    printf("+-----------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");
}

    // Caso nenhum cliente tenha sido encontrado
    if (count == 0) {
        printf("| Nenhum cliente encontrado.                                                                                                                              |\n");
        printf("+-----------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");
    }

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Menu de Clientes
void menuClientes() {
    int opcao;
    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU CLIENTES             \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Cadastrar Cliente                 |\n");
        printf("|      2. Editar Cliente                    |\n");
        printf("|      3. Excluir Cliente                   |\n");
        printf("|      4. Listar Clientes                   |\n");
        printf("|      0. Voltar ao Menu Principal          |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarCliente(); break;
            case 2: editarCliente(); break;
            case 3: excluirCliente(); break;
            case 4: listarClientes(); break;
            case 0: printf("Retornando ao menu principal...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}
