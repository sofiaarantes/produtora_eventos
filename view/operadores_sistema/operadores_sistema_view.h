#ifndef OPERADORES_SISTEMA_VIEW_H
#define OPERADORES_SISTEMA_VIEW_H

#include "../model/operadores_sistema/operadores_sistema.h"

// Exibe o menu de operadores e retorna a opção escolhida
int exibir_menu_operadores();

// Exibe o menu de login e retorna a opção escolhida
Operadores ler_dados_operador_login();

// Exibe o menu de registro e retorna a opção escolhida
Operadores ler_dados_operador_cadastro();

#endif