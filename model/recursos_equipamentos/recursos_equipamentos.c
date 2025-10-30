#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursos_equipamentos.h"
#include "../sessao/sessao.h"

// --- Variáveis para armazenamento em memória ---
#define MAX_EQUIPAMENTOS 100
static RecursosEquipamentos equipamentos_memoria[MAX_EQUIPAMENTOS];
static int qtd = 0; // Contador de equipamentos na memória

// ========================
// Adicionar equipamento
// ========================
RecursosEquipamentos* adicionar_equipamento(RecursosEquipamentos* equipamento, TipoArmazenamento tipo) {
    if (!equipamento) return NULL; // Se o ponteiro recebido for nulo, quebra o programa

    // Define automaticamente o operador logado como responsável pelo cadastro
    equipamento->operador_id = get_operador_logado();

    int novo_id = 1; // Id começa em 1 por padrão

    switch (tipo) {
        case MEMORIA: {
            if (qtd < MAX_EQUIPAMENTOS) {
                // Se não houver dados na memória, ele continua com o novo_id = 1, caso contrário, resgatará o
                // último id e somará mais 1
                if (qtd > 0) 
                    novo_id = equipamentos_memoria[qtd - 1].id + 1;

                // Antes de salvar, verifica se o equipamento inserido já existe em memória
                for (int i = 0; i < qtd; i++) {
                    if (strcmp(equipamentos_memoria[i].descricao, equipamento->descricao) == 0 &&  
                            strcmp(equipamentos_memoria[i].categoria, equipamento->categoria) == 0) {
                        printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n", equipamento->descricao, equipamento->categoria);
                        return NULL;
                    }
                }

                equipamento->id = novo_id;
                // Copia os dados do funcionário passado para o array de memória na posição qtd que ele representa
                equipamentos_memoria[qtd] = *equipamento;

                // Ponteiro para o funcionário que acabou de ser salvo
                RecursosEquipamentos* salvo = &equipamentos_memoria[qtd];

                qtd++; // Incrementa o contador de funcionários na memória

                printf("Equipamento %s salvo em MEMÓRIA! ID: %d\n", equipamento->descricao, equipamento->id);
                return salvo; // Retorna o endereço do funcion salvo
            } else {
                printf("Erro: limite de equipamentos na memória atingido!\n");
                return NULL;
            }
        }

        case TEXTO: {
            // Descobre o último id no arquivo e verifica se o cpf recebido já não foi cadastrado
            FILE* fp = fopen("equipamentos.txt", "r");
            if (fp) {
                RecursosEquipamentos tmp;
                while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                              &tmp.id, tmp.descricao, tmp.categoria, &tmp.qtd_estoque, 
                              &tmp.preco_custo, &tmp.valor_diaria, &tmp.operador_id) == 7) {
                    // Se encontrar mesma descrição, categoria impede o cadastro
                    if (strcmp(tmp.descricao, equipamento->descricao) == 0 &&  
                            strcmp(tmp.categoria, equipamento->categoria) == 0) {
                        printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n", equipamento->descricao, equipamento->categoria);
                        fclose(fp);
                        return NULL;
                    }
                    novo_id = tmp.id + 1; // sempre fica com o último id lido +1
                }
                fclose(fp);
            }
            equipamento->id = novo_id;

            // Reabre o arquivo em modo append(Abre no final do arquivo) para não sobrescrever os 
            // equipamentos anteriores
            fp = fopen("equipamentos.txt", "a");
            if (!fp) {
                perror("Erro ao abrir equipamentos.txt");
                return NULL;
            }

            // Escreve os dados do equipamento no arquivo separados por ponto e vírgula
            fprintf(fp, "%d;%s;%s;%d;%f;%f;%d;\n",
                    equipamento->id, equipamento->descricao, equipamento->categoria,
                    equipamento->qtd_estoque, equipamento->preco_custo, equipamento->valor_diaria,
                    equipamento->operador_id);
            fclose(fp);

            printf("Equipamento %s salvo em TEXTO! ID: %d\n", equipamento->descricao, equipamento->id);
            return equipamento;// Retorna o equipamento que foi salvo
        }

        case BINARIO: {
            // Descobre último ID no arquivo binário
            FILE* fp = fopen("equipamentos.bin", "rb");
            if (fp) {
                RecursosEquipamentos tmp;
                while (fread(&tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) {
                    if (strcmp(tmp.descricao, equipamento->descricao) == 0 &&  
                            strcmp(tmp.categoria, equipamento->categoria) == 0) {
                        printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n", equipamento->descricao, equipamento->categoria);
                        fclose(fp);
                        return NULL;
                    }
                    novo_id = tmp.id + 1;
                }
                fclose(fp);
            }
            equipamento->id = novo_id;

            // Abre o arquivo em modo append(final do arquivo) binário
            fp = fopen("equipamentos.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir equipamentos.bin");
                return NULL;
            }

            // Grava a struct recebida no arquivo binário
            fwrite(equipamento, sizeof(RecursosEquipamentos), 1, fp);
            fclose(fp);

            printf("Equipamento %s salvo em BINÁRIO! ID: %d\n", equipamento->descricao, equipamento->id);
            return equipamento; // retorno o equipamento que foi salvo
        }

        default:
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
    }
}

