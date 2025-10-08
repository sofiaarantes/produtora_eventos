#ifndef RECURSOS_EQUIPAMENTOS_VIEW_H
#define RECURSOS_EQUIPAMENTOS_VIEW_H

#include "../model/recursos_equipamentos/recursos_equipamentos.h"

// Mostra dados de uma equipe
void exibir_equipamento(const RecursosEquipamentos* equipamentos);

// Exibe o menu de Equipe e retorna a opção escolhida
int exibir_menu_equipamentos();

// Lê os dados de um equipamento e RETORNA um equipamento preenchido
RecursosEquipamentos ler_dados_equipamento();

// Pega dados atualizados de equipamento (mantemos por referência para comparar)
void ler_dados_atualizados_equipamento(char* descricao, char* categoria, int* qtd_estoque, float* preco_custo, float* valor_diaria);

#endif
