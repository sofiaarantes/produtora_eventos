#ifndef EQUIPE_INTERNA_VIEW_H
#define EQUIPE_INTERNA_VIEW_H

#include "../model/equipe_interna/equipe_interna.h"

// Mostra dados de uma equipe
void exibir_funcionario(const EquipeInterna* equipeInterna);

// Exibe o menu de Equipe e retorna a opção escolhida
int exibir_menu_equipe_interna();

// Lê os dados de um funcionario e RETORNA um funcionario preenchido
EquipeInterna ler_dados_funcionario();

// Pega dados atualizados de funcionario (mantemos por referência para comparar)
void ler_dados_atualizacao_funcionario(char* nome, int* cpf, char* funcao, float* valor_diaria);

#endif
