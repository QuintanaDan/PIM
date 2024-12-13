#ifndef LOGIN_H
#define LOGIN_H

int login(char *usuario, char *senha);
void ocultarSenha(char *senha, int tamanho);
int verificarFuncionarioExistente(char *cpf);

#endif //LOGIN_H
