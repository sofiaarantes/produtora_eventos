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

                printf("Fornecedor/Parceiro %s salvo em MEMORIA! Tipo de documento: %d\n",fornecedor->nome_fantasia,fornecedor->id_logado );

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

            printf("Fornecedor/Parceiro %s salvo em TEXTO! id logado: %d\n", fornecedor->nome_fantasia,fornecedor->id_logado);

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

            printf("Cliente %s salvo em BINARIO! id logado: %d\n", fornecedor->nome_fantasia,fornecedor->id_logado);

            // Retorna o ponteiro do fornecedor que foi salvo
            return fornecedor;
        }
    }

    // Caso o tipo de armazenamento nao seja reconhecido, retorno NULL
    return NULL;
}
// Função que cria e salva um fornecedor/parceiro de acordo com o tipo escolhido
// Fornecedor_parceiro* criar_fornecedor_parceiro(Fornecedor_parceiro* fornecedor_parceiro, TipoArmazenamento tipo) {
//     if (!fornecedor_parceiro) return NULL; // se o ponteiro for nulo, não posso salvar

//     switch (tipo) {

//                 // Caso 1: salvar na memória
//                 case MEMORIA: {
//                     if (qtd < MAX_FORNECEDORES) {
//                         // Se houver dados na memória, ele continua com o novo_id = 1, caso contrário, resgatará o
//                         // último id e somará mais 1
//                         if (qtd > 0) 
//                             novo_id = fornecedores_memoria[qtd - 1].id + 1;

//                         fornecedor_parceiro->id = novo_id;
//                         // copio todos os dados do fornecedor/parceiro passado para o array de memória
//                         fornecedores_memoria[qtd] = *fornecedor_parceiro;

//                         // crio um ponteiro que aponta para o fornecedor/parceiro que acabei de salvar
//                         Fornecedor_parceiro* salvo = &fornecedores_memoria[qtd];

//                         qtd++; // aumento o contador de fornecedores/parceiros na memória

//                         printf("Fornecedor/Parceiro %s salvo em MEMÓRIA!\n", salvo->nome_fantasia);
//                         return salvo; // retorno o endereço do fornecedor/parceiro salvo
//                     } else {
//                         printf("Erro: limite de fornecedores/parceiros na memória atingido!\n");
//                         return NULL;
//                     }
//                 }

            
//             case TEXTO: {
//                 // Descobre o último id no arquivo
//                 FILE* fp = fopen("fornecedores.txt", "r");
                // if (fp) {
                //     Fornecedor_parceiro tmp;
                //     while (fscanf(fp, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];\n",
                //                   &tmp.id, tmp.nome_fantasia, tmp.razao_social, tmp.cnpj,
                //                   tmp.endereco_completo,
                //                   tmp.tel, tmp.tipo_servico) != EOF) {
                //         novo_id = tmp.id + 1; // sempre fica com o último id lido +1
                //     }
                //     fclose(fp);
                // }
                // fornecedor_parceiro->id = novo_id;

//                 // Abro o arquivo em modo append para não sobrescrever os fornecedores/parceiros anteriores
//                  fp = fopen("fornecedores.txt", "a");
//                 if (!fp) {
//                     perror("Erro ao abrir fornecedores.txt");
//                     return NULL;
//                 }

//                 // Escrevo os dados do fornecedor/parceiro no arquivo separados por ponto e vírgula
//                 // Uso o limite máximo de caracteres baseado no tamanho do array da struct
//                 // Isso garante que o campo não ultrapasse o espaço definido
//                 fprintf(fp, "%d;%.49s;%.49s;%.99s;%.14s;%.11s;%.49s\n",
//                     fornecedor_parceiro->id,
//                     fornecedor_parceiro->nome_fantasia,
//                     fornecedor_parceiro->razao_social,
//                     fornecedor_parceiro->endereco_completo,
//                     fornecedor_parceiro->cnpj,
//                     fornecedor_parceiro->tel,
//                     fornecedor_parceiro->tipo_servico
//                 );

//                 fclose(fp); // Fecho o arquivo
//                 printf("Fornecedor/Parceiro %s salvo em TEXTO!\n", fornecedor_parceiro->nome_fantasia);
//                 return fornecedor_parceiro; // Retorno o fornecedor/parceiro que foi salvo
//             }
//             // Caso 3: salvar fornecedor/parceiro em arquivo binário
//                 case BINARIO: {
//                      // Descobre último id no arquivo
//                         FILE* fp = fopen("fornecedores.bin", "rb");
//                         if (fp) {
//                             Fornecedor_parceiro tmp;
//                             while (fread(&tmp, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
//                                 novo_id = tmp.id + 1;
//                             }
//                             fclose(fp);
//                         }
//                         fornecedor_parceiro->id = novo_id;

