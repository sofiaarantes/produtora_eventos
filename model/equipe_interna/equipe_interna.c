#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe_interna.h"
#include "model/config_armazenamento/config_armazenamento.h"

// Cria e inicializa um funcionário da equipe interna
EquipeInterna* adicionar_funcionario(int id, const char* nome, int cpf, const char* funcao, float valor_diaria) {
    TipoArmazenamento tipo = get_armazenamento();
    switch(tipo) {
        case MEMORIA:
            memoria[qtd++] = f;
            printf("Funcionario salvo em MEMORIA.\n");
            break;
        
        case TEXTO:
            FILE* fp = fopen("equipe.txt", "a");
            if (!fp) return;
            fprintf(fp, "%d;%s;%d;%s;%.2f\n", f->id, f->nome, f->cpf, f->funcao, f->valor_diaria);
            fclose(fp);
            printf("Funcionario salvo em TEXTO.\n");
            break;
        
        case BINARIO:
            FILE* fp = fopen("equipe.bin", "ab"); // append binário
            if (!fp) return;
            fwrite(f, sizeof(EquipeInterna), 1, fp);
            fclose(fp);
            printf("Funcionario salvo em BINARIO.\n");
            break;
    }
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

// Atualiza os dados do funcionário
void atualizar_funcionario_eqIn(EquipeInterna* equipeInterna, const char* nome, int cpf, const char* funcao, float valor_diaria) {
    if (!equipeInterna) return;
    strncpy(equipeInterna->nome, nome, sizeof(equipeInterna->nome));
    equipeInterna->cpf = cpf;
    strncpy(equipeInterna->funcao, funcao, sizeof(equipeInterna->funcao));
    equipeInterna->valor_diaria = valor_diaria;
}

// Remove o funcionário (libera memória)
void deletar_funcionario_eqIn(EquipeInterna* equipeInterna) {
    if (equipeInterna) free(equipeInterna);
}
