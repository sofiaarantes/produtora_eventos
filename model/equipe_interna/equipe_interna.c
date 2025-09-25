#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe_interna.h"
#include "config_armazenamento/config_armazenamento.h"

// --- Variáveis globais para armazenamento em memória ---
#define MAX_FUNCIONARIOS 100
static EquipeInterna* memoria[MAX_FUNCIONARIOS];
static int qtd = 0;

// ================================================
// Adicionar funcionário
// ================================================
EquipeInterna* adicionar_funcionario_eqIn(EquipeInterna* f, TipoArmazenamento tipo) {
    if (!f) return NULL;

    switch (tipo) {
        case MEMORIA:
            if (qtd < MAX_FUNCIONARIOS) {
                memoria[qtd++] = f;
                printf("Funcionario salvo em MEMORIA.\n");
            } else {
                printf("Erro: limite de funcionarios na memoria atingido!\n");
                return NULL;
            }
            break;

        case TEXTO: {
            FILE* fp = fopen("equipe.txt", "a");
            if (!fp) {
                printf("Erro ao abrir arquivo texto!\n");
                return NULL;
            }
            fprintf(fp, "%d;%s;%d;%s;%.2f\n", 
                    f->id, f->nome, f->cpf, f->funcao, f->valor_diaria);
            fclose(fp);
            printf("Funcionario salvo em TEXTO.\n");
            break;
        }

        case BINARIO: {
            FILE* fp = fopen("equipe.bin", "ab");
            if (!fp) {
                printf("Erro ao abrir arquivo binario!\n");
                return NULL;
            }
            fwrite(f, sizeof(EquipeInterna), 1, fp);
            fclose(fp);
            printf("Funcionario salvo em BINARIO.\n");
            break;
        }

        default:
            printf("Tipo de armazenamento invalido!\n");
            return NULL;
    }

    return f;
}

// ================================================
// Atualizar funcionário
// ================================================
void atualizar_funcionario_eqIn(EquipeInterna* f, const char* nome, int cpf, const char* funcao, float valor_diaria, TipoArmazenamento tipo) {
    if (!f) return;

    switch (tipo) {
        case MEMORIA:
            strncpy(f->nome, nome, sizeof(f->nome));
            f->cpf = cpf;
            strncpy(f->funcao, funcao, sizeof(f->funcao));
            f->valor_diaria = valor_diaria;
            printf("Funcionario atualizado em MEMORIA.\n");
            break;

        case TEXTO:
            printf("Atualizar em TEXTO não é simples! 🤔\n");
            printf("Sugestão: recrie o arquivo sem o antigo e grave os novos dados.\n");
            break;

        case BINARIO:
            printf("Atualizar em BINARIO não é simples! 🤔\n");
            printf("Sugestão: recrie o arquivo sem o antigo e grave os novos dados.\n");
            break;

        default:
            printf("Tipo de armazenamento invalido!\n");
    }
}

// ================================================
// Deletar funcionário
// ================================================
void deletar_funcionario_eqIn(EquipeInterna* f, TipoArmazenamento tipo) {
    if (!f) return;

    switch (tipo) {
        case MEMORIA: {
            int i;
            for (i = 0; i < qtd; i++) {
                if (memoria[i] == f) {
                    free(memoria[i]); // libera memória
                    for (int j = i; j < qtd - 1; j++) {
                        memoria[j] = memoria[j+1]; // compacta lista
                    }
                    qtd--;
                    printf("Funcionario removido da MEMORIA.\n");
                    return;
                }
            }
            printf("Funcionario nao encontrado na MEMORIA.\n");
            break;
        }

        case TEXTO:
            printf("Deletar em TEXTO exige recriar o arquivo sem o funcionario.\n");
            break;

        case BINARIO:
            printf("Deletar em BINARIO exige recriar o arquivo sem o funcionario.\n");
            break;

        default:
            printf("Tipo de armazenamento invalido!\n");
    }
}
