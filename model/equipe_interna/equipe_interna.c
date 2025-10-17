#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe_interna.h"
#include "../sessao/sessao.h"

// --- Variáveis para armazenamento em memória ---
#define MAX_FUNCIONARIOS 100 // Máximo de funcionários para memória
static EquipeInterna funcionarios_memoria[MAX_FUNCIONARIOS];
static int qtd = 0; // Contador de quantos funcionários já estão salvos na memória

// ========================
// Adicionar funcionário
// ========================
EquipeInterna* adicionar_funcionario(EquipeInterna* funcionario, TipoArmazenamento tipo) {
    if (!funcionario) return NULL; // Se o ponteiro recebido for nulo, quebra o programa

    // Define automaticamente o operador logado como responsável pelo cadastro
    funcionario->operador_id = get_operador_logado();

    int novo_id = 1; // Id começa em 1 por padrão

    switch (tipo) {
        case MEMORIA: {
            if (qtd < MAX_FUNCIONARIOS) {
                // Se houver dados na memória, ele continua com o novo_id = 1, caso contrário, resgatará o
                // último id e somará mais 1
                if (qtd > 0) 
                    novo_id = funcionarios_memoria[qtd - 1].id + 1;

                // Antes de salvar, verifica se o cpf inserido já existe em memória
                for (int i = 0; i < qtd; i++) {
                    if (strcmp(funcionarios_memoria[i].cpf, funcionario->cpf) == 0 &&
                        funcionarios_memoria[i].operador_id == funcionario->operador_id) {
                        printf("\nErro: Já existe um funcionário com o CPF '%s' cadastrado.\n", funcionario->cpf);
                        return NULL;
                    }
                }

                funcionario->id = novo_id;
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
            // Descobre o último id no arquivo e verifica se o cpf recebido já não foi cadastrado
            // por esse operador
            FILE* fp = fopen("funcionarios.txt", "r");
            if (fp) {
                EquipeInterna tmp;
                while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;%d;",
                            &tmp.id, tmp.nome, tmp.cpf, tmp.funcao, &tmp.valor_diaria, &tmp.operador_id) == 6) {
                
                    // Se encontrar mesmo CPF e mesmo operador_id impede o cadastro
                    if (strcmp(tmp.cpf, funcionario->cpf) == 0 &&
                        tmp.operador_id == funcionario->operador_id) {
                        printf("\nErro: Já existe um funcionário com o CPF '%s' cadastrado por este operador.\n",
                            funcionario->cpf);
                        fclose(fp);
                        return NULL;
                    }
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
            fprintf(fp, "%d;%s;%s;%s;%f;%d;\n",
                funcionario->id,
                funcionario->nome,
                funcionario->cpf,
                funcionario->funcao,
                funcionario->valor_diaria,
                funcionario->operador_id
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
                    if (strcmp(tmp.cpf, funcionario->cpf) == 0 &&
                        tmp.operador_id == funcionario->operador_id) {
                        printf("\nErro: Já existe um funcionário com o CPF '%s' cadastrado por este operador.\n",
                               funcionario->cpf);
                        fclose(fp);
                        return NULL;
                    }
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
int atualizar_funcionario_memoria(EquipeInterna* funcionario_memoria, const char* nome, const char* funcao, float valor_diaria) {
    // Verifica se o ponteiro não é nulo
    if (!funcionario_memoria) return 0;

    // Atualiza os dados do funcionario
    // Usar strncpy para evitar que o nome ultrapasse o tamanho do campo, -1 por causa do '\0'
    strncpy(funcionario_memoria->nome, nome, sizeof(funcionario_memoria->nome) - 1);
    funcionario_memoria->nome[sizeof(funcionario_memoria->nome) - 1] = '\0'; 

    strncpy(funcionario_memoria->funcao, funcao, sizeof(funcionario_memoria->funcao) - 1);
    funcionario_memoria->funcao[sizeof(funcionario_memoria->funcao) - 1] = '\0'; 

    funcionario_memoria->valor_diaria = valor_diaria;

    printf("Funcionário %s atualizado na Memória!\n", funcionario_memoria->nome);
    return 1;
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
    int operador_logado = get_operador_logado();

    // Percorre todos os registros  
    while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;%d;",
              &f.id, f.nome, f.cpf, f.funcao, &f.valor_diaria, &f.operador_id) == 6) {
        // Busca no arquivo o funcionário que quero atualizar pelo CPF e verifica se o operador pode atualizá-lo
        if (strcmp(f.cpf, cpf) == 0 && f.operador_id == operador_logado) {
            // strncpy(destino, origem, tamanho)
            strncpy(f.nome, nome, sizeof(f.nome) - 1);
            f.nome[sizeof(f.nome) - 1] = '\0';
            strncpy(f.funcao, funcao, sizeof(f.funcao) - 1);
            f.funcao[sizeof(f.funcao) - 1] = '\0';
            f.valor_diaria = valor_diaria;
            atualizado = 1; 
        }

        // Escreve o funcionario (atualizado ou não) no arquivo temporário
        fprintf(temp, "%d;%s;%s;%s;%f;%d;\n",
            f.id, f.nome, f.cpf, f.funcao, f.valor_diaria, f.operador_id);
    }

    // Fecha os arquivos abertos
    fclose(fp);
    fclose(temp);

    // Apaga o arquivo original e renomeia o temporário para o nome original
    remove("funcionarios.txt");
    rename("funcionarios_temp.txt", "funcionarios.txt");

    if (atualizado)
        printf("Funcionário %s atualizado em arquivo Texto!\n", nome);
    else
        printf("\nErro: funcionário não encontrado para este operador.\n");
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
    int operador_logado = get_operador_logado();

    // Lê cada funcionário do arquivo binário, 1 por vez e retorna 1 se 'sucesso'
    while (fread(&f, sizeof(EquipeInterna), 1, fp) == 1) {
        // Busca no arquivo o funcionário que quero atualizar pelo CPF
        if (strcmp(f.cpf, cpf) == 0 && f.operador_id == operador_logado) {
            // Atualiza os campos permitidos
            strncpy(f.nome, nome, sizeof(f.nome) - 1);
            f.nome[sizeof(f.nome) - 1] = '\0';
            strncpy(f.funcao, funcao, sizeof(f.funcao) - 1);
            f.funcao[sizeof(f.funcao) - 1] = '\0';
            f.valor_diaria = valor_diaria;

            // Move o cursor do arquivo para trás exatamente o tamanho de uma struct EquipeInterna
            fseek(fp, -(long)sizeof(EquipeInterna), SEEK_CUR);
            fwrite(&f, sizeof(EquipeInterna), 1, fp);

            atualizado = 1;
            break;
        }
    }

    fclose(fp); // Fecha o arquivo
    if (atualizado)
        printf("Funcionário %s atualizado em arquivo Binário!\n", nome);
    else
        printf("\nErro: funcionário não encontrado para este operador.\n");
    return atualizado;
}

// ====================
// Deletar Funcionário
// ====================
int deletar_funcionario(const char* cpf, TipoArmazenamento tipo) {
    int operador_logado = get_operador_logado();
    int removido = 0; // flag para indicar se encontrou e removeu o funcionário

    switch (tipo) {
        // ============================
        // Armazenamento em MEMÓRIA
        // ============================
        case MEMORIA: {
            for (int i = 0; i < qtd; i++) {
                // Verifica CPF e se pertence ao operador logado
                if (strcmp(funcionarios_memoria[i].cpf, cpf) == 0 &&
                    funcionarios_memoria[i].operador_id == operador_logado) {
                    // Move todos os funcionários após ele uma posição para trás
                    for (int j = i; j < qtd - 1; j++) {
                        funcionarios_memoria[j] = funcionarios_memoria[j + 1];
                    }
                    qtd--; // diminui a quantidade
                    removido = 1;
                    break;
                }
            }
            break;
        }

        // ============================
        // Armazenamento em TEXTO
        // ============================
        case TEXTO: {
            FILE* fp = fopen("funcionarios.txt", "r");
            FILE* temp = fopen("funcionarios_temp.txt", "w");
            if (!fp || !temp) {
                perror("Erro ao abrir arquivo de funcionarios");
                if (fp) fclose(fp);
                if (temp) fclose(temp);
                return 0;
            }

            EquipeInterna f;

            while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;%d;\n",
                          &f.id, f.nome, f.cpf, f.funcao, &f.valor_diaria, &f.operador_id) == 6) {
                // Se não for o funcionário do operador logado, grava no arquivo temporário
                if (!(strcmp(f.cpf, cpf) == 0 && f.operador_id == operador_logado)) {
                    fprintf(temp, "%d;%s;%s;%s;%f;%d;\n",
                            f.id, f.nome, f.cpf, f.funcao, f.valor_diaria, f.operador_id);
                } else {
                    removido = 1; // marcou como removido
                }
            }

            fclose(fp);
            fclose(temp);

            remove("funcionarios.txt");
            rename("funcionarios_temp.txt", "funcionarios.txt");
            break;
        }

        // ============================
        // Armazenamento em BINÁRIO
        // ============================
        case BINARIO: {
            FILE* fp = fopen("funcionarios.bin", "rb");
            FILE* temp = fopen("funcionarios_temp.bin", "wb");
            if (!fp || !temp) {
                perror("Erro ao abrir arquivo binário");
                if (fp) fclose(fp);
                if (temp) fclose(temp);
                return 0;
            }

            EquipeInterna f;

            while (fread(&f, sizeof(EquipeInterna), 1, fp) == 1) {
                // Copia todos os registros exceto o que será removido
                if (!(strcmp(f.cpf, cpf) == 0 && f.operador_id == operador_logado)) {
                    fwrite(&f, sizeof(EquipeInterna), 1, temp);
                } else {
                    removido = 1; // marcou como removido
                }
            }

            fclose(fp);
            fclose(temp);

            remove("funcionarios.bin");
            rename("funcionarios_temp.bin", "funcionarios.bin");
            break;
        }

        // ============================
        // Caso inválido
        // ============================
        default:
            printf("\nErro: tipo de armazenamento inválido!\n");
            break;
    }

    return removido;
}

// ================================================
// Retorna a quantidade de funcionários na memória
// ================================================
int get_qtd_funcionarios() { 
    return qtd; 
} 

// ======================
// Buscar Funcionário 
// ======================
EquipeInterna* buscar_funcionario_por_cpf(const char* cpf_busca, TipoArmazenamento tipo) {
    int operador_logado = get_operador_logado();

    // Ponteiro estático para manter os dados válidos fora da função
    static EquipeInterna funcionario_tmp;

    switch (tipo) {
        // ======================
        // Caso 1: Memória
        // ======================
        case MEMORIA: {
            for (int i = 0; i < get_qtd_funcionarios(); i++) {
                if (strcmp(funcionarios_memoria[i].cpf, cpf_busca) == 0 &&
                    funcionarios_memoria[i].operador_id == operador_logado) {
                    return &funcionarios_memoria[i];
                }
            }
            break;
        }

        // ======================
        // Caso 2: Texto
        // ======================
        case TEXTO: {
            FILE* fp = fopen("funcionarios.txt", "r");
            if (!fp) {
                perror("Erro ao abrir funcionarios.txt");
                return NULL;
            }

            // Busca os dados do cpf desejado dentro do arquivo txt
            while (fscanf(fp, "%d;%49[^;];%19[^;];%99[^;];%f;%d;\n",
                          &funcionario_tmp.id, funcionario_tmp.nome, funcionario_tmp.cpf, 
                          funcionario_tmp.funcao, &funcionario_tmp.valor_diaria, 
                          &funcionario_tmp.operador_id) == 6) {
                if (strcmp(funcionario_tmp.cpf, cpf_busca) == 0 &&
                    funcionario_tmp.operador_id == operador_logado) {
                    fclose(fp);
                    return &funcionario_tmp;
                }
            }

            fclose(fp);
            break;
        }

        // ======================
        // Caso 3: Binário
        // ======================
        case BINARIO: {
            FILE* fp = fopen("funcionarios.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir funcionarios.bin");
                return NULL;
            }

            while (fread(&funcionario_tmp, sizeof(EquipeInterna), 1, fp) == 1) {
                if (strcmp(funcionario_tmp.cpf, cpf_busca) == 0 &&
                    funcionario_tmp.operador_id == operador_logado) {
                    fclose(fp);
                    return &funcionario_tmp;
                }
            }

            fclose(fp);
            break;
        }

        default:
            printf("Tipo de armazenamento inválido!");
            break;
    }

    return NULL; // Não encontrado
}