// ============================
// Atualizar equipamento
// ============================
RecursosEquipamentos* atualizar_equipamento(int id_busca, RecursosEquipamentos* novos_dados, TipoArmazenamento tipo) {
    if (!id_busca || !novos_dados) return NULL;

    int operador_logado = get_operador_logado();

    switch (tipo) {
        // ==========================
        // MEMÓRIA
        // ==========================
        case MEMORIA: {
            // Verifica se já existe outro equipamento com mesma descrição e categoria
            for (int i = 0; i < qtd; i++) {
                if (equipamentos_memoria[i].id != id_busca &&
                    strcmp(equipamentos_memoria[i].descricao, novos_dados->descricao) == 0 &&
                    strcmp(equipamentos_memoria[i].categoria, novos_dados->categoria) == 0) {

                    printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n",
                           novos_dados->descricao, novos_dados->categoria);
                    return NULL;
                }
            }

            // Agora faz a atualização
            for (int i = 0; i < qtd; i++) {
                if (equipamentos_memoria[i].id == id_busca) {

                    int id_original = equipamentos_memoria[i].id;
                    int operador_original = equipamentos_memoria[i].operador_id;

                    equipamentos_memoria[i] = *novos_dados;
                    equipamentos_memoria[i].id = id_original;
                    equipamentos_memoria[i].operador_id = operador_original;

                    printf("Equipamento %s atualizado em MEMÓRIA!\n", equipamentos_memoria[i].descricao);
                    return &equipamentos_memoria[i];
                }
            }

            printf("Equipamento com ID %d não encontrado em MEMÓRIA!\n", id_busca);
            return NULL;
        }

        // ==========================
        // TEXTO
        // ==========================
        case TEXTO: {
            FILE* fp = fopen("equipamentos.txt", "r");
            FILE* temp = fopen("equipamentos_temp.txt", "w");
            if (!fp || !temp) {
                perror("Erro ao abrir arquivos de equipamentos");
                if (fp) fclose(fp);
                if (temp) fclose(temp);
                return NULL;
            }

            RecursosEquipamentos e;
            int atualizado = 0;
            int conflito = 0;

            // Primeira verificação: há conflito com outro equipamento?
            FILE* check = fopen("equipamentos.txt", "r");
            if (check) {
                RecursosEquipamentos tmp;
                while (fscanf(check, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                              &tmp.id, tmp.descricao, tmp.categoria, &tmp.qtd_estoque,
                              &tmp.preco_custo, &tmp.valor_diaria, &tmp.operador_id) == 7) {
                    if (tmp.id != id_busca &&
                        strcmp(tmp.descricao, novos_dados->descricao) == 0 &&
                        strcmp(tmp.categoria, novos_dados->categoria) == 0) {
                        conflito = 1;
                        break;
                    }
                }
                fclose(check);
            }

            if (conflito) {
                printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n",
                       novos_dados->descricao, novos_dados->categoria);
                fclose(fp);
                fclose(temp);
                remove("equipamentos_temp.txt");
                return NULL;
            }

            // Caso não haja conflito, atualiza normalmente
            while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                          &e.id, e.descricao, e.categoria, &e.qtd_estoque,
                          &e.preco_custo, &e.valor_diaria, &e.operador_id) == 7) {

                if (e.id == id_busca) {
                    int operador_original = e.operador_id;
                    e = *novos_dados;
                    e.id = id_busca;
                    e.operador_id = operador_original;
                    atualizado = 1;
                }

                fprintf(temp, "%d;%s;%s;%d;%f;%f;%d;\n",
                        e.id, e.descricao, e.categoria, e.qtd_estoque,
                        e.preco_custo, e.valor_diaria, e.operador_id);
            }

            fclose(fp);
            fclose(temp);
            remove("equipamentos.txt");
            rename("equipamentos_temp.txt", "equipamentos.txt");

            if (atualizado) {
                printf("Equipamento %s atualizado em TEXTO!\n", novos_dados->descricao);
                return novos_dados;
            } else {
                printf("Equipamento ID %d não encontrado ou sem permissão!\n", id_busca);
                return NULL;
            }
        }

        // ==========================
        // BINÁRIO
        // ==========================
        case BINARIO: {
            FILE* fp = fopen("equipamentos.bin", "r+b");
            if (!fp) {
                perror("Erro ao abrir equipamentos.bin");
                return NULL;
            }

            // Primeiro, verifica se já existe outro equipamento com os mesmos dados
            FILE* check = fopen("equipamentos.bin", "rb");
            if (check) {
                RecursosEquipamentos tmp;
                while (fread(&tmp, sizeof(RecursosEquipamentos), 1, check) == 1) {
                    if (tmp.id != id_busca && strcmp(tmp.descricao, novos_dados->descricao) == 0 &&
                        strcmp(tmp.categoria, novos_dados->categoria) == 0) {
                        printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n",
                               novos_dados->descricao, novos_dados->categoria);
                        fclose(check);
                        fclose(fp);
                        return NULL;
                    }
                }
                fclose(check);
            }

            RecursosEquipamentos e;
            int atualizado = 0;
            while (fread(&e, sizeof(RecursosEquipamentos), 1, fp) == 1) {
                if (e.id == id_busca) {
                    e = *novos_dados;
                    e.id = id_busca;
                    e.operador_id = operador_logado;

                    fseek(fp, -(long)sizeof(RecursosEquipamentos), SEEK_CUR);
                    fwrite(&e, sizeof(RecursosEquipamentos), 1, fp);
                    atualizado = 1;
                    break;
                }
            }

            fclose(fp);

            if (atualizado) {
                printf("Equipamento %s atualizado em BINÁRIO!\n", novos_dados->descricao);
                return novos_dados;
            } else {
                printf("Equipamento ID %d não encontrado ou sem permissão!\n", id_busca);
                return NULL;
            }
        }

        default:{
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
        }
    }
}