//                     // garante que CNPJ e telefone terminam com '\0'
//                     fornecedor_parceiro->cnpj[sizeof(fornecedor_parceiro->cnpj) - 1] = '\0';
//                     fornecedor_parceiro->tel[sizeof(fornecedor_parceiro->tel) - 1] = '\0';

//                     // abre o arquivo em modo append binário
//                      fp = fopen("fornecedores.bin", "ab");
//                     if (!fp) {
//                         perror("Erro ao abrir fornecedores.bin");
//                         return NULL;
//                     }

//                     // grava a struct inteira no arquivo
//                     if (fwrite(fornecedor_parceiro, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
//                         perror("Erro ao gravar fornecedor/parceiro em fornecedores.bin");
//                         fclose(fp);
//                         return NULL;
//                     }

//                     fclose(fp);

//                     printf("Fornecedor/Parceiro %s salvo em BINÁRIO!\n", fornecedor_parceiro->nome_fantasia);
//                     return fornecedor_parceiro; // Retorno o fornecedor/parceiro que foi salvo
//                 }

    
     
//     }
//     return NULL; // Caso inválido
// } 


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
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13]);
                                return &fornecedores_memoria[i];
                            }
                           
                        }
                        printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrado em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13]);
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
                                            if (fseek(fp, -(long)sizeof(Fornecedor_parceiro), SEEK_CUR) != 0) {
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
                                            cnpj_busca[13]);

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
                                        cnpj_busca[13]);
                                    return NULL;
                                 }

                }
                return NULL; // Caso inválido
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

            // Função para deletar um fornecedor/parceiro a partir do CNPJ e do tipo de armazenamento
        void deletar_fornecedor_parceiro(const char* cnpj_busca, TipoArmazenamento tipo) {
                // Verifico se o parâmetro cnpj_busca é nulo antes de continuar
                if (!cnpj_busca) {
                    printf("CNPJ inválido!\n");
                    return;
                }

                // Uso o switch para tratar cada tipo de armazenamento (memória, texto e binário)
                switch (tipo) {

                    // =====================================================
                    // Caso 1 — Deletar fornecedor/parceiro armazenado em MEMÓRIA
                    // =====================================================
                    case MEMORIA: {
                        int encontrado = 0; // variável para controlar se o fornecedor/parceiro foi achado

                        // Percorro o vetor global de fornecedores/parceiros armazenados em memória
                        for (int i = 0; i < qtd; i++) {
                            // Comparo o CNPJ do fornecedor/parceiro atual com o CNPJ digitado
                            if (strncmp(fornecedores_memoria[i].cnpj, cnpj_busca, sizeof(fornecedores_memoria[i].cnpj)) == 0) {

                                // Quando encontro o fornecedor/parceiro, preciso removê-lo deslocando os próximos para trás
                                for (int j = i; j < qtd - 1; j++) {
                                    fornecedores_memoria[j] = fornecedores_memoria[j + 1];
                                }

                                // Reduzo a quantidade total de fornecedores/parceiros armazenados
                                qtd--;

                                // Marco que encontrei o fornecedor/parceiro
                                encontrado = 1;

                                printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletado em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                cnpj_busca[13]);
                                break; // saio do laço, já deletei
                            }
                        }

                        // Caso o fornecedor/parceiro não tenha sido encontrado, aviso o usuário
                        if (!encontrado)
                            printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrado em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                cnpj_busca[13]);

                        break;
                    }

                    // =====================================================
                    // Caso 2 — Deletar fornecedor/parceiro de ARQUIVO TEXTO
                    // =====================================================
                    case TEXTO: {
                        // Abro o arquivo de fornecedores em modo leitura
                        FILE* fp = fopen("fornecedores.txt", "r");
                        if (!fp) {
                            perror("Erro ao abrir fornecedores.txt");
                            return;
                        }

                        // Crio um arquivo temporário para regravar os dados válidos
                        FILE* temp = fopen("fp_temp.txt", "w");
                        if (!temp) {
                            perror("Erro ao criar fp_temp.txt");
                            fclose(fp);
                            return;
                        }

                        char linha[512];   // buffer para ler linha por linha do arquivo
                        int deletado = 0;  // controle se o fornecedor/parceiro foi removido
                        Fornecedor_parceiro fornecedor;   // struct temporária para armazenar os dados lidos

                        // Leio o arquivo original linha por linha
                        while (fgets(linha, sizeof(linha), fp)) {
                            // Uso sscanf para quebrar os dados lidos separados por ';'
                            sscanf(linha, "%d;%49[^;];%49[^;];%99[^;];%14[^;];%11[^;];%49[^\n]",
                                            &fornecedor.id,
                                            fornecedor.nome_fantasia,
                                            fornecedor.razao_social,
                                            fornecedor.endereco_completo,
                                            fornecedor.cnpj,
                                            fornecedor.tel,
                                            fornecedor.tipo_servico
                            );

                            // Se o CNPJ não for o que quero excluir, regravo essa linha no arquivo temporário
                            if (strncmp(fornecedor.cnpj, cnpj_busca, sizeof(fornecedor.cnpj)) != 0) {
                                fprintf(temp, "%d;%s;%s;%s;%s;%s;%s\n",
                                    fornecedor.id,
                                    fornecedor.nome_fantasia,
                                    fornecedor.razao_social,
                                    fornecedor.endereco_completo,
                                    fornecedor.cnpj,
                                    fornecedor.tel,
                                    fornecedor.tipo_servico
                                );
                            } else {
                                // Caso o CNPJ seja o que quero deletar, apenas marco a exclusão
                                deletado = 1;
                            }
                        }

                        // Fecho os arquivos abertos
                        fclose(fp);
                        fclose(temp);

                        // Excluo o arquivo antigo e renomeio o temporário como o novo "fornecedores.txt"
                        remove("fornecedores.txt");
                        rename("fp_temp.txt", "fornecedores.txt");

                        // Exibo mensagem informando o resultado da operação
                        if (deletado)
                            printf("Fornecedor com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%cdeletado em ARQUIVO TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13]);
                        else
                            printf("Fornecedor com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%cnão encontrado em ARQUIVO TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13]);

                        break;
                    }

                        // =====================================================
                        // Caso 3 — Deletar fornecedor/parceiro de ARQUIVO BINÁRIO
                        // =====================================================
                        case BINARIO: {
                            // Abro o arquivo binário original em modo leitura binária
                            FILE* fp = fopen("fornecedores.bin", "rb");
                            if (!fp) {
                                perror("Erro ao abrir fornecedores.bin");
                                return;
                            }

                            // Crio um arquivo temporário para regravar os fornecedores/parceiros que continuarão existindo
                            FILE* temp = fopen("fp_temp.bin", "wb");
                            if (!temp) {
                                perror("Erro ao criar fp_temp.bin");
                                fclose(fp);
                                return;
                            }

                            Fornecedor_parceiro fornecedor;   // struct temporária para armazenar o fornecedor lido
                            int deletado = 0;  // controle se algum fornecedor foi realmente removido

                            // Leio cada fornecedor do arquivo binário original
                            while (fread(&fornecedor, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                                // Garanto que o CNPJ termina com '\0' para evitar erro de comparação
                                fornecedor.cnpj[sizeof(fornecedor.cnpj) - 1] = '\0';

                                // Se o CNPJ não for o que deve ser deletado, gravo esse fornecedor no arquivo temporário
                                if (strncmp(fornecedor.cnpj, cnpj_busca, sizeof(fornecedor.cnpj)) != 0) {
                                    fwrite(&fornecedor, sizeof(Fornecedor_parceiro), 1, temp);
                                } else {
                                    // Caso o CNPJ seja o do fornecedor/parceiro a excluir, apenas marco que deletei
                                    deletado = 1;
                                }
                            }

                            // Fecho os arquivos abertos
                            fclose(fp);
                            fclose(temp);

                            // Substituo o arquivo original pelo temporário atualizado
                            remove("fornecedores.bin");
                            rename("fp_temp.bin", "fornecedores.bin");

                            // Exibo mensagens de status ao usuário
                            if (deletado)
                                printf("Fornecedor com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%cdeletado em ARQUIVO BINARIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                    cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                    cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                    cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13]);
                            else
                            printf("Fornecedor com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%cnão encontrado em ARQUIVO BINARIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                    cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                    cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                    cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12], cnpj_busca[13]);

                            break;
                        }

                        // =====================================================
                        // Caso padrão — tipo de armazenamento inválido
                        // =====================================================
                        default:
                            printf("Tipo de armazenamento inválido!\n");
                            break;
                    }
                }
