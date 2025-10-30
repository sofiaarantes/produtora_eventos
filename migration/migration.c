#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../model/config_armazenamento/config_armazenamento.h"

// Headers dos modelos (exemplo: equipe_interna). Os outros modelos devem expor as mesmas funções.
#include "../../model/equipe_interna/equipe_interna.h"
#include "../../model/cliente/cliente.h"
#include "../../model/fornecedor/fornecedor_parceiro.h"
#include "../../model/produtora/produtora.h"
#include "../../model/recursos_equipamentos/recursos_equipamentos.h"

// NOTE: cada model deve implementar as funções abaixo (prototipadas em seus headers):
// - TipoRegistro* listar_todos(TipoArmazenamento tipo, int* out_count);
// - int limpar_armazenamento(TipoArmazenamento tipo);
// As funções listar_todos retornam um array alocado dinamicamente que o migrador deve liberar depois.
// Aqui usamos os protótipos reais para cada model (veja equipe_interna como exemplo).

int migrar_todo_sistema(TipoArmazenamento origem, TipoArmazenamento destino) {
    if (origem == destino) {
        printf("Origem e destino são iguais. Nada a migrar.\n");
        return 1;
    }

    printf("Iniciando migração de armazenamento: %d -> %d\n", origem, destino);

    int ok = 1;

    // 1) Equipe Interna
    int count_e = 0;
    EquipeInterna* lista_e = listar_todos_equipe_interna(origem, &count_e);
    if (lista_e == NULL && count_e > 0) {
        printf("Erro ao listar Equipe Interna do tipo %d\n", origem);
        ok = 0;
    } else {
        // Inserir no destino
        for (int i = 0; i < count_e && ok; i++) {
            // criar um copia local para passar para adicionar (evita alterar o array original)
            EquipeInterna tmp = lista_e[i];
            if (adicionar_funcionario(&tmp, destino) == NULL) {
                printf("Falha ao migrar EquipeInterna cpf=%s\n", tmp.cpf);
                ok = 0;
            }
        }
    }

    // 2) Cliente
    int count_c = 0;
    Cliente* lista_c = listar_todos_clientes(origem, &count_c);
    if (lista_c == NULL && count_c > 0) {
        printf("Erro ao listar Clientes do tipo %d\n", origem);
        ok = 0;
    } else {
        for (int i = 0; i < count_c && ok; i++) {
            Cliente tmp = lista_c[i];
            if (criar_cliente(&tmp, destino) == NULL) {
                printf("Falha ao migrar Cliente id=%d\n", tmp.id);
                ok = 0;
            }
        }
    }

    // 3) Fornecedor/Parceiro
    int count_f = 0;
    Fornecedor_parceiro* lista_f = listar_todos_fornecedores(origem, &count_f);
    if (lista_f == NULL && count_f > 0) {
        printf("Erro ao listar Fornecedores do tipo %d\n", origem);
        ok = 0;
    } else {
        for (int i = 0; i < count_f && ok; i++) {
            Fornecedor_parceiro tmp = lista_f[i];
            if (criar_fornecedor_parceiro(&tmp, destino) == NULL) {
                printf("Falha ao migrar Fornecedor id=%d\n", tmp.id);
                ok = 0;
            }
        }
    }

    // 4) Produtora
    int count_p = 0;
    Produtora* lista_p = listar_todos_produtoras(origem, &count_p);
    if (lista_p == NULL && count_p > 0) {
        printf("Erro ao listar Produtoras do tipo %d\n", origem);
        ok = 0;
    } else {
        for (int i = 0; i < count_p && ok; i++) {
            Produtora tmp = lista_p[i];
            if (criar_produtora(&tmp, destino) == NULL) {
                printf("Falha ao migrar Produtora nome=%s\n", tmp.nome_fantasia);
                ok = 0;
            }
        }
    }

    // 5) RecursosEquipamentos
    int count_r = 0;
    RecursosEquipamentos* lista_r = listar_todos_equipamentos(origem, &count_r);
    if (lista_r == NULL && count_r > 0) {
        printf("Erro ao listar Recursos/Equipamentos do tipo %d\n", origem);
        ok = 0;
    } else {
        for (int i = 0; i < count_r && ok; i++) {
            RecursosEquipamentos tmp = lista_r[i];
            if (adicionar_equipamento(&tmp, destino) == NULL) {
                printf("Falha ao migrar Recurso descricao=%s\n", tmp.descricao);
                ok = 0;
            }
        }
    }

    // Se tudo ok, limpa os dados da origem. Se falhou, NÃO apaga para evitar perda de dados.
    if (ok) {
        printf("Migração dos dados concluída com sucesso. Removendo dados da origem...\n");
        if (!limpar_equipe_interna(origem)) {
            printf("Aviso: falha ao limpar Equipe Interna na origem %d\n", origem);
        }
        if (!limpar_clientes(origem)) {
            printf("Aviso: falha ao limpar Clientes na origem %d\n", origem);
        }
        if (!limpar_fornecedores(origem)) {
            printf("Aviso: falha ao limpar Fornecedores na origem %d\n", origem);
        }
        if (!limpar_produtoras(origem)) {
            printf("Aviso: falha ao limpar Produtoras na origem %d\n", origem);
        }
        if (!limpar_equipamentos(origem)) {
            printf("Aviso: falha ao limpar Recursos na origem %d\n", origem);
        }
        printf("Limpeza da origem finalizada.\n");
    } else {
        printf("Migração abortada devido a erros. Dados na origem foram preservados.\n");
    }

    // libera memoria alocada pelos listar_todos
    if (lista_e) free(lista_e);
    if (lista_c) free(lista_c);
    if (lista_f) free(lista_f);
    if (lista_p) free(lista_p);
    if (lista_r) free(lista_r);

    return ok;
}

