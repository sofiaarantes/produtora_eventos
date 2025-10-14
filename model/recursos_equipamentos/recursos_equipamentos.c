#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursos_equipamentos.h"

// --- Variáveis para armazenamento em memória ---
#define MAX_EQUIPAMENTOS 100 // Máximo de equipamentos para memória
static RecursosEquipamentos equipamentos_memoria[MAX_EQUIPAMENTOS];
static int qtd = 0; // Contador de quantos equipamentos já estão salvos na memória

// =============================
// Adicionar equipamento/recurso
// =============================
RecursosEquipamentos* adicionar_equipamento(RecursosEquipamentos* equipamento, TipoArmazenamento tipo) {
    if (!equipamento) return NULL; // Se o ponteiro recebido for nulo, quebra o programa

    int novo_id = 1; // Id começa em 1 por padrão

    switch (tipo) {
        case MEMORIA: {
            if (qtd < MAX_EQUIPAMENTOS) {
                // Se houver dados na memória, ele continua com o novo_id = 1, caso contrário, resgatará o
                // último id e somará mais 1
                if (qtd > 0) 
                    novo_id = equipamentos_memoria[qtd - 1].id + 1;

                equipamento->id = novo_id;
                // Copia os dados do equipamento passado para o array de memória na posição qtd que ele representa
                equipamentos_memoria[qtd] = *equipamento;
                // Ponteiro para o equipamento que acabou de ser salvo
                RecursosEquipamentos* salvo = &equipamentos_memoria[qtd];

                qtd++; // Incrementa o contador de equipamentos na memória

                printf("Equipamento %s salvo em Memória! O id do equipamento é %d\n", 
                salvo->descricao, equipamento->id);
                return salvo; // Retorna o endereço do funcion salvo
            } else {
                printf("Erro: limite de equipamentos na memória atingido!\n");
                return NULL;
            }
        }
        
        case TEXTO: {
            // Descobre o último id no arquivo
            FILE* fp = fopen("equipamentos.txt", "r");
            if (fp) {
                RecursosEquipamentos tmp;
                while (fscanf(fp, "%d;%49[^;];%49[^;];%d;%f;%f;\n",
                              &tmp.id, tmp.descricao, tmp.categoria,
                              &tmp.qtd_estoque, &tmp.preco_custo, &tmp.valor_diaria) != EOF) {
                    novo_id = tmp.id + 1; // sempre fica com o último id lido +1
                }
                fclose(fp);
            }
            equipamento->id = novo_id;

            // Reabre o arquivo em modo append(abre no fim do arquivo) para não sobrescrever os
            // equipamentos anteriores
            fp = fopen("equipamentos.txt", "a");
            if (!fp) {
                perror("Erro ao abrir equipamentos.txt");
                return NULL;
            }

            // Escreve os dados do funcion no arquivo separados por ponto e vírgula
            fprintf(fp, "%d;%s;%s;%d;%f;%f;\n",
                equipamento->id,
                equipamento->descricao,
                equipamento->categoria,
                equipamento->qtd_estoque,
                equipamento->preco_custo,
                equipamento->valor_diaria
            );

            fclose(fp); // Fecha o arquivo
            printf("Equipamento %s salvo em arquivo Texto! O id do equipamento é %d\n", 
                equipamento->descricao, equipamento->id);
            return equipamento; // Retorna o equipamento que foi salvo
        }

        case BINARIO: {
            // Descobre último id no arquivo
            FILE* fp = fopen("equipamentos.bin", "rb");
            if (fp) {
                RecursosEquipamentos tmp;
                while (fread(&tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) {
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

            fclose(fp); // Fecha o arquivo
            printf("Equipamento %s salvo em Binário! O id do equipamento é %d\n", 
                equipamento->descricao, equipamento->id);
            return equipamento; // Retorna o equipamento que foi salvo
        }

        // Caso inválido
        default:
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
    }
}

// ================================
// Atualizar equipamento - MEMÓRIA
// ================================
void atualizar_equipamento_memoria(RecursosEquipamentos* equipamento_memoria, const char* descricao, const char* categoria, 
                    int qtd_estoque, float preco_custo, float valor_diaria) {
    // Verifica se o ponteiro não é nulo
    if (!equipamento_memoria) return;

    // Atualiza os dados do equipamento
    // Usar strncpy para evitar que o nome ultrapasse o tamanho do campo, -1 por causa do '\0'
    strncpy(equipamento_memoria->descricao, descricao, sizeof(equipamento_memoria->descricao) - 1);
    equipamento_memoria->descricao[sizeof(equipamento_memoria->descricao) - 1] = '\0'; 

    strncpy(equipamento_memoria->categoria, categoria, sizeof(equipamento_memoria->categoria) - 1);
    equipamento_memoria->categoria[sizeof(equipamento_memoria->categoria) - 1] = '\0'; 

    equipamento_memoria->qtd_estoque = qtd_estoque;
    equipamento_memoria->preco_custo = preco_custo;
    equipamento_memoria->valor_diaria = valor_diaria;

    printf("Equipamento atualizado na Memória!\n");
}

// ==============================
// Atualizar equipamento - TEXTO
// ==============================
int atualizar_equipamento_texto(int id, const char* descricao, const char* categoria, 
                    int qtd_estoque, float preco_custo, float valor_diaria) {

    // Abre o arquivo original na função de ler(read)
    FILE* fp = fopen("equipamentos.txt", "r");
    // Cria um arquivo temporário para escrever os equipamentos atualizados (write)
    FILE* temp = fopen("equipamentos_temp.txt", "w");
    // Se não conseguir abrir algum dos arquivos, mostra erro e sai
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo de equipamentos");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0; 
    }

    // Lê cada equipamento do arquivo original
    RecursosEquipamentos e;
    int atualizado = 0;
    while (fscanf(fp, "%d;%99[^;];%49[^;];%d;%f;%f;\n",
                  &e.id, e.descricao, e.categoria, &e.qtd_estoque, &e.preco_custo
                  , &e.valor_diaria) != EOF) {

        // Busca no arquivo o equipamento que quero atualizar pelo CPF
        if (e.id == id) {
            // Atualiza os campos permitidos
            strncpy(e.descricao, descricao, sizeof(e.descricao) - 1);
            e.descricao[sizeof(e.descricao) - 1] = '\0';
            strncpy(e.categoria, categoria, sizeof(e.categoria) - 1);
            e.categoria[sizeof(e.categoria) - 1] = '\0';
            e.qtd_estoque = qtd_estoque;
            e.preco_custo = preco_custo;
            e.valor_diaria = valor_diaria;
            atualizado = 1;
        }

        // Escreve o funcion (atualizado ou não) no arquivo temporário
        fprintf(temp, "%d;%s;%s;%d;%f;%f\n",
                e.id, e.descricao, e.categoria, e.qtd_estoque, e.preco_custo, e.valor_diaria);
    }

    // Fecha os arquivos abertos
    fclose(fp);
    fclose(temp);

    // Apaga o arquivo original e renomeia o temporário para o nome original
    remove("equipamentos.txt");
    rename("equipamentos_temp.txt", "equipamentos.txt");

    printf("Equipamento atualizado em arquivo Texto!\n");
    return atualizado;
}

// ==============================
// Atualizar equipamento - BINÁRIO
// ==============================
int atualizar_equipamento_binario(int id, const char* descricao, const char* categoria, 
                    int qtd_estoque, float preco_custo, float valor_diaria) {
    // Abre o arquivo binário para leitura e escrita
    FILE* fp = fopen("equipamentos.bin", "r+b");
    if (!fp) {
        perror("Erro ao abrir equipamentos.bin");
        return 0;
    }

    RecursosEquipamentos e;
    int atualizado = 0;
    // Lê cada equipamento do arquivo binário
    while (fread(&e, sizeof(RecursosEquipamentos), 1, fp) == 1) {
        // Busca no arquivo o equipamento que quero atualizar pelo id
        if (e.id == id) {
            // Atualiza os campos permitidos
            strncpy(e.descricao, descricao, sizeof(e.descricao) - 1);
            e.descricao[sizeof(e.descricao) - 1] = '\0';
            strncpy(e.categoria, categoria, sizeof(e.categoria) - 1);
            e.categoria[sizeof(e.categoria) - 1] = '\0';
            e.qtd_estoque = qtd_estoque;
            e.preco_custo = preco_custo;
            e.valor_diaria = valor_diaria;

            // Move o ponteiro do arquivo de volta para o início do registro que acabou de ler
            fseek(fp, -(long)sizeof(RecursosEquipamentos), SEEK_CUR);
            fwrite(&e, sizeof(RecursosEquipamentos), 1, fp);

            printf("Equipamento atualizado no arquivo Binário!\n");
            atualizado = 1;
        }
    }

    fclose(fp); // Fecha o arquivo
    return atualizado;
}


// ==============================
// Deletar equipamento - MEMORIA
// ==============================
int deletar_equipamento_memoria(int id) {
    // Procura o equipamento na memória
    for (int i = 0; i < qtd; i++) {
        if (equipamentos_memoria[i].id == id) {
            // Move todos os equipamentos após ele uma posição para trás
            for (int j = i; j < qtd - 1; j++) {
                equipamentos_memoria[j] = equipamentos_memoria[j + 1];
            }
            qtd--; // Diminui a quantidade
            return 1; // Sucesso
        }
    }
    return 0; // Não encontrou
}


// ============================
// Deletar equipamento - TEXTO
// ============================
int deletar_equipamento_texto(int id) {
    FILE* fp = fopen("equipamentos.txt", "r");
    FILE* temp = fopen("equipamentos_temp.txt", "w");
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    RecursosEquipamentos e;
    int removido = 0;
    // Lê linha por linha do arquivo original
    while (fscanf(fp, "%d;%99[^;];%49[^;];%d;%f;%f;\n",
                  &e.id, e.descricao, e.categoria, &e.qtd_estoque, &e.preco_custo, &e.valor_diaria) != EOF) {
        // Se não for o id que quero remover, regravo no temp
        if (e.id != id) {
            fprintf(temp, "%d;%s;%s;%d;%f;%f;\n",
                e.id, e.descricao, e.categoria, e.qtd_estoque, e.preco_custo, e.valor_diaria);
        } else {
            removido = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("equipamentos.txt");
    rename("equipamentos_temp.txt", "equipamentos.txt");

    return removido;
}


// =============================
// Deletar equipamento - BINÁRIO
// =============================
int deletar_equipamento_binario(int id) {
    FILE* fp = fopen("equipamentos.bin", "rb");
    FILE* temp = fopen("equipamentos_temp.bin", "wb");
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    RecursosEquipamentos e;
    int removido = 0;
    // Lê cada struct do arquivo original
    while (fread(&e, sizeof(RecursosEquipamentos), 1, fp) == 1) {
        // Copia todos MENOS o que quero remover
        if (e.id != id) {
            fwrite(&e, sizeof(RecursosEquipamentos), 1, temp);
        } else {
            removido = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("equipamentos.bin");
    rename("equipamentos_temp.bin", "equipamentos.bin");

    return removido;
}

// ================================================
// Retorna a quantidade de equipamentos na memória
// ================================================
int get_qtd_equipamentos() { 
    return qtd; 
} 


// ==============================
// Buscar equipamento - MEMÓRIA
// ==============================
RecursosEquipamentos* buscar_equipamento_por_id_memoria(int id_busca) {
    // Uso um loop for para percorrer o array de equipamentos na memória
    int i;
    for ( i = 0; i < get_qtd_equipamentos(); i++) {
        // Compara o CPF do equipamento na memória com o que foi passado para busca
        if (equipamentos_memoria[i].id == id_busca) {
            // Retorna o endereço do equipamento encontrado
            return &equipamentos_memoria[i];
        }
    }
    // Se não encontrar, retorna NULL
    return NULL;
}

// ==============================
// Buscar equipamento - TEXTO
// ==============================
RecursosEquipamentos* buscar_equipamento_por_id_texto(int id_busca) {
    // Abre o arquivo de texto que contém os equipamentos
    FILE* fp = fopen("equipamentos.txt", "r");
    if (!fp) {
        perror("Erro ao abrir equipamentos.txt");
        return NULL;
    }

    // Uso static para que o ponteiro continue válido fora da função
    static RecursosEquipamentos equipamento_tmp;

    // Lê cada equipamento do arquivo e se for igual ao CPF buscado, retorna o ponteiro para ele
    while (fscanf(fp, "%d;%99[^;];%49[^;];%d;%f;%f;\n",
                  &equipamento_tmp.id,
                  equipamento_tmp.descricao,
                  equipamento_tmp.categoria,
                  &equipamento_tmp.qtd_estoque,
                  &equipamento_tmp.preco_custo,
                  &equipamento_tmp.valor_diaria) == 6) {

        if (equipamento_tmp.id == id_busca) {
            // Achou o equipamento! Fecha o arquivo e retorna o ponteiro
            fclose(fp);
            return &equipamento_tmp;
        }
    }

    // Se não encontrar, fecha o arquivo e retorna NULL
    fclose(fp);
    return NULL;
}

// ==============================
// Buscar equipamento - BINÁRIO
// ==============================
RecursosEquipamentos* buscar_equipamento_por_id_binario(int id_busca) {
    // Abre o arquivo binário que contém os equipamentos
    FILE* fp = fopen("equipamentos.bin", "rb");
    if (!fp) {
        perror("Erro ao abrir equipamentos.bin");
        return NULL;
    }

    // Lê cada equipamento do arquivo binário
    static RecursosEquipamentos equipamento_tmp; // static para manter o ponteiro válido
    while (fread(&equipamento_tmp, sizeof(RecursosEquipamentos), 1, fp) == 1) {
        // Compara o ID do equipamento lido com o ID buscado
        if (equipamento_tmp.id == id_busca) {
            // Se encontrar, fecha o arquivo e retorna o ponteiro para o equipamento
            fclose(fp);
            return &equipamento_tmp;
        }
    }

    // Se não encontrar, fecha o arquivo e retorna NULL
    fclose(fp);
    return NULL;
}
