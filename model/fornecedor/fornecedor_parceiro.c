#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor_parceiro.h"
#include "../../view/fornecedor/fornecedor_parceiro_view.h"
#include "../../view/main/main_view.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"


#define MAX_FORNECEDORES 100
static Fornecedor_parceiro fornecedores_memoria[MAX_FORNECEDORES]; // cada posição guarda um fornecedor/parceiro
static int qtd = 0; // contador de quantos fornecedores/parceiros já estão salvos na memória

int novo_id = 1; // Id começa em 1 por padrão

Fornecedor_parceiro* criar_fornecedor_parceiro(Fornecedor_parceiro* fornecedor, TipoArmazenamento tipo) {
    // Se o ponteiro fornecedor for nulo, nao há como salvar — retorno NULL
    if (!fornecedor) return NULL;

    // Começo o ID em 1. Caso haja fornecedores anteriores, ele será atualizado mais abaixo
    int novo_id = 1;

    fornecedor->id_logado = get_operador_logado(); // Associo o fornecedor ao operador logado

    // Escolho o tipo de armazenamento que o sistema está usando (memória, texto ou binário)
    switch (tipo) {
        

        // ============================================
        // CASO 1 - SALVAR NA MEMORIA
        // ============================================
        case MEMORIA: {
            // Verifico se ainda há espaço no array de fornecedors em memória
            if (qtd < MAX_FORNECEDORES) {

                // Se já existir pelo menos um fornecedor, pego o último ID e incremento
                if (qtd > 0)
                    novo_id = fornecedores_memoria[qtd - 1].id + 1;

                // Atribuo o novo ID ao fornecedor atual
                fornecedor->id = novo_id;

                // Salvo o fornecedor no vetor em memória
                fornecedores_memoria[qtd] = *fornecedor;

                // Crio um ponteiro que aponta para o fornecedor recém-salvo
                Fornecedor_parceiro* salvo = &fornecedores_memoria[qtd];

                fornecedor->id_logado = get_operador_logado(); // garante que está atualizado

                // Incremento a contagem total de fornecedors
                qtd++;

                printf("Fornecedor/Parceiro %s salvo em MEMORIA!\n",fornecedor->nome_fantasia );

                // Retorno o ponteiro do fornecedores que acabou de ser salvo
                return salvo;
            } else {
                // Caso o vetor esteja cheio, nao consigo salvar
                printf("Erro: limite de fornedores/parceiros na memória atingido!\n");
                return NULL;
            }
        }


        // ============================================
        // CASO 2 - SALVAR EM ARQUIVO TEXTO
        // ============================================
        case TEXTO: {
            // Abro o arquivo de texto para leitura, para descobrir o último ID usado
            FILE* fp = fopen("fornecedores.txt", "r");
            if (fp) {
                char linha[512];  // buffer para armazenar cada linha lida
                Fornecedor_parceiro tmp;      // struct temporária para ler os dados do arquivo

                // Lê o arquivo linha por linha de forma segura (fgets evita travamentos)
                while (fgets(linha, sizeof(linha), fp)) {
                    // Tenta ler até os campos formatados por ponto e vírgula
                    // %[^;] significa “ler até encontrar um ponto e vírgula”
                    int lidos = sscanf(linha, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];%d",
                                        &tmp.id, tmp.nome_fantasia, tmp.razao_social,
                                        tmp.cnpj, tmp.endereco_completo,
                                        tmp.tel, tmp.tipo_servico, &tmp.id_logado);

                    // Se conseguiu ler pelo menos o ID, atualiza o próximo id
                    if (lidos >= 1) {
                        novo_id = tmp.id + 1;
                    }
                }
                // Fecha o arquivo após ler todas as linhas
                fclose(fp);
            }

            // Define o novo ID no fornecedor que será inserido
            fornecedor->id = novo_id;

            fornecedor->id_logado = get_operador_logado(); // Associo o fornecedor ao operador logado

            // Agora abre o arquivo em modo append (“a”) para adicionar o novo fornecedor no final
            fp = fopen("fornecedores.txt", "a");
            if (!fp) {
                perror("Erro ao abrir fornecedores.txt");
                return NULL;
            }

            // Grava os dados do fornecedor em uma linha formatada, separados por ponto e vírgula
            fprintf(fp, "%d;%s;%s;%s;%s;%s;%s;%d\n",
                    fornecedor->id,
                    fornecedor->nome_fantasia,
                    fornecedor->razao_social,
                    fornecedor->endereco_completo,
                    fornecedor->cnpj,
                    fornecedor->tel,
                    fornecedor->tipo_servico,
                    fornecedor->id_logado
                );

            // Fecha o arquivo após a escrita
            fclose(fp);

            printf("Fornecedor/Parceiro %s salvo em TEXTO!\n", fornecedor->nome_fantasia);

            // Retorna o ponteiro para o fornecedor criado
            return fornecedor;
        }


        // ============================================
        // CASO 3 - SALVAR EM ARQUIVO BINARIO
        // ============================================
        case BINARIO: {
            // Abre o arquivo binário em modo leitura ("rb") para descobrir o último ID
            FILE* fp = fopen("fornecedores.bin", "rb");
            if (fp) {
                Fornecedor_parceiro tmp;
                // Lê fornecedor por fornecedor até o final do arquivo
                while (fread(&tmp, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                    novo_id = tmp.id + 1; // sempre pega o último ID e soma 1
                }
                fclose(fp);
            }

            // Define o novo ID para o fornecedor atual
            fornecedor->id = novo_id;

            fornecedor->id_logado = get_operador_logado(); // Associo o fornecedor ao operador logado
            // Garante que os campos string terminam com '\0'
            fornecedor->cnpj[sizeof(fornecedor->cnpj) - 1] = '\0';
            fornecedor->tel[sizeof(fornecedor->tel) - 1] = '\0';

            // Abre o arquivo binário em modo append binário ("ab")
            fp = fopen("fornecedores.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir fornecedores.bin");
                return NULL;
            }

            // Escreve o fornecedor no arquivo em formato binário
            if (fwrite(fornecedor, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
                perror("Erro ao gravar fornecedor em fornecedors.bin");
                fclose(fp);
                return NULL;
            }

            // Fecha o arquivo após gravar
            fclose(fp);

            printf("Fornecedor/Parceiro %s salvo em BINARIO!\n", fornecedor->nome_fantasia);

            // Retorna o ponteiro do fornecedor que foi salvo
            return fornecedor;
        }
    }

    // Caso o tipo de armazenamento nao seja reconhecido, retorno NULL
    return NULL;
}

