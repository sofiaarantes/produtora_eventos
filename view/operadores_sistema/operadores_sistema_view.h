#ifndef OPERADORES_SISTEMA_VIEW_H
#define OPERADORES_SISTEMA_VIEW_H

#include "../model/operadores_sistema/operadores_sistema.h"

// Exibe o menu de operadores e retorna a opção escolhida
int exibir_menu_operadores();

// Lê os dados de login do operador e retorna a struct preenchida
Operadores ler_dados_operador_login();

// Lê os dados de cadastro do operador e retorna a struct preenchida
Operadores ler_dados_operador_cadastro();

// Exibe as informações de um operador
void exibir_operador(const Operadores* operador);

// Exibe o menu de edição do operador logado e retorna a opção escolhida
int exibir_menu_operadores_editar();

// Lê os novos dados do operador (exceto senha) para atualização
void ler_dados_atualizados_operador(char* nome, char* usuario);

#endif
