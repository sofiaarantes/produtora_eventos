#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe_interna.h"

// Cria e inicializa um fucionário da equipe interna
EquipeInterna* criar_equipe_interna(int id, const char* nome, int cpf, const char* funcao, float valor_diaria) {
    EquipeInterna* novo = (EquipeInterna*) malloc(sizeof(EquipeInterna));
    if (!novo) {
        return NULL; // erro de memória
    }
    novo->id = id;
    strncpy(novo->nome, nome, sizeof(novo->nome));
    novo->cpf = cpf;
    strncpy(novo->funcao, funcao, sizeof(novo->funcao));
    novo->valor_diaria = valor_diaria;
    return novo;
}

// Atualiza os dados do cliente
void atualizar_equipe_interna(EquipeInterna* equipeInterna, const char* nome, int cpf, const char* funcao, float valor_diaria) {
    if (!equipeInterna) return;
    strncpy(equipeInterna->nome, nome, sizeof(equipeInterna->nome));
    equipeInterna->cpf = cpf;
    strncpy(equipeInterna->funcao, funcao, sizeof(equipeInterna->funcao));
    equipeInterna->valor_diaria = valor_diaria;
}

// Remove o cliente (libera memória)
void deletar_equipe_interna(EquipeInterna* equipeInterna) {
    if (equipeInterna) free(equipeInterna);
}
