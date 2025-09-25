#ifndef EQUIPE_INTERNA_H
#define EQUIPE_INTERNA_H

#include "config_armazenamento/config_armazenamento.h"

// =============================
// Model de Equipe Interna
// =============================

// Estrutura básica de Equipe Interna
typedef struct {
    int id;
    char nome[50];
    int cpf;
    char funcao[100];
    float valor_diaria;
} EquipeInterna;

// Funções do modelo (CRUD)

// Recebe o tipo de armazenamento e a struct EquipeInterna a ser adicionada
EquipeInterna* adicionar_funcionario_eqIn(EquipeInterna* equipeInterna, TipoArmazenamento tipo);
// Atualiza os dados do funcionário existente, recebe a struct a ser atualizada e os novos dados
void atualizar_funcionario_eqIn(EquipeInterna* equipeInterna, const char* nome, int cpf, const char* funcao, float valor_diaria, TipoArmazenamento tipo);
void deletar_funcionario_eqIn(EquipeInterna* equipeInterna, TipoArmazenamento tipo);

#endif
