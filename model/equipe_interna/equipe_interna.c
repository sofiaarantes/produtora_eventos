#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe_interna.h"

// --- Variáveis para armazenamento em memória ---
#define MAX_FUNCIONARIOS 100 // Máximo de funcionários para memória
static EquipeInterna funcionarios_memoria[MAX_FUNCIONARIOS];
static int qtd = 0; // Contador de quantos funcionários já estão salvos na memória

// ========================
// Adicionar funcionário
// ========================

EquipeInterna* adicionar_funcionario(EquipeInterna* funcionario, TipoArmazenamento tipo) {
    if (!funcionario) return NULL; // Se o ponteiro recebido for nulo, quebra o programa

    int novo_id = 1; // Id começa em 1 por padrão

    switch (tipo) {
        case MEMORIA: {
            if (qtd < MAX_FUNCIONARIOS) {
                // Se houver dados na memória, ele continua com o novo_id = 1, caso contrário, resgatará o
                // último id e somará mais 1
                if (qtd > 0) 
                    novo_id = funcionarios_memoria[qtd - 1].id + 1;
                // Copia os dados do funcionário passado para o array de memória na posição qtd que ele representa
                funcionarios_memoria[qtd] = *funcionario;
                // Ponteiro para o funcionário que acabou de ser salvo
                EquipeInterna* salvo = &funcionarios_memoria[qtd];

                qtd++; // Incrementa o contador de funcionários na memória

                printf("Funcionário %s salvo em Memória!\n", salvo->nome);
                return salvo; // Retorna o endereço do funcion salvo
            } else {
                printf("Erro: limite de funcionarios na memória atingido!\n");
                return NULL;
            }
        }
        
        case TEXTO: {
            // Descobre o último id no arquivo
            FILE* fp = fopen("funcionarios.txt", "r");
            if (fp) {
                EquipeInterna tmp;
                while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;\n",
                              &tmp.id, tmp.nome, tmp.cpf, tmp.funcao, &tmp.valor_diaria) != EOF) {
                    novo_id = tmp.id + 1; // sempre fica com o último id lido +1
                }
                fclose(fp);
            }
            funcionario->id = novo_id;

            // Reabre o arquivo em modo append(Abre no final do arquivo) para não sobrescrever os 
            // funcionarios anteriores
            fp = fopen("funcionarios.txt", "a");
            if (!fp) {
                perror("Erro ao abrir funcionarios.txt");
                return NULL;
            }

            // Escreve os dados do funcion no arquivo separados por ponto e vírgula
            fprintf(fp, "%d;%s;%s;%s;%f;\n",
                funcionario->id,
                funcionario->nome,
                funcionario->cpf,
                funcionario->funcao,
                funcionario->valor_diaria
            );

            fclose(fp); // Fecha o arquivo
            printf("Funcionario %s salvo em arquivo Texto!\n", funcionario->nome);
            return funcionario; // Retorna o funcion que foi salvo
        }

        case BINARIO: {
            // Descobre último id no arquivo
            FILE* fp = fopen("funcionarios.bin", "rb");
            if (fp) {
                EquipeInterna tmp;
                while (fread(&tmp, sizeof(EquipeInterna), 1, fp) == 1) {
                    novo_id = tmp.id + 1;
                }
                fclose(fp);
            }
            funcionario->id = novo_id;

            // Abre o arquivo em modo append(final do arquivo) binário
            fp = fopen("funcionarios.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir funcionarios.bin");
                return NULL;
            }

            // Grava a struct recebida no arquivo binário
            fwrite(funcionario, sizeof(EquipeInterna), 1, fp);

            fclose(fp); // Fecha o arquivo
            printf("Funcionario %s salvo em Binário!\n", funcionario->nome);
            return funcionario; // retorno o funcion que foi salvo
        }

        // Caso inválido
        default:
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
    }
}

// ================================
// Atualizar funcionário - MEMÓRIA
// ================================
void atualizar_funcionario_memoria(EquipeInterna* funcionario_memoria, const char* nome, const char* funcao, float valor_diaria) {
    // Verifica se o ponteiro não é nulo
    if (!funcionario_memoria) return;

    // Atualiza os dados do funcionario
    // Usar strncpy para evitar que o nome ultrapasse o tamanho do campo, -1 por causa do '\0'
    strncpy(funcionario_memoria->nome, nome, sizeof(funcionario_memoria->nome) - 1);
    funcionario_memoria->nome[sizeof(funcionario_memoria->nome) - 1] = '\0'; 

    strncpy(funcionario_memoria->funcao, funcao, sizeof(funcionario_memoria->funcao) - 1);
    funcionario_memoria->funcao[sizeof(funcionario_memoria->funcao) - 1] = '\0'; 

    funcionario_memoria->valor_diaria = valor_diaria;

    printf("Funcionário %s atualizado na Memória!\n", funcionario_memoria->nome);
}