// ====================
// Deletar equipamento
// ====================
int deletar_equipamento(int id_busca, TipoArmazenamento tipo) {
    int removido = 0; // flag para indicar se encontrou e removeu o funcionário

    switch (tipo) {
        // ============================
        // MEMÓRIA
        // ============================
        case MEMORIA: {
            for (int i = 0; i < qtd; i++) {
                if (equipamentos_memoria[i].id == id_busca) {
                    // Move todos os equipamentos após ele para uma posição para trás
                    for (int j = i; j < qtd - 1; j++) {
                        equipamentos_memoria[j] = equipamentos_memoria[j + 1];
                    }
                    qtd--;
                    removido = 1;
                    break;
                }
            }
            break;
        }

        // ============================
        // TEXTO
        // ============================
        case TEXTO: {
            FILE* fp = fopen("equipamentos.txt", "r");
            FILE* temp = fopen("equipamentos_temp.txt", "w");
            if (!fp || !temp) {
                perror("Erro ao abrir arquivos de equipamentos");
                if (fp) fclose(fp);
                if (temp) fclose(temp);
                return 0;
            }

            RecursosEquipamentos e;
            while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                          &e.id, e.descricao, e.categoria, &e.qtd_estoque,
                          &e.preco_custo, &e.valor_diaria, &e.operador_id) == 7) {
                // Se não for o equipamento, grava no arquivo temporário
                if (e.id != id_busca) {
                    fprintf(temp, "%d;%s;%s;%d;%f;%f;%d;\n",
                            e.id, e.descricao, e.categoria, e.qtd_estoque,
                            e.preco_custo, e.valor_diaria, e.operador_id);
                } else {
                    removido = 1;
                }
            }

            fclose(fp);
            fclose(temp);

            remove("equipamentos.txt");
            rename("equipamentos_temp.txt", "equipamentos.txt");
            break;
        }

        // ============================
        // BINÁRIO
        // ============================
        case BINARIO: {
            FILE* fp = fopen("equipamentos.bin", "rb");
            FILE* temp = fopen("equipamentos_temp.bin", "wb");
            if (!fp || !temp) {
                perror("Erro ao abrir arquivos de equipamentos");
                if (fp) fclose(fp);
                if (temp) fclose(temp);
                return 0;
            }

            RecursosEquipamentos e;
            while (fread(&e, sizeof(RecursosEquipamentos), 1, fp) == 1) {
                // Copia todos os registros exceto o que será removido
                if (e.id != id_busca) {
                    fwrite(&e, sizeof(RecursosEquipamentos), 1, temp);
                } else {
                    removido = 1;
                }
            }

            fclose(fp);
            fclose(temp);
            remove("equipamentos.bin");
            rename("equipamentos_temp.bin", "equipamentos.bin");
            break;
        }

        // ============================
        // Caso inválido
        // ============================
        default:
            printf("Tipo de armazenamento inválido!\n");
            break;
    }

    return removido;
}

