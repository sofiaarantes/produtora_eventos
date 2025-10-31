#ifndef ORCAMENTO_VIEW_H
#define ORCAMENTO_VIEW_H

#include "../model/orcamento/orcamento.h"
#include "../model/config_armazenamento/config_armazenamento.h"

//funçao que exibe o menu de orcamento e retorna a opçao escolhida
int menu_orcamento();

//funçao que lê os dados do orcamento 
Orcamento ler_dados_orcamento(TipoArmazenamento tipo);
#endif