// ==============================
// Atualizar Funcionário - TEXTO
// ==============================
int atualizar_funcionario_texto(const char* cpf, const char* nome, const char* funcao, float valor_diaria) {

    // Abre o arquivo original na função de ler(read)
    FILE* fp = fopen("funcionarios.txt", "r");
    // Cria um arquivo temporário para escrever os funcionários atualizados (write)
    FILE* temp = fopen("funcionarios_temp.txt", "w");
    // Se não conseguir abrir algum dos arquivos, mostra erro e sai
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo de funcionarios");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0; 
    }

    // Lê cada funcion do arquivo original
    EquipeInterna f;
    int atualizado = 0;
    while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;\n",
                  &f.id, f.nome, f.cpf, f.funcao, &f.valor_diaria) != EOF) {

        // Busca no arquivo o funcionário que quero atualizar pelo CPF
        if (strcmp(f.cpf, cpf) == 0) {
            // Atualiza os campos permitidos
            strncpy(f.nome, nome, sizeof(f.nome) - 1);
            f.nome[sizeof(f.nome) - 1] = '\0';
            strncpy(f.funcao, funcao, sizeof(f.funcao) - 1);
            f.funcao[sizeof(f.funcao) - 1] = '\0';
            f.valor_diaria = valor_diaria;
            atualizado = 1;
        }

        // Escreve o funcion (atualizado ou não) no arquivo temporário
        fprintf(temp, "%d;%s;%s;%s;%f\n",
                f.id, f.nome, f.cpf, f.funcao, f.valor_diaria);
    }

    // Fecha os arquivos abertos
    fclose(fp);
    fclose(temp);

    // Apaga o arquivo original e renomeia o temporário para o nome original
    remove("funcionarios.txt");
    rename("funcionarios_temp.txt", "funcionarios.txt");

    printf("Funcionário %s atualizado em arquivo Texto!\n", nome);
    return atualizado;
}

// ==============================
// Atualizar Funcionário - BINÁRIO
// ==============================
int atualizar_funcionario_binario(const char* cpf, const char* nome, const char* funcao, float valor_diaria) {
    // Abre o arquivo binário para leitura e escrita
    FILE* fp = fopen("funcionarios.bin", "r+b");
    if (!fp) {
        perror("Erro ao abrir funcionarios.bin");
        return 0;
    }

    EquipeInterna f;
    int atualizado = 0;
    // Lê cada funcionário do arquivo binário
    while (fread(&f, sizeof(EquipeInterna), 1, fp) == 1) {
        // Busca no arquivo o funcionário que quero atualizar pelo CPF
        if (strcmp(f.cpf, cpf) == 0) {
            // Atualiza os campos permitidos
            strncpy(f.nome, nome, sizeof(f.nome) - 1);
            f.nome[sizeof(f.nome) - 1] = '\0';
            strncpy(f.funcao, funcao, sizeof(f.funcao) - 1);
            f.funcao[sizeof(f.funcao) - 1] = '\0';
            f.valor_diaria = valor_diaria;

            // Move o ponteiro do arquivo de volta para o início do registro que acabou de ler
            fseek(fp, -(long)sizeof(EquipeInterna), SEEK_CUR);
            fwrite(&f, sizeof(EquipeInterna), 1, fp);

            printf("funcion %s atualizado no arquivo Binário!\n", nome);
            atualizado = 1;
        }
    }

    fclose(fp); // Fecha o arquivo
    return atualizado;
}


// ==============================
// Deletar Funcionário - MEMORIA
// ==============================
int deletar_funcionario_memoria(const char* cpf) {
    // Procura o funcionário na memória
    for (int i = 0; i < qtd; i++) {
        if (strcmp(funcionarios_memoria[i].cpf, cpf) == 0) {
            // Move todos os funcionários após ele uma posição para trás
            for (int j = i; j < qtd - 1; j++) {
                funcionarios_memoria[j] = funcionarios_memoria[j + 1];
            }
            qtd--; // Diminui a quantidade
            return 1; // Sucesso
        }
    }
    return 0; // Não encontrou
}


