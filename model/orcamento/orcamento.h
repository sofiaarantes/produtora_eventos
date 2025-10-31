#ifndef ORCAMENTO_H
#define ORCAMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/config_armazenamento/config_armazenamento.h"

// ---------------------------------------------------------------
// ENUM: Status do orçamento
// ---------------------------------------------------------------
// Define o estado atual do orçamento/evento, controlando seu ciclo:
//  - RASCUNHO: orçamento ainda sendo montado
//  - APROVADO: orçamento confirmado, recursos reservados
//  - EM_EXECUCAO: evento em andamento
//  - FINALIZADO: evento concluído, recursos liberados
typedef enum {
    RASCUNHO,
    APROVADO,
    EM_EXECUCAO,
    FINALIZADO
} StatusOrcamento;

// ---------------------------------------------------------------
// ENUM: Tipo de item do orçamento
// ---------------------------------------------------------------
// Define o tipo do item dentro do orçamento:
//  - ITEM_RECURSO: equipamentos e materiais da produtora
//  - ITEM_SERVICO: fornecedores/parceiros (ex: buffet, segurança)
//  - ITEM_EQUIPE: funcionários/colaboradores da produtora
typedef enum {
    ITEM_RECURSO,
    ITEM_SERVICO,
    ITEM_EQUIPE
} TipoItem;

// ---------------------------------------------------------------
// STRUCT: Item do orçamento
// ---------------------------------------------------------------
// Cada item representa um recurso, serviço ou membro da equipe
// adicionado ao orçamento.
typedef struct {
    int id;                     // identificador do item (único dentro do orçamento)
    TipoItem tipo;              // tipo do item (recurso, serviço ou equipe)
    int idReferencia;           // id do recurso/fornecedor/funcionário
    char descricao[100];        // descrição do item (ex: "Caixa de som 500W")
    int quantidade;             // quantidade de unidades
    float precoUnitario;        // preço por unidade (ou diária)
    float subtotal;             // quantidade * precoUnitario
    int status_item;            //status do item (se reservado ou não)
} ItemOrcamento;

// ---------------------------------------------------------------
// STRUCT: Orçamento principal
// ---------------------------------------------------------------
// Guarda os dados do evento e a lista de itens (recursos e serviços)
typedef struct {
    int id;                      // identificador do orçamento
    int idCliente;               // id do cliente (do seu cadastro de clientes)
    char local[100];             // local do evento
    char dataInicio[20];         // data inicial (formato "DD/MM/YYYY")
    char dataFim[20];            // data final (formato "DD/MM/YYYY")
    char horaInicio[6];          // hora inicial (formato "HH:MM")
    char horaFim[6];             // hora final (formato "HH:MM")
    StatusOrcamento status;      // estado atual do orçamento/evento
    ItemOrcamento *itens;        // vetor dinâmico de itens
    int numItens;                // número de itens adicionados
    int capacidadeItens;         // capacidade atual alocada (para realloc)
    float valorTotalEquipamentos;// total dos preços dositens de tipo RECURSO
    float valorTotalServicos;    // total dos preços dos itens de tipo SERVICO
    float valorTotalEqInterna;  //total de preço da eq interna
    float valorTotalOrcamento;   // soma dos valores totais
} Orcamento;

// ---------------------------------------------------------------
// STRUCT: Reserva de recursos
// ---------------------------------------------------------------
// Criada quando o orçamento é aprovado, para marcar equipamentos
// como "reservados" e evitar conflitos de data.
typedef struct {
    int id;                      // identificador da reserva
    int idRecurso;               // id do recurso (equipamento)
    int idOrcamento;             // id do orçamento associado
    char dataInicio[20];         // data de início da reserva
    char dataFim[20];            // data de fim da reserva
    int quantidade;              // quantidade de unidades reservadas
    int id_logado;  //id do operador a quem pertence o orçamento (logado)
} ReservaRecurso;

#endif // ORCAMENTO_H
