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
    char cpf[20];
    char funcao[100];
    float valor_diaria;
} EquipeInterna;

// Funções do modelo (CRUD)

// Cria um novo funcionário e retorna um ponteiro para ele, recebe o tipo armazenamento e a struct a ser adicionada
EquipeInterna* adicionar_funcionario_eqIn(EquipeInterna* funcionario, TipoArmazenamento tipo);

// Atualiza os dados do funcionário existente, recebe a struct a ser atualizada e os novos dados

// Atualizar funcionário em memória
void atualizar_funcionario_memoria(EquipeInterna* funcionario_mem, const char* nome, const char* funcao, float valor_diaria);

// Atualizar funcionário em binário
int atualizar_funcionario_binario(EquipeInterna* funcionario_bin, const char* nome, const char* funcao, float valor_diaria);

// Atualizar funcionário em texto
int atualizar_funcionario_texto(EquipeInterna* funcionario_txt, const char* nome, const char* funcao, float valor_diaria);

// Remove funcionário da memória
void deletar_funcionario(EquipeInterna* funcionario);

// Retorna a quantidade de funcionários na memória
int get_qtd_funcionarios();

// Busca funcionário por CPF na memória (retorna ponteiro para o funcionário ou NULL se não achar)
EquipeInterna* buscar_funcionario_por_cpf_memoria(const char* cpf_busca);

// Busca funcionário por CPF em arquivo texto (retorna ponteiro para o funcionário ou NULL se não achar)
EquipeInterna* buscar_funcionario_por_cpf_texto(const char* cpf_busca);

// Busca funcionário por CPF em arquivo binário (retorna ponteiro para o funcionário ou NULL se não achar)
EquipeInterna* buscar_funcionario_por_cpf_binario(const char* cpf_busca);

#endif
