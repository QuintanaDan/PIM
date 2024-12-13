#include <stdio.h>
#include <string.h>
#include "fornecedores.h"
#include "../Uteis/uteis.h"
#include <windows.h>

//Função para cadastrar um Fornecedor
void cadastrarFornecedor() {
    Fornecedor fornecedor;

    // Solicita o CNPJ primeiro
    printf("Digite o CNPJ: ");
    ler_cnpj(fornecedor.cnpj);

    printf("Digite o nome: ");
    fgets(fornecedor.nome, sizeof(fornecedor.nome), stdin);
    fornecedor.nome[strcspn(fornecedor.nome, "\n")] = 0;

    printf("Digite o endereco: ");
    fgets(fornecedor.endereco, sizeof(fornecedor.endereco), stdin);
    fornecedor.endereco[strcspn(fornecedor.endereco, "\n")] = 0;

    ler_telefone(fornecedor.telefone);

    printf("Digite o email: ");
    fgets(fornecedor.email, sizeof(fornecedor.email), stdin);
    fornecedor.email[strcspn(fornecedor.email, "\n")] = 0;

    // Salva os dados no arquivo
    FILE *file = fopen("fornecedores.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }

    fprintf(file, "CNPJ: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nEmail: %s\n\n",
            fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email);

    fclose(file);
    printf("Fornecedor cadastrado com sucesso!\n");
}

//Função para editar o cadastro de um fornecedor
void editarFornecedor() {
    char cnpj[20], novoValor[100];
    int campo, encontrado = 0;
    FILE *file = fopen("fornecedores.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Fornecedor fornecedor;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Solicita o CNPJ para encontrar o fornecedor
    printf("Digite o CNPJ do fornecedor a ser editado: ");
    fgets(cnpj, sizeof(cnpj), stdin);
    cnpj[strcspn(cnpj, "\n")] = 0;

    // Busca o fornecedor no arquivo
    while (fscanf(file,
                  "CNPJ: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
                  fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email) == 5) {
        if (strcmp(fornecedor.cnpj, cnpj) == 0) {
            encontrado = 1;

            // Exibe informações do fornecedor encontrado
            printf("\nFornecedor encontrado:\n");
            printf("CNPJ: %s\n", fornecedor.cnpj);
            printf("Nome: %s\n", fornecedor.nome);
            printf("Endereco: %s\n", fornecedor.endereco);
            printf("Telefone: %s\n", fornecedor.telefone);
            printf("Email: %s\n", fornecedor.email);

            // Solicita o campo a ser editado
            printf("\nEscolha um campo para editar:\n");
            printf("1. Nome\n2. Endereco\n3. Telefone\n4. Email\n");
            printf("Digite o numero do campo: ");
            scanf("%d", &campo);
            getchar();

            // Solicita o novo valor
            printf("Digite o novo valor: ");
            fgets(novoValor, sizeof(novoValor), stdin);
            novoValor[strcspn(novoValor, "\n")] = 0;

            // Atualiza o campo selecionado
            switch (campo) {
                case 1: strcpy(fornecedor.nome, novoValor); break;
                case 2: strcpy(fornecedor.endereco, novoValor); break;
                case 3: strcpy(fornecedor.telefone, novoValor); break;
                case 4: strcpy(fornecedor.email, novoValor); break;
                default: printf("Opcao invalida!\n"); return;
            }
        }

        // Escreve no arquivo temporário
        fprintf(tempFile, "CNPJ: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nEmail: %s\n\n",
                fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email);
    }

    fclose(file);
    fclose(tempFile);

    // Atualiza o arquivo de fornecedores
    if (encontrado) {
        remove("fornecedores.txt");
        rename("temp.txt", "fornecedores.txt");
        printf("Fornecedor atualizado com sucesso.\n");
    } else {
        remove("temp.txt");
        printf("Fornecedor com CNPJ %s nao encontrado.\n", cnpj);
    }
}

//Função para excluir o cadastro de um fornecedor pelo CNPJ
void excluirFornecedor() {
    char cnpj[20], resposta[3];
    int encontrado = 0;
    FILE *file = fopen("fornecedores.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Fornecedor fornecedor;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Solicita o CNPJ para excluir o fornecedor
    printf("Digite o CNPJ do fornecedor a ser excluido: ");
    fgets(cnpj, sizeof(cnpj), stdin);
    cnpj[strcspn(cnpj, "\n")] = 0;

    // Percorre os fornecedores no arquivo
    while (fscanf(file,
                  "CNPJ: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
                  fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email) == 5) {
        if (strcmp(fornecedor.cnpj, cnpj) != 0) {
            // Copia o fornecedor para o arquivo temporário se o CNPJ não corresponder
            fprintf(tempFile, "CNPJ: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nEmail: %s\n\n",
                    fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email);
        } else {
            // Mostra as informações do fornecedor encontrado
            encontrado = 1;
            printf("\nFornecedor encontrado:\n");
            printf("CNPJ: %s\n", fornecedor.cnpj);
            printf("Nome: %s\n", fornecedor.nome);
            printf("Endereco: %s\n", fornecedor.endereco);
            printf("Telefone: %s\n", fornecedor.telefone);
            printf("Email: %s\n", fornecedor.email);

            // Confirmação para excluir
            printf("\nDeseja realmente excluir este fornecedor? (s/n): ");
            fgets(resposta, sizeof(resposta), stdin);
            resposta[strcspn(resposta, "\n")] = 0;

            if (strcmp(resposta, "s") != 0 && strcmp(resposta, "S") != 0) {
                // Cancela a exclusão, copia o fornecedor para o arquivo temporário
                fprintf(tempFile, "CNPJ: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nEmail: %s\n\n",
                        fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email);
                printf("Exclusao cancelada.\n");
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    // Atualiza o arquivo original se o fornecedor foi confirmado para exclusão
    if (encontrado) {
        remove("fornecedores.txt");
        rename("temp.txt", "fornecedores.txt");
        printf("Fornecedor excluído com sucesso.\n");
    } else {
        remove("temp.txt");
        printf("Fornecedor com CNPJ %s nao encontrado.\n", cnpj);
    }
}

//Função de Listar os Fornecedores
void listarFornecedores() {
    FILE *file = fopen("fornecedores.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    Fornecedor fornecedor;

    // Cabeçalho da tabela
    printf("+--------------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");
    printf("| CNPJ               | Nome                                             | Endereco                                 | Telefone        | Email                       |\n");
    printf("+--------------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");

    int count = 0;
    while (fscanf(file,
                  "CNPJ: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nEmail: %[^\n]\n\n",
                  fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email) == 5) {

        // Aplicando a máscara no telefone e CNPJ
        aplicarMascaraTelefone(fornecedor.telefone);
        aplicarMascaraCnpj(fornecedor.cnpj);

        // Exibindo as informações formatadas
        printf("| %-16s | %-48s | %-40s | %-15s | %-27s |\n",
               fornecedor.cnpj, fornecedor.nome, fornecedor.endereco, fornecedor.telefone, fornecedor.email);
        printf("+--------------------+--------------------------------------------------+------------------------------------------+-----------------+-----------------------------+\n");
        count++;
                  }

    if (count == 0) {
        printf("| Nenhum fornecedor encontrado.                                                                                      |\n");
    }

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Menu de Fornecedores
void menuFornecedores() {
    int opcao;
    do {
        limpa_tela();
        nomehort();
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU FORNECEDORES              \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Cadastrar Fornecedor              |\n");
        printf("|      2. Editar Fornecedor                 |\n");
        printf("|      3. Excluir Fornecedor                |\n");
        printf("|      4. Listar Fornecedores               |\n");
        printf("|      0. Voltar ao Menu Principal          |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarFornecedor(); break;
            case 2: editarFornecedor(); break;
            case 3: excluirFornecedor(); break;
            case 4: listarFornecedores(); break;
            case 0: printf("Retornando ao menu principal...\n"); break;
            default: printf("Opção invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}
