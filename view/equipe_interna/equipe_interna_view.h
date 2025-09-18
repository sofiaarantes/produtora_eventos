#ifndef EQUIPE_INTERNA_VIEW_H
#define EQUIPE_INTERNA_VIEW_H

#include "../model/equipe_interna/equipe_interna.h"

// Mostra dados de uma equipe
void exibir_equipe_interna(const EquipeInterna* equipeInterna);

// Exibe o menu de Equipe e retorna a opção escolhida
int exibir_menu_equipe_interna();

// Lê os dados de um cliente e RETORNA um Cliente preenchido
EquipeInterna ler_dados_cliente();

// Pega dados atualizados de cliente (mantemos por referência para comparar)
void ler_dados_atualizacao(char* nome, int* idade);

// Exibe mensagem genérica
void exibir_mensagem(const char* msg);

#endif
