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
    int operador_id;
} EquipeInterna;

// Funções do modelo (CRUD)

// Cria um novo funcionário e retorna um ponteiro para ele, recebe o tipo armazenamento e a struct a ser adicionada
EquipeInterna* adicionar_funcionario(EquipeInterna* funcionario, TipoArmazenamento tipo);

// Recebe o cpf a ser atualizado, os novos dados e o tipo do armazenamento
EquipeInterna* atualizar_funcionario(const char* cpf_busca, EquipeInterna* novos_dados, TipoArmazenamento tipo);

// Deletar funcionário 
int deletar_funcionario(const char* cpf, TipoArmazenamento tipo);

// Retorna a quantidade de funcionários na memória
int get_qtd_funcionarios();

// Busca funcionário por CPF (retorna ponteiro para o funcionário ou NULL se não achar)
EquipeInterna* buscar_funcionario_por_cpf(const char* cpf_busca, TipoArmazenamento tipo);

//função que lista os funcionários de acordo com o tipo de armazenamento 
void listar_equipe_interna(TipoArmazenamento tipo);

//função que busca um membro da equipe pelo ID
EquipeInterna* buscar_equipe_por_id(TipoArmazenamento tipo, int id);

//função que busca um funcionário pelo ID
EquipeInterna* buscar_funcionario_por_id(TipoArmazenamento tipo, int id);
// Migrar dados

// Lista todos os registros do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
EquipeInterna* listar_todos_equipe_interna(TipoArmazenamento tipo, int* out_count);

// Remove todos os registros do armazenamento especificado. Retorna 1 em sucesso.
int limpar_equipe_interna(TipoArmazenamento tipo);

#endif
