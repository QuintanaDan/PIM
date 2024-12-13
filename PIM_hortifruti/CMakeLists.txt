cmake_minimum_required(VERSION 3.29)
project(PIM_hortifruti C)

set(CMAKE_C_STANDARD 11)

add_executable(PIM_hortifruti main.c
        Clientes/clientes.h
        Clientes/clientes.c
        Produtos/produtos.h
        Produtos/produtos.c
        Fornecedores/fornecedores.h
        Fornecedores/fornecedores.c
        Usuarios/usuarios.h
        Usuarios/usuarios.c
        Funcionarios/funcionarios.h
        Funcionarios/funcionarios.c
        Compras/compras.h
        Compras/compras.c
        Vendas/vendas.h
        Vendas/vendas.c
        Relatorios/relatorios.h
        Relatorios/relatorios.c
        Orçamentos/orcamentos.h
        Orçamentos/orcamentos.c
        Pesagem/pesagem.h
        Pesagem/pesagem.c
        Uteis/uteis.h
        Uteis/uteis.c
        Menus/menus.h
        Menus/menus.c
        Login/login.h
        Login/login.c
        Avisos/avisos.h
        Avisos/avisos.c)