Fornecedor_parceiro* atualizar_fornecedor_parceiro(const char* cnpj_busca, Fornecedor_parceiro* novos_dados, TipoArmazenamento tipo) {

    // Verifico se os parâmetros passados são válidos antes de qualquer coisa
    if (!cnpj_busca || !novos_dados) return NULL;

    // Pego o ID do operador que está logado no momento (para verificar permissões)
    int operador_atual = get_operador_logado();

    // Uso um switch para tratar de acordo com o tipo de armazenamento escolhido
    switch (tipo) {

        // =======================================================
        // CASO 1: Atualização de fornecedor armazenado na MEMÓRIA
        // =======================================================
        case MEMORIA: {
            // Percorro todos os fornecedores armazenados na memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ atual com o que foi informado pelo usuário
                if (strcmp(fornecedores_memoria[i].cnpj, cnpj_busca) == 0) {

                    // Verifico se o operador logado é o mesmo que criou esse fornecedor
                    if (fornecedores_memoria[i].id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar este fornecedor \n");
                        return NULL; // se nao for o mesmo, nao permito a atualização
                    }

                    // Guardo o id_logado original
                    int id_logado_original = fornecedores_memoria[i].id_logado;

                    // Substituo o fornecedor antigo pelos novos dados
                    fornecedores_memoria[i] = *novos_dados;

                    // Mas garanto que o CNPJ e id_logado originais nao sejam alterados
                    strcpy(fornecedores_memoria[i].cnpj, cnpj_busca);
                    fornecedores_memoria[i].id_logado = id_logado_original;

                    // Aviso que o fornecedor foi atualizado
                    printf("Fornecedor/Parceiro %s atualizado em MEMORIA!\n", fornecedores_memoria[i].nome_fantasia);

                    // Retorno o ponteiro para o fornecedor atualizado
                    return &fornecedores_memoria[i];
                }
            }

            // Se chegar aqui, é porque o CNPJ nao foi encontrado
           printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em MEMORIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            return NULL;
        }

        // =======================================================
        // CASO 2: Atualização de fornecedor armazenado em ARQUIVO TEXTO
        // =======================================================
        case TEXTO: {
            // Abro o arquivo original de fornecedores em modo leitura
            FILE* fp = fopen("fornecedores.txt", "r");
            if (!fp) {
                perror("Erro ao abrir fornecedores.txt");
                return NULL;
            }

            // Crio um arquivo temporário onde vou escrever as atualizações
            FILE* temp = fopen("fornecedores_tmp.txt", "w");
            if (!temp) {
                perror("Erro ao criar fornecedores_tmp.txt");
                fclose(fp);
                return NULL;
            }

            char linha[400]; // buffer para ler as linhas do arquivo
            int atualizado = 0; // flag para indicar se algum fornecedor foi atualizado

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                Fornecedor_parceiro f = {0};

                // Quebro a linha em campos separados por ';'
                 sscanf(linha, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];%d",
                                        &f.id, f.nome_fantasia, f.razao_social,
                                       f.endereco_completo, f.cnpj, 
                                        f.tel, f.tipo_servico, &f.id_logado);


                // Comparo o CNPJ lido com o buscado
                if (strcmp(f.cnpj, cnpj_busca) == 0) {

                    // Confiro se o operador logado tem permissão para atualizar esse fornecedor
                    if (f.id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar este fornecedor\n");
                        fclose(fp);
                        fclose(temp);
                        remove("fornecedores_tmp.txt"); // removo o arquivo temporário, já que deu erro
                        return NULL; // e saio direto
                    }

                    // Aqui sobrescrevo a linha do fornecedor com os novos dados
                    // mantendo id, CNPJ e id_logado originais
                    fprintf(temp, "%d;%s;%s;%s;%s;%s;%s;%d\n",
                            f.id, 
                            novos_dados->nome_fantasia,
                            novos_dados->razao_social,
                            novos_dados->endereco_completo,
                            f.cnpj,
                            novos_dados->tel,
                            novos_dados->tipo_servico,
                            f.id_logado
                        );
                        

                    atualizado = 1; // marco que o fornecedor foi atualizado
                } else {
                    // Se nao for o fornecedor buscado, só copio a linha normalmente
                    fputs(linha, temp);
                }
            }

            // Fecho os dois arquivos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo novo com as atualizações
            remove("fornecedores.txt");
            rename("fornecedores_tmp.txt", "fornecedores.txt");

            // Exibo mensagem de acordo com o resultado
            if (atualizado) {
                printf("Fornecedor/Parceiro %s atualizado em TEXTO!\n", novos_dados->nome_fantasia);
                return novos_dados;
            } else {
                printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
                return NULL;
            }
        }

        // =======================================================
        // CASO 3: Atualização de fornecedor armazenado em ARQUIVO BINÁRIO
        // =======================================================
        case BINARIO: {
            // Abro o arquivo binário para leitura e escrita
            FILE* fp = fopen("fornecedores.bin", "rb+");
            if (!fp) {
                perror("Erro ao abrir fornecedores.bin");
                return NULL;
            }

            Fornecedor_parceiro f;
            int encontrado = 0; // flag para marcar se encontrei o fornecedor

            // Leio fornecedor por fornecedor no arquivo
            while (fread(&f, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                // Garante que o CPF/CNPJ está terminado corretamente com '\0'
                f.cnpj[sizeof(f.cnpj) - 1] = '\0';

                // Comparo o CPF/CNPJ lido com o buscado
                if (strcmp(f.cnpj, cnpj_busca) == 0) {
                    encontrado = 1;

                    // Verifico se o operador logado é o mesmo que criou o fornecedor
                    if (f.id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar este fornecedor\n");
                        fclose(fp);
                        return NULL; // retorno imediatamente, sem exibir "nao encontrado"
                    }

                    // Volto o cursor do arquivo para reescrever esse mesmo fornecedor
                    fseek(fp, -(long)sizeof(Fornecedor_parceiro), SEEK_CUR);

                    // Copio os dados fixos que nao podem ser alterados
                    novos_dados->id = f.id;
                    strcpy(novos_dados->cnpj, f.cnpj);
                    novos_dados->id_logado = f.id_logado;

                    // Gravo os novos dados no arquivo (sobrescrevendo o fornecedor antigo)
                    if (fwrite(novos_dados, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
                        perror("Erro ao escrever fornecedor atualizado em fornecedores.bin");
                        fclose(fp);
                        return NULL;
                    }

                    // Fecho o arquivo e informo o sucesso
                    fclose(fp);
                    printf("Fornecedor/Parceiro %s atualizado em BINARIO!\n", novos_dados->nome_fantasia);
                    return novos_dados;
                }
            }

            // Fecho o arquivo caso o fornecedor nao tenha sido encontrado
            fclose(fp);

            // Se nao encontrei o fornecedor, aviso o usuário
            if (!encontrado) {
                 printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em BINARIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            }

            return NULL;
        }

        // =======================================================
        // Caso padrão — tipo de armazenamento inválido
        // =======================================================
        default:
            printf("Tipo de armazenamento invalido!\n");
            return NULL;
    }
}

