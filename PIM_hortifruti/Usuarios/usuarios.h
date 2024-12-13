#ifndef USUARIOS_H
#define USUARIOS_H


typedef struct {
    char username[50];
    char password[50];
    int nivel_acesso;
    char cpf_funcionario[12];
} Usuario;

void cadastrarUsuario();
void editarUsuario();
void excluirUsuario();
void listarUsuarios();
void menuUsuarios();

#endif
