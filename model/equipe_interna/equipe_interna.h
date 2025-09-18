#ifndef CLIENTE_H
#define CLIENTE_H

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
EquipeInterna* criar_equipe_interna(int id, const char* nome, int cpf, const char* funcao, float valor_diaria);
void atualizar_equipe_interna(EquipeInterna* equipeInterna, const char* nome, int cpf, const char* funcao, float valor_diaria);
void deletar_equipe_interna(EquipeInterna* equipeInterna);

#endif
