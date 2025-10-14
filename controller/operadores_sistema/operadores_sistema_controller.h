#ifndef OPERADORES_SISTEMA_CONTROLLER_H
#define OPERADORES_SISTEMA_CONTROLLER_H

// =============================
// CONTROLLER: conecta MODEL e VIEW
// =============================

// Criptografa a senha utilizando a cifra de César
void criptografar_senha(char* senha);

// Gerencia o login e cadastro de operadores
void gerenciar_login();

// Permite ao operador logado editar seus dados ou deletar sua conta
void editar_operador();

#endif
