#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor_parceiro.h"
#include "../../view/fornecedor/fornecedor_parceiro_view.h"
#include "../../view/main/main_view.h"


#define MAX_FORNECEDORES 100
static Fornecedor_parceiro fornecedores_memoria[MAX_FORNECEDORES]; // cada posição guarda um fornecedor/parceiro
static int qtd = 0; // contador de quantos fornecedores/parceiros já estão salvos na memória

// Função que cria e salva um fornecedor/parceiro de acordo com o tipo escolhido
Fornecedor_parceiro* criar_fornecedor_parceiro(Fornecedor_parceiro* fornecedor_parceiro, TipoArmazenamento tipo) {
    if (!fornecedor_parceiro) return NULL; // se o ponteiro for nulo, não posso salvar

    switch (tipo) {

                // Caso 1: salvar na memória
                case MEMORIA: {
                    if (qtd < MAX_FORNECEDORES) {
                        // copio todos os dados do fornecedor/parceiro passado para o array de memória
                        fornecedores_memoria[qtd] = *fornecedor_parceiro;

                        // crio um ponteiro que aponta para o fornecedor/parceiro que acabei de salvar
                        Fornecedor_parceiro* salvo = &fornecedores_memoria[qtd];

                        qtd++; // aumento o contador de fornecedores/parceiros na memória

                        printf("Fornecedor/Parceiro %s salvo em MEMÓRIA!\n", salvo->nome_fantasia);
                        return salvo; // retorno o endereço do fornecedor/parceiro salvo
                    } else {
                        printf("Erro: limite de fornecedores/parceiros na memória atingido!\n");
                        return NULL;
                    }
                }

            
            case TEXTO: {
                // Abro o arquivo em modo append para não sobrescrever os fornecedores/parceiros anteriores
                FILE* fp = fopen("fornecedores.txt", "a");
                if (!fp) {
                    perror("Erro ao abrir fornecedores.txt");
                    return NULL;
                }

                // Escrevo os dados do fornecedor/parceiro no arquivo separados por ponto e vírgula
                // Uso o limite máximo de caracteres baseado no tamanho do array da struct
                // Isso garante que o campo não ultrapasse o espaço definido
                fprintf(fp, "%d;%.49s;%.49s;%.99s;%.14s;%.11s;%.49s\n",
                    fornecedor_parceiro->id,
                    fornecedor_parceiro->nome_fantasia,
                    fornecedor_parceiro->razao_social,
                    fornecedor_parceiro->endereco_completo,
                    fornecedor_parceiro->cnpj,
                    fornecedor_parceiro->tel,
                    fornecedor_parceiro->tipo_servico
                );

                fclose(fp); // Fecho o arquivo
                printf("Fornecedor/Parceiro %s salvo em TEXTO!\n", fornecedor_parceiro->nome_fantasia);
                return fornecedor_parceiro; // Retorno o fornecedor/parceiro que foi salvo
            }
            // Caso 3: salvar fornecedor/parceiro em arquivo binário
                case BINARIO: {
                    // garante que CNPJ e telefone terminam com '\0'
                    fornecedor_parceiro->cnpj[sizeof(fornecedor_parceiro->cnpj) - 1] = '\0';
                    fornecedor_parceiro->tel[sizeof(fornecedor_parceiro->tel) - 1] = '\0';

                    // abre o arquivo em modo append binário
                    FILE* fp = fopen("fornecedores.bin", "ab");
                    if (!fp) {
                        perror("Erro ao abrir fornecedores.bin");
                        return NULL;
                    }

                    // grava a struct inteira no arquivo
                    if (fwrite(fornecedor_parceiro, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
                        perror("Erro ao gravar fornecedor/parceiro em fornecedores.bin");
                        fclose(fp);
                        return NULL;
                    }

                    fclose(fp);

                    printf("Fornecedor/Parceiro %s salvo em BINÁRIO!\n", fornecedor_parceiro->nome_fantasia);
                    return fornecedor_parceiro; // Retorno o fornecedor/parceiro que foi salvo
                }

    
     
    }
}

// Função que atualiza um fornecedor/parceiro de acordo com o tipo escolhido
            // recebe o CNPJ do fornecedor/parceiro que quero atualizar e o tipo de armazenamento
            Fornecedor_parceiro* atualizar_fornecedor_parceiro(const char* cnpj_busca, Fornecedor_parceiro* novos_dados, TipoArmazenamento tipo) {
                if (!cnpj_busca || !novos_dados) return NULL;

                switch (tipo) {

                    // Caso 1: atualizar na memória
                    case MEMORIA: {
                        int i;
                        for ( i = 0; i < qtd; i++) {
                            if (strcmp(fornecedores_memoria[i].cnpj, cnpj_busca) == 0) {
                                fornecedores_memoria[i] = *novos_dados; // sobrescreve os dados
                                strcpy(fornecedores_memoria[i].cnpj, cnpj_busca);
                                // garantia de não mudar o CNPJ

                                printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c atualizado em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13], cnpj_busca[14]);
                                return &fornecedores_memoria[i];
                            }
                           
                        }
                        printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrado em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13], cnpj_busca[14]);
                        return NULL;
                    }

                    // Caso 2: atualizar fornecedor/parceiro em arquivo texto 
                            case TEXTO: {
                                // Abro o arquivo original em modo leitura
                                FILE* fp = fopen("fornecedores.txt", "r");
                                if (!fp) {
                                    perror("Erro ao abrir fornecedores.txt");
                                    return NULL;
                                }

                                // Crio um arquivo temporário onde vamos salvar os dados atualizados
                                FILE* temp = fopen("fornecedores_tmp.txt", "w");
                                if (!temp) {
                                    perror("Erro ao criar fornecedores_tmp.txt");
                                    fclose(fp);
                                    return NULL;
                                }

                                char linha[400];     // buffer para ler cada linha do arquivo original
                                int atualizado = 0;  // flag que indica se encontramos o fornecedor/parceiro e atualizamos

                                // Percorro cada linha do arquivo original
                                while (fgets(linha, sizeof(linha), fp)) {
                                    Fornecedor_parceiro f = {0}; // zera todos os campos antes de ler

                                    // Leitura dos campos na MESMA ORDEM usada no inserir
                                    sscanf(linha, "%d;%49[^;];%49[^;];%99[^;];%14[^;];%11[^;];%49[^\n]",
                                        &f.id, f.nome_fantasia, f.razao_social,
                                        f.endereco_completo, f.cnpj, f.tel, f.tipo_servico);

                                    // Se o CNPJ lido for igual ao CNPJ que queremos atualizar
                                    if (strcmp(f.cnpj, cnpj_busca) == 0) {
                                        // Gravo os novos dados no arquivo temporário, mantendo o mesmo CNPJ e o mesmo id
                                        fprintf(temp, "%d;%s;%s;%s;%s;%s;%s\n",
                                            f.id, // id não muda
                                            novos_dados->nome_fantasia,
                                            novos_dados->razao_social,
                                            novos_dados->endereco_completo,
                                            f.cnpj, // CNPJ não muda
                                            novos_dados->tel,
                                            novos_dados->tipo_servico
                                        );
                                        atualizado = 1;
                                    } else {
                                        // Se não for o fornecedor/parceiro que queremos atualizar, apenas regravo a linha original
                                        fputs(linha, temp);
                                    }
                                }

                                // Fecho os arquivos
                                fclose(fp);
                                fclose(temp);

                                // Substituo o arquivo original pelo temporário
                                remove("fornecedores.txt");
                                rename("fornecedores_tmp.txt", "fornecedores.txt");

                                // Mensagem de sucesso ou falha
                                if (atualizado) {
                                    printf("Fornecedor/Parceiro com CNPJ %s atualizado em TEXTO!\n", cnpj_busca);
                                    return novos_dados;
                                } else {
                                    printf("Fornecedor/Parceiro com CNPJ %s não encontrado em TEXTO!\n", cnpj_busca);
                                    return NULL;
                                }
                            }

                            // Caso 3: atualizar fornecedor/parceiro em arquivo binário
                                case BINARIO: {
                                    // Abro o arquivo 'fornecedores.bin' em modo leitura + escrita binária
                                    FILE *fp = fopen("fornecedores.bin", "rb+");
                                    if (!fp) {
                                        // Se não conseguir abrir, mostro o erro do sistema e retorno NULL
                                        perror("Erro ao abrir fornecedores.bin");
                                        return NULL;
                                    }

                                    Fornecedor_parceiro fp_temp; // Estrutura temporária para ler os fornecedores/parceiros do arquivo

                                    // Percorro todo o arquivo fornecedor por fornecedor
                                    while (fread(&fp_temp, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                                        // Garante que o CNPJ lido do arquivo termina com '\0', evitando lixo
                                        fp_temp.cnpj[sizeof(fp_temp.cnpj)-1] = '\0';

                                        // Comparo o CNPJ lido com o CNPJ que queremos atualizar
                                        // Uso strncmp com o tamanho total do campo (14) para incluir o '\0'
                                        if (strncmp(fp_temp.cnpj, cnpj_busca, sizeof(fp_temp.cnpj)) == 0) {

                                            // Volto o ponteiro do arquivo para o início deste registro
                                            // para sobrescrever com os novos dados
                                            if (fseek(fp, -sizeof(Fornecedor_parceiro), SEEK_CUR) != 0) {
                                                perror("Erro no fseek durante atualização");
                                                fclose(fp);
                                                return NULL;
                                            }

                                            // Mantém o CNPJ original do fornecedor/parceiro, pois não pode ser alterado
                                            strncpy(novos_dados->cnpj, fp_temp.cnpj, sizeof(novos_dados->cnpj));
                                            novos_dados->cnpj[sizeof(novos_dados->cnpj)-1] = '\0';

                                            // Gravo a struct atualizada no arquivo no lugar do antigo registro
                                            if (fwrite(novos_dados, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
                                                perror("Erro ao sobrescrever fornecedor/parceiro em fornecedores.bin");
                                                fclose(fp);
                                                return NULL;
                                            }

                                            // Fecho o arquivo e informo que a atualização deu certo
                                            fclose(fp);
                                            printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c atualizado em BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                            cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                            cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                            cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                            cnpj_busca[13], cnpj_busca[14]);

                                            // Retorno o ponteiro para os novos dados
                                            return novos_dados;
                                        }
                                    }

                                    // Se chegar aqui, significa que não encontrei o CNPJ no arquivo
                                    fclose(fp);
                                    printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrado em BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                        cnpj_busca[13], cnpj_busca[14]);
                                    return NULL;
                                 }

                }
             }

             // Retorna quantos fornecedores/parceiros já estão salvos em memória
            // Aqui eu só quero saber a quantidade de fornecedores/parceiros que já cadastrei na memória
            int get_qtd_fornecedores() { 
                return qtd; // retorno o valor do contador, que tá sendo incrementado toda vez que salvo um fornecedor/parceiro
            }

            // Função para buscar e exibir diretamente um fornecedor/parceiro pelo CNPJ
            void buscar_e_exibir_fornecedor_parceiro(const char* cnpj_busca, TipoArmazenamento tipo) {
                // Verifico se o CNPJ passado é nulo (inválido)
                if (!cnpj_busca) {
                    printf("+--------------------------+\n");
                    printf("| CNPJ inválido!          |\n");
                    printf("+--------------------------+\n");
                    return; // se for inválido, saio da função
                }

                Fornecedor_parceiro* fornecedor_parceiro = NULL; // Ponteiro que vai armazenar o fornecedor/parceiro encontrado, se existir

                // Escolho o tipo de armazenamento usado
                switch (tipo) {

                    // =========================
                    // Caso 1: busca em memória
                    // =========================
                    case MEMORIA: {
                        // Percorro o array de fornecedores/parceiros em memória
                        for (int i = 0; i < qtd; i++) {
                            // Comparo o CNPJ de cada fornecedor/parceiro com o CNPJ digitado
                            if (strncmp(fornecedores_memoria[i].cnpj, cnpj_busca, sizeof(fornecedores_memoria[i].cnpj)) == 0) {
                                // Se encontrar, guardo o endereço do fornecedor/parceiro encontrado
                                fornecedor_parceiro = &fornecedores_memoria[i];
                                // Exibo as informações desse fornecedor/parceiro
                                ver_fornecedor_parceiro(fornecedor_parceiro);
                                break; // paro o loop, pois já encontrei o fornecedor/parceiro
                            }
                        }
                        break;
                    }

                    // ==============================
                    // Caso 2: busca em arquivo texto
                    // ==============================
                    case TEXTO: {
                        // Abro o arquivo texto para leitura
                        FILE* fp = fopen("fornecedores.txt", "r");
                        if (!fp) {
                            // Se não conseguir abrir, mostro o erro e saio
                            perror("Erro ao abrir fornecedores.txt");
                            return;
                        }

                        // Aloco memória para armazenar temporariamente um fornecedor/parceiro
                        fornecedor_parceiro = malloc(sizeof(Fornecedor_parceiro));
                        char linha[512]; // buffer para armazenar cada linha do arquivo

                        // Leio o arquivo linha por linha
                        while (fgets(linha, sizeof(linha), fp)) {
                            // Extraio os campos separados por ponto e vírgula
                            sscanf(linha, "%d;%49[^;];%49[^;];%99[^;];%14[^;];%11[^;];%49[^\n]",
                                &fornecedor_parceiro->id,
                                fornecedor_parceiro->nome_fantasia,
                                fornecedor_parceiro->razao_social,
                                fornecedor_parceiro->endereco_completo,
                                fornecedor_parceiro->cnpj,
                                fornecedor_parceiro->tel,
                                fornecedor_parceiro->tipo_servico
                            );
                            

                            // Comparo o CNPJ lido com o CNPJ buscado
                            if (strncmp(fornecedor_parceiro->cnpj, cnpj_busca, sizeof(fornecedor_parceiro->cnpj)) == 0) {
                                // Se encontrar, fecho o arquivo e exibo o fornecedor/parceiro
                                fclose(fp);
                                ver_fornecedor_parceiro(fornecedor_parceiro);
                                free(fornecedor_parceiro); // libero a memória alocada
                                return; // retorno, pois já encontrei o fornecedor/parceiro
                            }
                        }

                        // Se chegar aqui, o fornecedor/parceiro não foi encontrado
                        fclose(fp);   // fecho o arquivo
                        free(fornecedor_parceiro); // libero a memória usada
                        fornecedor_parceiro = NULL; // deixo o ponteiro nulo
                        break;
                    }

                    // =================================
                    // Caso 3: busca em arquivo binário
                    // =================================
                    case BINARIO: {
                        // Abro o arquivo binário para leitura
                        FILE* fp = fopen("fornecedores.bin", "rb");
                        if (!fp) {
                            // Se não conseguir abrir, mostro o erro e saio
                            perror("Erro ao abrir fornecedores.bin");
                            return;
                        }

                        // Aloco memória para armazenar temporariamente um fornecedor/parceiro
                        fornecedor_parceiro = malloc(sizeof(Fornecedor_parceiro));

                        // Leio o arquivo binário fornecedor/parceiro por fornecedor/parceiro
                        while (fread(fornecedor_parceiro, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                            // Garante que a string do CNPJ termina em '\0'
                            fornecedor_parceiro->cnpj[sizeof(fornecedor_parceiro->cnpj)-1] = '\0';

                            // Comparo o CNPJ lido com o CNPJ buscado
                            if (strncmp(fornecedor_parceiro->cnpj, cnpj_busca, sizeof(fornecedor_parceiro->cnpj)) == 0) {
                                // Se encontrar, fecho o arquivo e exibo as informações
                                fclose(fp);
                                ver_fornecedor_parceiro(fornecedor_parceiro);
                                free(fornecedor_parceiro); // libero a memória alocada
                                return; // retorno, pois já encontrei o fornecedor/parceiro
                            }
                        }

                        // Se terminar o arquivo e não encontrar, libero tudo
                        fclose(fp);
                        free(fornecedor_parceiro);
                        fornecedor_parceiro = NULL;
                        break;
                    }

                    // Caso o tipo de armazenamento não seja reconhecido
                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!\n");
                        return;
                }

                // Se nenhum fornecedor/parceiro foi encontrado nos casos acima, mostro mensagem padrão
                if (!fornecedor_parceiro) {
                    printf("+--------------------------+\n");
                    printf("| Fornecedor não encontrado!  |\n");
                    printf("+--------------------------+\n");
                }
            }