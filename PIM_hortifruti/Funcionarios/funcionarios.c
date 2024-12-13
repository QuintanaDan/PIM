#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "funcionarios.h"
#include "../Uteis/uteis.h"

//Função para Cadastrar um Funcionário
void cadastrarFuncionario() {
    Funcionario funcionario;

    ler_cpf(funcionario.cpf);

    // Solicita os outros dados do funcionário
    printf("Digite o nome: ");
    fgets(funcionario.nome, sizeof(funcionario.nome), stdin);
    funcionario.nome[strcspn(funcionario.nome, "\n")] = 0;

    printf("Digite o endereco: ");
    fgets(funcionario.endereco, sizeof(funcionario.endereco), stdin);
    funcionario.endereco[strcspn(funcionario.endereco, "\n")] = 0;

    // Leitura do telefone
    ler_telefone(funcionario.telefone);

    printf("Digite o genero: ");
    fgets(funcionario.genero, sizeof(funcionario.genero), stdin);
    funcionario.genero[strcspn(funcionario.genero, "\n")] = 0;

    // Limpa buffer antes de ler a data de nascimento
    limpar_buffer();
    ler_data_nascimento(funcionario.dataNascimento);

    // Limpa buffer antes de ler o cargo
    printf("Digite o cargo: ");
    limpar_buffer();
    fgets(funcionario.cargo, sizeof(funcionario.cargo), stdin);
    funcionario.cargo[strcspn(funcionario.cargo, "\n")] = 0;

    // Leitura do salário usando scanf e limpeza do buffer
    printf("Digite o salario: ");
    if (scanf("%f", &funcionario.salario) != 1) {
        printf("Erro ao ler o salario. Certifique-se de que eh um numero valido.\n");
        funcionario.salario = 0.0;
    }
    limpar_buffer(); // Limpa o buffer após a leitura do salário

    // Salva os dados no arquivo
    FILE *file = fopen("funcionarios.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }

    fprintf(file, "CPF: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nGenero: %s\nData de Nascimento: %s\nCargo: %s\nSalario: %.2f\n\n",
            funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone,
            funcionario.genero, funcionario.dataNascimento, funcionario.cargo, funcionario.salario);

    fclose(file);
    printf("Funcionario cadastrado com sucesso!\n");
}

//Função para editar Funcionário
void editarFuncionario() {
    char cpf[15], novoValor[100];
    int campo, encontrado = 0;
    FILE *file = fopen("funcionarios.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Funcionario funcionario;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    // Solicita o CPF para encontrar o funcionário
    printf("Digite o CPF do funcionario a ser editado: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    // Busca o funcionário no arquivo
    while (fscanf(file,
                  "CPF: %s\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nGenero: %[^\n]\nData de Nascimento: %[^\n]\nCargo: %[^\n]\nSalario: %f\n\n",
                  funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone, funcionario.genero,
                  funcionario.dataNascimento, funcionario.cargo, &funcionario.salario) == 8) {

        if (strcmp(funcionario.cpf, cpf) == 0) {
            encontrado = 1;

            // Exibe informações do funcionário encontrado
            printf("\nFuncionario encontrado:\n");
            printf("Nome: %s\n", funcionario.nome);
            printf("CPF: %s\n", funcionario.cpf);
            printf("Endereco: %s\n", funcionario.endereco);
            printf("Telefone: %s\n", funcionario.telefone);
            printf("Genero: %s\n", funcionario.genero);
            printf("Data de Nascimento: %s\n", funcionario.dataNascimento);
            printf("Cargo: %s\n", funcionario.cargo);
            printf("Salario: %.2f\n", funcionario.salario);

            // Solicita o campo a ser editado
            printf("\nEscolha um campo para editar:\n");
            printf("1. Nome\n2. Endereco\n3. Telefone\n4. Genero\n5. Data de Nascimento\n6. Cargo\n7. Salario\n");
            printf("Digite o numero do campo: ");
            scanf("%d", &campo);
            getchar();  // Limpa o buffer após a leitura do campo

            // Solicita o novo valor
            printf("Digite o novo valor: ");
            fgets(novoValor, sizeof(novoValor), stdin);
            novoValor[strcspn(novoValor, "\n")] = 0;

            // Atualiza o campo selecionado
            switch (campo) {
                case 1: strcpy(funcionario.nome, novoValor); break;
                case 2: strcpy(funcionario.endereco, novoValor); break;
                case 3: strcpy(funcionario.telefone, novoValor); break;
                case 4: strcpy(funcionario.genero, novoValor); break;
                case 5: strcpy(funcionario.dataNascimento, novoValor); break;
                case 6: strcpy(funcionario.cargo, novoValor); break;
                case 7:
                    if (sscanf(novoValor, "%f", &funcionario.salario) != 1) {
                        printf("Valor invalido para o salario.\n");
                        fclose(file);
                        fclose(tempFile);
                        remove("temp.txt");
                        return;
                    }
                    break;
                default:
                    printf("Opcao invalida!\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("temp.txt");
                    return;
            }
        }

        // Escreve os dados no arquivo temporário
        fprintf(tempFile, "CPF: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nGenero: %s\nData de Nascimento: %s\nCargo: %s\nSalario: %.2f\n\n",
                funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone,
                funcionario.genero, funcionario.dataNascimento, funcionario.cargo, funcionario.salario);
    }

    fclose(file);
    fclose(tempFile);

    // Atualiza o arquivo de funcionários
    if (encontrado) {
        remove("funcionarios.txt");  // Apaga o arquivo original
        rename("temp.txt", "funcionarios.txt");  // Renomeia o arquivo temporário para o nome original
        printf("Funcionario atualizado com sucesso.\n");
    } else {
        remove("temp.txt");  // Apaga o arquivo temporário se o funcionário não for encontrado
        printf("Funcionario com CPF %s nao encontrado.\n", cpf);
    }
}

//Função para excluir um Funcionário
void excluirFuncionario() {
    char cpf[12], resposta[3];
    int encontrado = 0;
    FILE *file = fopen("funcionarios.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Funcionario funcionario;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Solicita o CPF para excluir o funcionário
    printf("Digite o CPF do funcionario a ser excluido: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    // Percorre os funcionários no arquivo com o formato correto
    while (fscanf(file,
                  "CPF: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nGenero: %[^\n]\nData de Nascimento: %[^\n]\nCargo: %[^\n]\nSalario: %f\n\n",
                  funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone, funcionario.genero,
                  funcionario.dataNascimento, funcionario.cargo, &funcionario.salario) == 8) {

        // Comparação do CPF
        if (strcmp(funcionario.cpf, cpf) == 0) {
            encontrado = 1;

            // Mostra as informações do funcionário encontrado
            printf("\nFuncionario encontrado:\n");
            printf("CPF: %s\n", funcionario.cpf);
            printf("Nome: %s\n", funcionario.nome);
            printf("Endereco: %s\n", funcionario.endereco);
            printf("Telefone: %s\n", funcionario.telefone);
            printf("Genero: %s\n", funcionario.genero);
            printf("Data de Nascimento: %s\n", funcionario.dataNascimento);
            printf("Cargo: %s\n", funcionario.cargo);
            printf("Salario: %.2f\n", funcionario.salario);

            // Confirmação para excluir
            limpar_buffer();  // Limpa o buffer antes de solicitar a confirmação
            printf("\nDeseja realmente excluir este funcionario? (s/n): ");
            fgets(resposta, sizeof(resposta), stdin);
            resposta[strcspn(resposta, "\n")] = 0;

            if (strcmp(resposta, "n") == 0) {
                // Não exclui e copia o funcionário de volta ao arquivo temporário
                fprintf(tempFile, "CPF: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nGenero: %s\nData de Nascimento: %s\nCargo: %s\nSalario: %.2f\n\n",
                        funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone,
                        funcionario.genero, funcionario.dataNascimento, funcionario.cargo, funcionario.salario);
            } else {
                printf("Funcionario excluido com sucesso.\n");
            }
        } else {
            // Copia o funcionário que não foi excluído para o arquivo temporário
            fprintf(tempFile, "CPF: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nGenero: %s\nData de Nascimento: %s\nCargo: %s\nSalario: %.2f\n\n",
                    funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone,
                    funcionario.genero, funcionario.dataNascimento, funcionario.cargo, funcionario.salario);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("funcionarios.txt");
        rename("temp.txt", "funcionarios.txt");
    } else {
        remove("temp.txt");
        printf("Funcionario com CPF %s nao encontrado.\n", cpf);
    }

    printf("Pressione Enter para retornar ao menu...");
    limpar_buffer();
    getchar();
}

//Função para Listar Funcionarios
void listarFuncionarios() {
    FILE *file = fopen("funcionarios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Funcionario funcionario;

    // Cabeçalho da tabela
    printf("+--------------+---------------------------+------------------------------------------+-----------------+-----------+--------------------+------------------+------------+\n");
    printf("| CPF          | Nome                      | Endereco                                 | Telefone        | Genero    | Data de Nascimento | Cargo            | Salario    |\n");
    printf("+--------------+---------------------------+------------------------------------------+-----------------+-----------+--------------------+------------------+------------+\n");

    int count = 0;
    while (fscanf(file,
                  "CPF: %[^\n]\nNome: %[^\n]\nEndereco: %[^\n]\nTelefone: %[^\n]\nGenero: %[^\n]\nData de Nascimento: %[^\n]\nCargo: %[^\n]\nSalario: %f\n\n",
                  funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone, funcionario.genero,
                  funcionario.dataNascimento, funcionario.cargo, &funcionario.salario) == 8) {

        // Exibindo as informações formatadas
        printf("| %-12s | %-25s | %-40s | %-15s | %-8s | %-18s | %-16s | %-10.2f |\n",
               funcionario.cpf, funcionario.nome, funcionario.endereco, funcionario.telefone,
               funcionario.genero, funcionario.dataNascimento, funcionario.cargo, funcionario.salario);
        printf("+--------------+---------------------------+------------------------------------------+-----------------+-----------+--------------------+------------------+------------+\n");
        count++;
    }

    if (count == 0) {
        printf("| Nenhum funcionario encontrado.                                                                                     |\n");
        printf("+--------------+---------------------------+------------------------------------------+-----------------+-----------+--------------------+------------------+------------+\n");
    }

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Menu de Funcionários
void menuFuncionarios() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("              MENU DE FUNCIONARIOS           \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Cadastrar Funcionario             |\n");
        printf("|      2. Editar Funcionario                |\n");
        printf("|      3. Excluir Funcionario               |\n");
        printf("|      4. Listar Funcionarios               |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do stdin

        switch (opcao) {
            case 1: cadastrarFuncionario(); break;
            case 2: editarFuncionario(); break;
            case 3: excluirFuncionario(); break;
            case 4: listarFuncionarios(); break;
            case 0: printf("Saindo do menu de funcionarios.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}
