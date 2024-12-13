#include "menus.h"

#include <stdio.h>
#include <string.h>
#include "../Clientes/clientes.h"
#include "../Fornecedores/fornecedores.h"
#include "../Produtos/produtos.h"
#include "../Usuarios/usuarios.h"
#include "../Funcionarios/funcionarios.h"
#include "../Compras/compras.h"
#include "../Vendas/vendas.h"
#include "../Orçamentos/orcamentos.h"
#include "../Relatorios/relatorios.h"
#include "../Pesagem/pesagem.h"
#include "../Uteis/uteis.h"
#include <windows.h>
#include "../Avisos/avisos.h"

//Menu do Administrativo (Escritório)
void menuEscritorio() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Defina cor para o título em verde brilhante
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("     BEM VINDO(A) AO MENU ADMINISTRATIVO     \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|        ---- Sistema Hortifruti ----       |\n");
        printf("|      1. Avisos                            |\n");
        printf("|      2. Balanca                           |\n");
        printf("|      3. Clientes                          |\n");
        printf("|      4. Compras                           |\n");
        printf("|      5. Fornecedores                      |\n");
        printf("|      6. Funcionarios                      |\n");
        printf("|      7. Orcamentos                        |\n");
        printf("|      8. Produtos                          |\n");
        printf("|      9. Relatorios                        |\n");
        printf("|      10. Usuarios                         |\n");
        printf("|      11. Vendas                           |\n");
        printf("|      0.  Sair                             |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1: menuAvisos(); break;
            case 2: menuPesagem(); break;
            case 3: menuClientes(); break;
            case 4: menuCompras(); break;
            case 5: menuFornecedores(); break;
            case 6: menuFuncionarios(); break;
            case 7: menuOrcamentos(); break;
            case 8: menuProdutos(); break;
            case 9: menuRelatorios(); break;
            case 10: menuUsuarios(); break;
            case 11: menuVendas(); break;
            case 0: printf("Saindo do sistema...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}

//Menu do ponto de venda
void menuPontoDeVenda() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Defina cor para o título em verde brilhante
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("             MENU PONTO DE VENDA             \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Realizar Venda                    |\n");
        printf("|      2. Visualizar Relatorio de Vendas    |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do stdin

        switch (opcao) {
            case 1: registrarVendaCliente(); break;
            case 2: gerarRelatorioVendas(); break;
            case 0: printf("Saindo do menu de Ponto de Venda.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}

//Menu do Estoque
void menuEstoque() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Defina cor para o título em verde brilhante
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("               MENU DO ESTOQUE               \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Cadastrar Produto                 |\n");
        printf("|      2. Editar Produto                    |\n");
        printf("|      3. Excluir Produto                   |\n");
        printf("|      4. Listar Produtos                   |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do stdin

        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: editarProduto(); break;
            case 3: excluirProduto(); break;
            case 4: listarProdutos(); break;
            case 0: printf("Saindo do menu de Estoque.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}

//Menu de Pesagem (Balança)
void menuPesagem() {
    int opcao;

    do {
        limpa_tela();
        nomehort();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        printf("\n\n");
        printf("=============================================\n");
        printf("                MENU DE PESAGEM              \n");
        printf("=============================================\n");

        // Restaura a cor padrão para o menu
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("+-------------------------------------------+\n");
        printf("|      1. Registrar Pesagem                 |\n");
        printf("|      2. Visualizar Historico de Pesagens  |\n");
        printf("|      0. Sair                              |\n");
        printf("+-------------------------------------------+\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do stdin

        switch (opcao) {
            case 1: registrarPesagem(); break;
            case 2: listarPesagens(); break;
            case 0: printf("Saindo do menu de Pesagem.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}

//Menu de nivel de acesso
void menu_principal(int nivel_acesso) {
    switch (nivel_acesso) {
        case 1: menuEscritorio(); break;
        case 2: menuPontoDeVenda(); break;
        case 3: menuEstoque(); break;
        case 4: menuPesagem(); break;
        default: printf("Acesso invalido!\n");
    }
}
