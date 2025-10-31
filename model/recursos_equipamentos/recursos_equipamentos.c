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
                            strcmp(equipamentos_memoria[i].categoria, equipamento->categoria) == 0 && 
                                equipamentos_memoria[i].operador_id == equipamento->operador_id) {
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

                printf("Equipamento %s salvo em MEMÓRIA!\n", equipamento->descricao);
                return salvo; // Retorna o endereço do funcion salvo
            } else {
                printf("Erro: limite de equipamentos na memória atingido!\n");
                return NULL;
            }
        }

        case TEXTO: {
            // Descobre o último id no arquivo e verifica se o cpf recebido já não foi cadastrado
            // por esse operador
            FILE* fp = fopen("equipamentos.txt", "r");
            if (fp) {
                RecursosEquipamentos tmp;
                while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                              &tmp.id, tmp.descricao, tmp.categoria, &tmp.qtd_estoque, 
                              &tmp.preco_custo, &tmp.valor_diaria, &tmp.operador_id) == 7) {
                    // Se encontrar mesma descrição, categoria e mesmo operador_id impede o cadastro
                    if (strcmp(tmp.descricao, equipamento->descricao) == 0 &&  
                            strcmp(tmp.categoria, equipamento->categoria) == 0 && 
                            tmp.operador_id == equipamento->operador_id) {
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

            printf("Equipamento %s salvo em TEXTO!\n", equipamento->descricao);
            return equipamento;// Retorna o equipamento que foi salvo
        }

        case BINARIO: {
            // Descobre último ID no arquivo binário
            FILE* fp = fopen("equipamentos.bin", "rb");
            if (fp) {
                RecursosEquipamentos tmp;
                while (fread(&tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) {
                    if (strcmp(tmp.descricao, equipamento->descricao) == 0 &&  
                            strcmp(tmp.categoria, equipamento->categoria) == 0 && 
                                tmp.operador_id == equipamento->operador_id) {
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

            printf("Equipamento %s salvo em BINÁRIO! \n", equipamento->descricao);
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
                if (equipamentos_memoria[i].operador_id == operador_logado &&
                    equipamentos_memoria[i].id != id_busca &&
                    strcmp(equipamentos_memoria[i].descricao, novos_dados->descricao) == 0 &&
                    strcmp(equipamentos_memoria[i].categoria, novos_dados->categoria) == 0) {

                    printf("\nErro: Já existe um equipamento '%s' cadastrado na categoria %s.\n",
                           novos_dados->descricao, novos_dados->categoria);
                    return NULL;
                }
            }

            // Agora faz a atualização
            for (int i = 0; i < qtd; i++) {
                if (equipamentos_memoria[i].id == id_busca &&
                    equipamentos_memoria[i].operador_id == operador_logado) {

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
                    if (tmp.operador_id == operador_logado &&
                        tmp.id != id_busca &&
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

                if (e.id == id_busca && e.operador_id == operador_logado) {
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
                    if (tmp.operador_id == operador_logado &&
                        tmp.id != id_busca &&
                        strcmp(tmp.descricao, novos_dados->descricao) == 0 &&
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
                if (e.id == id_busca && e.operador_id == operador_logado) {
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
    int operador_logado = get_operador_logado();
    int removido = 0; // flag para indicar se encontrou e removeu o funcionário

    switch (tipo) {
        // ============================
        // MEMÓRIA
        // ============================
        case MEMORIA: {
            for (int i = 0; i < qtd; i++) {
                // Verifica ID e se pertence ao operador logado
                if (equipamentos_memoria[i].id == id_busca &&
                    equipamentos_memoria[i].operador_id == operador_logado) {
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
                // Se não for o equipamento do operador logado, grava no arquivo temporário
                if (e.id != id_busca && e.operador_id == operador_logado) {
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
                if (e.id != id_busca && e.operador_id != operador_logado) {
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

    int operador_logado = get_operador_logado();

    switch (tipo) {
        // ======================
        // Memória
        // ======================
        case MEMORIA: {
            for (int i = 0; i < qtd; i++) {
                if (equipamentos_memoria[i].id == id_busca &&
                    equipamentos_memoria[i].operador_id == operador_logado) {
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
                if (equipamento_tmp.id == id_busca &&
                    equipamento_tmp.operador_id == operador_logado) {
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
                if (equipamento_tmp.id == id_busca &&
                    equipamento_tmp.operador_id == operador_logado) {
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

void listar_equipamentos(TipoArmazenamento tipo) {
-
    printf("\n");
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                     LISTA DE EQUIPAMENTOS                       |\n");
    printf("+ --------------------------------------------------------------- +\n");

    // ------------------------------------------------------------------
    // Aqui eu trato de acordo com o tipo de armazenamento que o usuário
    // escolheu (MEMORIA, TEXTO ou BINARIO)
    // ------------------------------------------------------------------
    switch (tipo) {

        // ===============================================================
        // MODO MEMÓRIA
        // Nesse caso, os dados estão em um vetor global (equipamentos_memoria)
        // ===============================================================
        case MEMORIA: {
            // Se não tiver nenhum equipamento cadastrado, eu aviso
            if (qtd == 0) {
                printf("|  Nenhum equipamento cadastrado.\n");
                break;
            }

            // Caso tenha, eu percorro o vetor e mostro todos os registros
            for (int i = 0; i < qtd; i++) {
                printf("|  ID: %d | Descrição: %s | Categoria: %s | Estoque: %d | Diária: %.2f\n",
                       equipamentos_memoria[i].id,           // id do equipamento
                       equipamentos_memoria[i].descricao,     // nome ou descrição
                       equipamentos_memoria[i].categoria,     // categoria (ex: som, iluminação)
                       equipamentos_memoria[i].qtd_estoque,   // quantidade disponível
                       equipamentos_memoria[i].valor_diaria); // valor da diária
            }
            break;
        }

        // ===============================================================
        // MODO TEXTO
        // Aqui eu leio o arquivo "equipamentos.txt"
        // Cada linha do arquivo representa um equipamento
        // ===============================================================
        case TEXTO: {
            FILE* fp = fopen("equipamentos.txt", "r"); // abre o arquivo no modo leitura
            if (!fp) {
                // Se o arquivo não existir, eu aviso
                printf("|  Arquivo 'equipamentos.txt' não encontrado.\n");
                return;
            }

            RecursosEquipamentos tmp;  // struct temporária para armazenar o que eu leio do arquivo
            int encontrados = 0;       // contador para saber se achei algo

            // Leio linha por linha do arquivo, respeitando o formato usado no salvamento
            while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;%d;\n",
                          &tmp.id, tmp.descricao, tmp.categoria,
                          &tmp.qtd_estoque, &tmp.preco_custo,
                          &tmp.valor_diaria, &tmp.operador_id) == 7) {

                // Aqui eu exibo os dados formatadinhos na tela
                printf("|  ID: %d | Descrição: %s | Categoria: %s | Estoque: %d | Diária: %.2f\n",
                       tmp.id, tmp.descricao, tmp.categoria,
                       tmp.qtd_estoque, tmp.valor_diaria);
                encontrados++;
            }

            fclose(fp); // fecho o arquivo após ler tudo

            // Se não encontrei nenhum registro, aviso o usuário
            if (encontrados == 0)
                printf("|  Nenhum equipamento encontrado no arquivo.\n");

            break;
        }

        // ===============================================================
        // MODO BINÁRIO
        // Aqui eu leio o arquivo "equipamentos.bin"
        // Esse modo é mais rápido e ocupa menos espaço, mas não dá pra
        // abrir direto e ler, por isso uso fread()
        // ===============================================================
        case BINARIO: {
            FILE* fp = fopen("equipamentos.bin", "rb"); // abre o arquivo no modo leitura binária
            if (!fp) {
                printf("|  Arquivo 'equipamentos.bin' não encontrado.\n");
                return;
            }

            RecursosEquipamentos tmp;  // struct temporária usada pra leitura
            int encontrados = 0;       // contador de registros

            // Leio um por um até o final do arquivo
            while (fread(&tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) {
                printf("|  ID: %d | Descrição: %s | Categoria: %s | Estoque: %d | Diária: %.2f\n",
                       tmp.id, tmp.descricao, tmp.categoria,
                       tmp.qtd_estoque, tmp.valor_diaria);
                encontrados++;
            }

            fclose(fp); // sempre fecho o arquivo

            // Caso não exista nenhum equipamento no arquivo
            if (encontrados == 0)
                printf("|  Nenhum equipamento encontrado no arquivo.\n");

            break;
        }
    }
    printf("+ --------------------------------------------------------------- +\n");
}

RecursosEquipamentos* buscar_recurso_por_id(TipoArmazenamento tipo, int id) {
    static RecursosEquipamentos recurso;  // uso de static para manter o dado acessível após o retorno
    FILE* arquivo;                      // ponteiro de arquivo usado nos modos TEXTO e BINÁRIO

    switch (tipo) {

        // ------------------------------------------------------------
        // Caso 1: Busca em MEMÓRIA
        // ------------------------------------------------------------
        case MEMORIA:
            // percorro o vetor de recursos que está em memória
            for (int i = 0; i < qtd; i++) {
                // se o ID bater, retorno o endereço do recurso encontrado
                if (equipamentos_memoria[i].id == id) {
                    return &equipamentos_memoria[i];
                }
            }
            // se sair do loop sem retornar, é porque o recurso não foi encontrado
            return NULL;

        // ------------------------------------------------------------
        // Caso 2: Busca em ARQUIVO TEXTO
        // ------------------------------------------------------------
        case TEXTO:
            arquivo = fopen("equipamentos.txt", "r");
            if (!arquivo) {
                printf("Erro: arquivo equipamentos.txt não encontrado.\n");
                return NULL;
            }

            // faço a leitura linha a linha no formato CSV com separador ';'
            while (fscanf(arquivo, "%d;%100[^;];%49[^;];%d;%f;%f;%d;\n",
                          &recurso.id, recurso.descricao, recurso.categoria,
                          &recurso.qtd_estoque, &recurso.preco_custo,
                          &recurso.valor_diaria, &recurso.operador_id) == 7) {

                // se o id lido for igual ao id procurado, já posso devolver o recurso
                if (recurso.id == id) {
                    fclose(arquivo);
                    return &recurso;
                }
            }

            // se terminar a leitura sem encontrar, fecho o arquivo e retorno NULL
            fclose(arquivo);
            return NULL;

        // ------------------------------------------------------------
        // Caso 3: Busca em ARQUIVO BINÁRIO
        // ------------------------------------------------------------
        case BINARIO:
            arquivo = fopen("equipamentos.bin", "rb");
            if (!arquivo) {
                printf("Erro: arquivo equipamentos.bin não encontrado.\n");
                return NULL;
            }

            // leio registro por registro até o final do arquivo
            while (fread(&recurso, sizeof(RecursosEquipamentos), 1, arquivo)) {
                if (recurso.id == id) {
                    fclose(arquivo);
                    return &recurso;
                }
            }

            // se não encontrar, fecho o arquivo e retorno NULL
            fclose(arquivo);
            return NULL;
    }

    // caso algum tipo inválido de armazenamento seja passado
    return NULL;
}
