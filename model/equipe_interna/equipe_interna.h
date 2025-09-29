#ifndef EQUIPE_INTERNA_H
#define EQUIPE_INTERNA_H

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
EquipeInterna* adicionar_funcionario_eqIn(int id, const char* nome, int cpf, const char* funcao, float valor_diaria);
void atualizar_funcionario_eqIn(EquipeInterna* equipeInterna, const char* nome, int cpf, const char* funcao, float valor_diaria);
void deletar_funcionario_eqIn(EquipeInterna* equipeInterna);
#endif