// ===============================================
// Retorna a quantidade de equipamentos em memória
// ===============================================
int get_qtd_equipamentos() {
    return qtd;
}

// =========================
// Buscar equipamento por ID
// =========================
RecursosEquipamentos* buscar_equipamento_por_id(int id_busca, TipoArmazenamento tipo) {
    // Ponteiro estático para manter os dados válidos fora da função
    static RecursosEquipamentos equipamento_tmp;

    switch (tipo) {
        // ======================
        // Memória
        // ======================
        case MEMORIA: {
            for (int i = 0; i < qtd; i++) {
                if (equipamentos_memoria[i].id == id_busca) {
                    return &equipamentos_memoria[i];
                }
            }
            break;
        }
        // ======================
        // Texto
        // ======================
        case TEXTO: {
            FILE* fp = fopen("equipamentos.txt", "r");
            if (!fp) {
                perror("Erro ao abrir equipamentos.txt");
                return NULL;
            }

            while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                          &equipamento_tmp.id, equipamento_tmp.descricao,
                          equipamento_tmp.categoria, &equipamento_tmp.qtd_estoque,
                          &equipamento_tmp.preco_custo, &equipamento_tmp.valor_diaria,
                          &equipamento_tmp.operador_id) == 7) {
                if (equipamento_tmp.id == id_busca) {
                    fclose(fp);
                    return &equipamento_tmp;
                }
            }

            fclose(fp);
            break;
        }

        // ======================
        // Binário
        // ======================
        case BINARIO: {
            FILE* fp = fopen("equipamentos.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir equipamentos.bin");
                return NULL;
            }

            while (fread(&equipamento_tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) {
                if (equipamento_tmp.id == id_busca) {
                    fclose(fp);
                    return &equipamento_tmp;
                }
            }

            fclose(fp);
            break;
        }

        default:
            printf("Tipo de armazenamento inválido!\n");
            break;
    }

    return NULL;
}

// Lista todos os equipamentos do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
RecursosEquipamentos* listar_todos_equipamentos(TipoArmazenamento tipo, int* out_count) {
    if (!out_count) return NULL;
    *out_count = 0;

    switch (tipo) {
        case MEMORIA: {
            if (qtd == 0) return NULL;
            RecursosEquipamentos* arr = malloc(sizeof(RecursosEquipamentos) * qtd);
            if (!arr) return NULL;
            for (int i = 0; i < qtd; i++) arr[i] = equipamentos_memoria[i];
            *out_count = qtd;
            return arr;
        }
        case TEXTO: {
            FILE* fp = fopen("equipamentos.txt", "r");
            if (!fp) return NULL;
            int count = 0;
            char linha[512];
            while (fgets(linha, sizeof(linha), fp)) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            RecursosEquipamentos* arr = malloc(sizeof(RecursosEquipamentos) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fgets(linha, sizeof(linha), fp) && idx < count) {
                RecursosEquipamentos e = {0};
                sscanf(linha, "%d;%99[^;];%49[^;];%d;%f;%f;%d;",
                       &e.id, e.descricao, e.categoria, &e.qtd_estoque,
                       &e.preco_custo, &e.valor_diaria, &e.operador_id);
                arr[idx++] = e;
            }
            fclose(fp);
            *out_count = idx;
            return arr;
        }
        case BINARIO: {
            FILE* fp = fopen("equipamentos.bin", "rb");
            if (!fp) return NULL;
            int count = 0;
            RecursosEquipamentos tmp;
            while (fread(&tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            RecursosEquipamentos* arr = malloc(sizeof(RecursosEquipamentos) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fread(&arr[idx], sizeof(RecursosEquipamentos), 1, fp) == 1) {
                idx++;
            }
            fclose(fp);
            *out_count = idx;
            return arr;
        }
        default:
            return NULL;
    }
}

// Remove todos os equipamentos do armazenamento especificado. Retorna 1 em sucesso, 0 caso contrário.
int limpar_equipamentos(TipoArmazenamento tipo) {
    switch (tipo) {
        case MEMORIA:
            qtd = 0;
            // opcional: memset(equipamentos_memoria, 0, sizeof(equipamentos_memoria));
            return 1;
        case TEXTO:
            remove("equipamentos.txt");
            return 1;
        case BINARIO:
            remove("equipamentos.bin");
            return 1;
        default:
            return 0;
    }
}
