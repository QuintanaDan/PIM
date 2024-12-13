#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "produtos.h"
#include <time.h>
#include "../Uteis/uteis.h"

//Função para cadastrar produtos
void cadastrarProduto() {
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
    scanf("%f", &produto.quantidade); // Alterado para float
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
    printf("Produto cadastrado com sucesso! Codigo atribuído: %d\n", produto.codigo);
}

//Função para dar saída em produto
void saidaProduto() {
    int codigo;
    float quantidadeSaida;
    Produto produto;
    int encontrado = 0;  // Variável para verificar se o produto foi encontrado

    FILE *file = fopen("produtos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o Codigo do produto para saida: ");
    scanf("%d", &codigo);

    // Ler o arquivo para verificar o produto
    while (fscanf(file, "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                  &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo, &produto.quantidade,
                  produto.categoria, produto.data_entrada, produto.validade) == 9) {

        if (produto.codigo == codigo) {
            encontrado = 1;

            // Exibir o produto encontrado
            printf("\nProduto encontrado:\n");
            printf("Nome: %s\nCodigo: %d\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade em Estoque (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n",
                   produto.nome, produto.codigo, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade,
                   produto.categoria, produto.data_entrada, produto.validade);

            // Perguntar se o usuário deseja dar saída no produto
            char confirmacao;
            printf("\nDeseja dar saida neste produto? (s/n): ");
            getchar();  // Limpar o buffer do enter
            scanf("%c", &confirmacao);

            if (confirmacao != 's' && confirmacao != 'S') {
                printf("Saida cancelada.\n");
                fclose(file);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            printf("Digite a quantidade a ser retirada do estoque (em kg): ");
            scanf("%f", &quantidadeSaida);

            // Verifica se a quantidade solicitada é maior do que a disponível
            if (quantidadeSaida > produto.quantidade) {
                printf("Quantidade solicitada maior do que a disponivel em estoque.\n");
                fclose(file);
                fclose(temp);
                remove("temp.txt");
                return;
            }
            produto.quantidade -= quantidadeSaida;  // Atualiza a quantidade
            printf("Saida registrada com sucesso para o produto com Codigo %d. Quantidade atualizada para %.2f kg.\n", codigo, produto.quantidade);
        }

        // Escreve no arquivo temporário
        fprintf(temp, "Codigo: %d\nNome: %s\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n\n",
                produto.codigo, produto.nome, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade,
                produto.categoria, produto.data_entrada, produto.validade);
    }

    if (!encontrado) {
        printf("Produto com Codigo %d não encontrado no estoque.\n", codigo);
    }

    fclose(file);
    fclose(temp);

    // Substitui o arquivo original apenas se pelo menos um produto foi encontrado e atualizado
    if (encontrado) {
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
    } else {
        remove("temp.txt");
    }
}

//Função para editar um produto
void editarProduto() {
    int codigo, opcao, encontrado = 0;
    char novoValor[100];
    Produto produto;

    FILE *file = fopen("produtos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o codigo do produto a ser editado: ");
    scanf("%d", &codigo);
    getchar();

    while (fscanf(file, "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                  &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo, &produto.quantidade,
                  produto.categoria, produto.data_entrada, produto.validade) == 9) {

        if (produto.codigo == codigo) {
            encontrado = 1;
            printf("\nProduto encontrado. Escolha o campo para editar:\n");
            printf("1. Nome\n2. Descricao\n3. Preco por Kg\n4. Preco de Custo por Kg\n5. Quantidade em Estoque (kg)\n6. Categoria\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            getchar();

            printf("Novo valor: ");
            fgets(novoValor, sizeof(novoValor), stdin);
            novoValor[strcspn(novoValor, "\n")] = 0;

            switch (opcao) {
                case 1:
                    strcpy(produto.nome, novoValor);
                    break;
                case 2:
                    strcpy(produto.descricao, novoValor);
                    break;
                case 3:
                    if (sscanf(novoValor, "%f", &produto.preco) != 1) {
                        printf("Valor invalido para o preco por Kg.\n");
                        fclose(file);
                        fclose(temp);
                        remove("temp.txt");
                        return;
                    }
                    break;
                case 4:
                    if (sscanf(novoValor, "%f", &produto.preco_custo) != 1) {
                        printf("Valor invalido para o preco de custo por Kg.\n");
                        fclose(file);
                        fclose(temp);
                        remove("temp.txt");
                        return;
                    }
                    break;
                case 5:
                    if (sscanf(novoValor, "%f", &produto.quantidade) != 1) {
                        printf("Valor invalido para a quantidade em estoque.\n");
                        fclose(file);
                        fclose(temp);
                        remove("temp.txt");
                        return;
                    }
                    break;
                case 6:
                    strcpy(produto.categoria, novoValor);
                    break;
                default:
                    printf("Opcao invalida!\n");
                    fclose(file);
                    fclose(temp);
                    remove("temp.txt");
                    return;
            }
        }

        fprintf(temp, "Codigo: %d\nNome: %s\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n\n",
                produto.codigo, produto.nome, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade,
                produto.categoria, produto.data_entrada, produto.validade);
    }

    fclose(file);
    fclose(temp);

    if (encontrado) {
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
        printf("Produto atualizado com sucesso.\n");
    } else {
        remove("temp.txt");
        printf("Produto com codigo %d não encontrado.\n", codigo);
    }
}

//Função para excluir um produto
void excluirProduto() {
    int codigo, encontrado = 0;
    Produto produto;
    char confirmacao;

    FILE *file = fopen("produtos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o Codigo do produto a ser excluido: ");
    scanf("%d", &codigo);
    getchar();  // Limpar o buffer do scanf

    while (fscanf(file, "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                  &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo, &produto.quantidade,
                  produto.categoria, produto.data_entrada, produto.validade) == 9) {

        if (produto.codigo == codigo) {
            encontrado = 1;

            // Exibir o produto antes de confirmar exclusão
            printf("\nProduto encontrado:\n");
            printf("Nome: %s\nCodigo: %d\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n",
                   produto.nome, produto.codigo, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade, produto.categoria,
                   produto.data_entrada, produto.validade);

            // Perguntar se o usuário tem certeza da exclusão
            printf("\nTem certeza que deseja excluir este produto? (s/n): ");
            scanf(" %c", &confirmacao);  // Nota: Espaço antes de %c para capturar qualquer caractere de nova linha residual

            if (confirmacao != 's' && confirmacao != 'S') {
                printf("Exclusao cancelada.\n");
                fclose(file);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            // Caso o produto seja excluído, não o escreve no arquivo temporário
            printf("Produto com Codigo %d excluido com sucesso.\n", codigo);
        } else {
            // Se o produto não for o escolhido, escreve-o no arquivo temporário
            fprintf(temp, "Codigo: %d\nNome: %s\nDescricao: %s\nPreco por Kg: %.2f\nPreco de Custo por Kg: %.2f\nQuantidade (kg): %.2f\nCategoria: %s\nData de Entrada: %s\nValidade: %s\n\n",
                    produto.codigo, produto.nome, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade,
                    produto.categoria, produto.data_entrada, produto.validade);
        }
    }

    fclose(file);
    fclose(temp);

    if (encontrado) {
        // Substituir o arquivo original pelo temporário
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
    } else {
        remove("temp.txt");
        printf("Produto com Codigo %d nao encontrado.\n", codigo);
    }
}

//Função para listar os produtos do estoque
void listarProdutos() {
    FILE *file = fopen("produtos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    Produto produto;

    // Exibição do cabeçalho
    printf("+--------+--------------------+-------------------------+----------+----------+-----------------+---------------+------------+------------+\n");
    printf("| Codigo | Nome               | Descricao               | Preco Kg | Custo Kg | Quantidade (kg) | Categoria     |  Entrada   |  Validade  |\n");
    printf("+--------+--------------------+-------------------------+----------+----------+-----------------+---------------+------------+------------+\n");

    int count = 0; // Contador para verificar quantos produtos foram lidos
    while (fscanf(file, "Codigo: %d\nNome: %[^\n]\nDescricao: %[^\n]\nPreco por Kg: %f\nPreco de Custo por Kg: %f\nQuantidade (kg): %f\nCategoria: %[^\n]\nData de Entrada: %[^\n]\nValidade: %[^\n]\n\n",
                  &produto.codigo, produto.nome, produto.descricao, &produto.preco, &produto.preco_custo, &produto.quantidade, produto.categoria, produto.data_entrada, produto.validade) == 9) {
        printf("| %-6d | %-18s | %-23s | %-8.2f | %-8.2f | %-15.2f | %-13s | %-10s | %-10s |\n",
               produto.codigo, produto.nome, produto.descricao, produto.preco, produto.preco_custo, produto.quantidade, produto.categoria, produto.data_entrada, produto.validade);
        printf("+--------+--------------------+-------------------------+----------+----------+-----------------+---------------+------------+------------+\n");
        count++;
    }

    if (count == 0) {
        printf("| Nenhum produto encontrado.                                                                                 |\n");
        printf("+-----------------------------------------------------------------------------------------------------------+\n");
    }

    fclose(file);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

//Menu de produtos
void menuProdutos() {
    int opcao;
    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE PRODUTOS             \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Cadastrar Produto                 |\n");
        printf("|      2. Entrada de Produtos               |\n");
        printf("|      3. Saida de Produtos                 |\n");
        printf("|      4. Editar Produto                    |\n");
        printf("|      5. Excluir Produto                   |\n");
        printf("|      6. Listar Produtos                   |\n");
        printf("|      0. Voltar ao Menu Principal          |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: cadastrarProduto(); break;//A função de dar entrada e cadastrar produtos é a mesma para criar um cadastro novo por motivo de validade
            case 3: saidaProduto(); break;
            case 4: editarProduto(); break;
            case 5: excluirProduto(); break;
            case 6: listarProdutos(); break;
            case 0: printf("Retornando ao menu principal...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}