// ============================
// Deletar Funcionário - TEXTO
// ============================
int deletar_funcionario_texto(const char* cpf) {
    FILE* fp = fopen("funcionarios.txt", "r");
    FILE* temp = fopen("funcionarios_temp.txt", "w");
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    EquipeInterna f;
    int removido = 0;
    // Lê linha por linha do arquivo original
    while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;\n",
                  &f.id, f.nome, f.cpf, f.funcao, &f.valor_diaria) != EOF) {
        // Se não for o CPF que quero remover, regravo no temp
        if (strcmp(f.cpf, cpf) != 0) {
            fprintf(temp, "%d;%s;%s;%s;%f;\n",
                f.id, f.nome, f.cpf, f.funcao, f.valor_diaria);
        } else {
            removido = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("funcionarios.txt");
    rename("funcionarios_temp.txt", "funcionarios.txt");

    return removido;
}


// =============================
// Deletar Funcionário - BINÁRIO
// =============================
int deletar_funcionario_binario(const char* cpf) {
    FILE* fp = fopen("funcionarios.bin", "rb");
    FILE* temp = fopen("funcionarios_temp.bin", "wb");
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    EquipeInterna f;
    int removido = 0;
    // Lê cada struct do arquivo original
    while (fread(&f, sizeof(EquipeInterna), 1, fp) == 1) {
        // Copia todos MENOS o que quero remover
        if (strcmp(f.cpf, cpf) != 0) {
            fwrite(&f, sizeof(EquipeInterna), 1, temp);
        } else {
            removido = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("funcionarios.bin");
    rename("funcionarios_temp.bin", "funcionarios.bin");

    return removido;
}

// ================================================
// Retorna a quantidade de funcionários na memória
// ================================================
int get_qtd_funcionarios() { 
    return qtd; 
} 


// ==============================
// Buscar Funcionário - MEMÓRIA
// ==============================
EquipeInterna* buscar_funcionario_por_cpf_memoria(const char* cpf_busca) {
    // Uso um loop for para percorrer o array de funcionários na memória
    int i;
    for ( i = 0; i < get_qtd_funcionarios(); i++) {
        // Compara o CPF do funcionário na memória com o que foi passado para busca
        if (strcmp(funcionarios_memoria[i].cpf, cpf_busca) == 0) {
            // Retorna o endereço do funcionário encontrado
            return &funcionarios_memoria[i];
        }
    }
    // Se não encontrar, retorna NULL
    return NULL;
}

// ==============================
// Buscar Funcionário - TEXTO
// ==============================
EquipeInterna* buscar_funcionario_por_cpf_texto(const char* cpf_busca) {
    // Abre o arquivo de texto que contém os funcionários
    FILE* fp = fopen("funcionarios.txt", "r");
    if (!fp) {
        perror("Erro ao abrir funcionarios.txt");
        return NULL;
    }

    // Uso static para que o ponteiro continue válido fora da função
    static EquipeInterna funcionario_tmp;

    // Lê cada funcionário do arquivo e se for igual ao CPF buscado, retorna o ponteiro para ele
    while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;\n",
                  &funcionario_tmp.id,
                  funcionario_tmp.nome,
                  funcionario_tmp.cpf,
                  funcionario_tmp.funcao,
                  &funcionario_tmp.valor_diaria) == 5) {

        if (strcmp(funcionario_tmp.cpf, cpf_busca) == 0) {
            // Achou o funcionário! Fecha o arquivo e retorna o ponteiro
            fclose(fp);
            return &funcionario_tmp;
        }
    }

    // Se não encontrar, fecha o arquivo e retorna NULL
    fclose(fp);
    return NULL;
}

// ==============================
// Buscar Funcionário - BINÁRIO
// ==============================
EquipeInterna* buscar_funcionario_por_cpf_binario(const char* cpf_busca) {
    // Abre o arquivo binário que contém os funcionários
    FILE* fp = fopen("funcionarios.bin", "rb");
    if (!fp) {
        perror("Erro ao abrir funcionarios.bin");
        return NULL;
    }

    // Lê cada funcionário do arquivo binário
    static EquipeInterna funcionario_tmp; // static para manter o ponteiro válido
    while (fread(&funcionario_tmp, sizeof(EquipeInterna), 1, fp) == 1) {
        // Compara o CPF do funcionário lido com o CPF buscado
        if (strcmp(funcionario_tmp.cpf, cpf_busca) == 0) {
            // Se encontrar, fecha o arquivo e retorna o ponteiro para o funcionário
            fclose(fp);
            return &funcionario_tmp;
        }
    }

    // Se não encontrar, fecha o arquivo e retorna NULL
    fclose(fp);
    return NULL;
}
