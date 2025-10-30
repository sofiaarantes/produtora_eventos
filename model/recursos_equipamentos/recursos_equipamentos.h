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

// Recebe o id do equipamento/recurso a ser atualizado, a struct a ser atualizada e os novos dados
RecursosEquipamentos* atualizar_equipamento(int id_busca, RecursosEquipamentos* novos_dados, TipoArmazenamento tipo);

// Deletar equipamento/recurso
int deletar_equipamento(int id_busca, TipoArmazenamento tipo);

// Retorna a quantidade de equipamento/recursos na memória
int get_qtd_equipamentos();

// Busca equipamento/recurso por id (retorna ponteiro para o equipamento/recurso ou NULL se não achar)
RecursosEquipamentos* buscar_equipamento_por_id(int id_busca, TipoArmazenamento tipo);

// Migrar dados

// Lista todos os registros do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
RecursosEquipamentos* listar_todos_equipamentos(TipoArmazenamento tipo, int* out_count);

// Remove todos os registros do armazenamento especificado. Retorna 1 em sucesso.
int limpar_equipamentos(TipoArmazenamento tipo);

#endif
