#ifndef RECURSOS_EQUIPAMENTOS_H
#define RECURSOS_EQUIPAMENTOS_H

#include "config_armazenamento/config_armazenamento.h"

// ================================
// Model de Recursos e Equipamentos
// ================================

// Estrutura básica de Equipe Interna
typedef struct {
    int id;
    char descricao[100];
    char categoria[50];
    int qtd_estoque;
    float preco_custo;
    float valor_diaria;
    int operador_id;
} RecursosEquipamentos;

// Funções do modelo (CRUD)

// Cria um novo equipamento/recurso e retorna um ponteiro para ele, recebe o tipo armazenamento e a struct a ser adicionada
RecursosEquipamentos* adicionar_equipamento(RecursosEquipamentos* equipamento, TipoArmazenamento tipo);

// Atualiza os dados do equipamento/recurso existente, recebe a struct a ser atualizada e os novos dados

// Atualizar equipamento/recurso em memória
void atualizar_equipamento_memoria(RecursosEquipamentos* equipamento_mem, const char* descricao, 
                  const char* categoria, int qtd_estoque, float preco_custo, float valor_diaria);

// Atualizar equipamento/recurso em texto
int atualizar_equipamento_texto(int id, const char* descricao, const char* categoria, 
                    int qtd_estoque, float preco_custo, float valor_diaria);

// Atualizar equipamento/recurso em binário
int atualizar_equipamento_binario(int id, const char* descricao, const char* categoria, 
                    int qtd_estoque, float preco_custo, float valor_diaria);

// Deletar equipamento/recurso em memória
int deletar_equipamento_memoria(int id);

// Deletar equipamento/recurso em arquivo texto
int deletar_equipamento_texto(int id);

// Deletar equipamento/recurso em arquivo binário
int deletar_equipamento_binario(int id);

// Retorna a quantidade de equipamento/recursos na memória
int get_qtd_equipamentos();

// Busca equipamento/recurso por id na memória (retorna ponteiro para o equipamento/recurso ou NULL se não achar)
RecursosEquipamentos* buscar_equipamento_por_id_memoria(int id_busca);

// Busca equipamento/recurso por id em arquivo texto (retorna ponteiro para o equipamento/recurso ou NULL se não achar)
RecursosEquipamentos* buscar_equipamento_por_id_texto(int id_busca);

// Busca equipamento/recurso por id em arquivo binário (retorna ponteiro para o equipamento/recurso ou NULL se não achar)
RecursosEquipamentos* buscar_equipamento_por_id_binario(int id_busca);

#endif
