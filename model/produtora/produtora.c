#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtora.h"
#include "../../view/produtora/produtora_view.h"
#include "../../view/main/main_view.h"


#define MAX_PRODUTORAS 100
static Produtora produtoras_memoria[MAX_PRODUTORAS]; // cada posição guarda uma produtora
static int qtd = 0; // contador de quantas produtoras já estão salvas na memória

// Função que cria e salva uma produtora de acordo com o tipo escolhido
Produtora* criar_produtora(Produtora* produtora, TipoArmazenamento tipo) {
    if (!produtora) return NULL; // se o ponteiro for nulo, não posso salvar

    switch (tipo) {

        // Caso 1: salvar na memória
        case MEMORIA: {
            if (qtd < MAX_PRODUTORAS) {
                // copio todos os dados da produtora passada para o array de memória
                produtoras_memoria[qtd] = *produtora;

                // crio um ponteiro que aponta para a produtora que acabei de salvar
                Produtora* salvo = &produtoras_memoria[qtd];

                qtd++; // aumento o contador de produtoras na memória

                printf("Produtora %s salva em MEMÓRIA!\n", salvo->nome_fantasia);
                return salvo; // retorno o endereço da produtora salva
            } else {
                printf("Erro: limite de produtoras na memória atingido!\n");
                return NULL;
            }
        }

        // Caso 2: salvar em arquivo texto
            case TEXTO: {
                // Abro o arquivo em modo append para não sobrescrever as produtoras anteriores
                FILE* fp = fopen("produtoras.txt", "a");
                if (!fp) {
                    perror("Erro ao abrir produtoras.txt");
                    return NULL;
                }

                // Escrevo os dados da produtora no arquivo separados por ponto e vírgula
                // Uso o limite máximo de caracteres baseado no tamanho do array da struct
                // Isso garante que o campo não ultrapasse o espaço definido
                fprintf(fp, "%.49s;%.49s;%.13s;%.14s;%.99s;%.11s;%.49s;%.49s;%.11s;%f\n",
                    produtora->nome_fantasia,
                    produtora->razao_social,
                    produtora->inscricao_estadual,
                    produtora->cnpj,
                    produtora->endereco_completo,
                    produtora->tel,
                    produtora->email,
                    produtora->nome_resp,
                    produtora->tel_resp,
                    produtora->lucro
                );

                fclose(fp); // Fecho o arquivo
                printf("Produtora %s salva em TEXTO!\n", produtora->nome_fantasia);
                return produtora; // Retorno a produtora que foi salva
            }
        // Caso 3: salvar produtora em arquivo binário
            case BINARIO: {
                // garante que CNPJ e telefone terminam com '\0'
                produtora->cnpj[sizeof(produtora->cnpj) - 1] = '\0';
                produtora->tel[sizeof(produtora->tel) - 1] = '\0';

                // abre o arquivo em modo append binário
                FILE* fp = fopen("produtoras.bin", "ab");
                if (!fp) {
                    perror("Erro ao abrir produtoras.bin");
                    return NULL;
                }

                // grava a struct inteira no arquivo
                if (fwrite(produtora, sizeof(Produtora), 1, fp) != 1) {
                    perror("Erro ao gravar produtora em produtoras.bin");
                    fclose(fp);
                    return NULL;
                }

                fclose(fp);

                printf("Produtora %s salva em BINÁRIO!\n", produtora->nome_fantasia);
                return produtora;
            }
        default: {
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
        }
    }
}    
            // Função que atualiza um produtora de acordo com o tipo escolhido
            // recebe o CPF/CNPJ do produtora que quero atualizar e o tipo de armazenamento
            Produtora* atualizar_produtora(const char* cnpj_busca, Produtora* novos_dados, TipoArmazenamento tipo) {
                if (!cnpj_busca || !novos_dados) return NULL;

                switch (tipo) {

                    // Caso 1: atualizar na memória
                    case MEMORIA: {
                        int i;
                        for ( i = 0; i < qtd; i++) {
                            if (strcmp(produtoras_memoria[i].cnpj, cnpj_busca) == 0) {
                                produtoras_memoria[i] = *novos_dados; // sobrescreve os dados
                                strcpy(produtoras_memoria[i].cnpj, cnpj_busca);
                                // garantia de não mudar o CNPJ

                                printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c atualizado em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                cnpj_busca[13], cnpj_busca[14]);
                                return &produtoras_memoria[i];
                            }
                           
                        }
                        printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13], cnpj_busca[14]);
                                            return NULL;
                    }

                    // Caso 2: atualizar produtora em arquivo texto
                            case TEXTO: {
                                // Abro o arquivo original em modo leitura
                                FILE* fp = fopen("produtoras.txt", "r");
                                if (!fp) {
                                    perror("Erro ao abrir produtoras.txt");
                                    return NULL; // não conseguimos abrir, retorna NULL
                                }

                                // Crio um arquivo temporário onde vamos salvar os dados atualizados
                                FILE* temp = fopen("produtoras_tmp.txt", "w");
                                if (!temp) {
                                    perror("Erro ao criar produtoras_tmp.txt");
                                    fclose(fp);
                                    return NULL; // falha ao criar temporário
                                }

                                char linha[300];         // buffer para ler cada linha do arquivo original
                                int atualizado = 0;      // flag que indica se encontramos o produtora e atualizamos

                                // Percorro cada linha do arquivo original
                                while (fgets(linha, sizeof(linha), fp)) {
                                    Produtora p = {0}; // zera todos os campos antes de ler
                                    // Leitura dos campos da linha, separados por ';'
                                    sscanf(linha, "%49[^;];%49[^;];%13[^;];%14[^;];%99[^;];%11[^\n];%49[^\n];%49[^\n];%11[^\n];%f",
                                         p.nome_fantasia, p.razao_social, p.inscricao_estadual,
                                        p.cnpj, p.endereco_completo,p.tel, p.email, p.nome_resp,p.tel_resp,p.lucro);

                                    // Removo possíveis '\n' ou '\r' de todos os campos lidos
                                    

                                    // Se o CNPJ lido for igual ao CNPJ que queremos atualizar
                                    if (strcmp(p.cnpj, cnpj_busca) == 0) {
                                        // Gravo os novos dados no arquivo temporário, mantendo o mesmo CNPJ
                                        fprintf(temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%f\n",
                                            novos_dados->nome_fantasia,
                                            novos_dados->razao_social,
                                            novos_dados->inscricao_estadual,
                                            p.cnpj,
                                            novos_dados->endereco_completo,
                                            novos_dados->tel,
                                            novos_dados->email,
                                            novos_dados->nome_resp,
                                            novos_dados->tel_resp,
                                            novos_dados->lucro
                                        );
                                        atualizado = 1; // marca que o produtora foi atualizado
                                    } else {
                                        // Se não for o produtora que queremos atualizar, apenas regravo a linha original
                                             fputs(linha, temp);
                                        
                                    
                                }

                                // Fecho os arquivos
                                fclose(fp);
                                fclose(temp);

                                // Substituo o arquivo original pelo temporário
                                remove("produtoras.txt");
                                rename("produtoras_tmp.txt", "produtoras.txt");

                                // Mensagem de sucesso ou falha
                                if (atualizado) {
                                   printf("Produtora com %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c atualizado em TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                    cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                    cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                    cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                    cnpj_busca[13], cnpj_busca[14]);
                                    return novos_dados; // retorna os novos dados do produtora
                                } else {
                                    printf("Produtora com %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrado em TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                    cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                    cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                    cnpj_busca[9], cnpj_busca[10],cnpj_busca[11], cnpj_busca[12],
                                    cnpj_busca[13], cnpj_busca[14]);
                                    return NULL; // produtora não encontrada
                                }
                            }
  
                            case BINARIO: {
                                // Abro o arquivo 'produtoras.bin' em modo leitura + escrita binária
                                FILE *fp = fopen("produtoras.bin", "rb+");
                                if (!fp) {
                                    // Se não conseguir abrir, mostro o erro do sistema e retorno NULL
                                    perror("Erro ao abrir produtoras.bin");
                                    return NULL;
                                }

                                Produtora p; // Estrutura temporária para ler as produtoras do arquivo

                                // Percorro todo o arquivo produtora por produtora
                                while (fread(&p, sizeof(Produtora), 1, fp) == 1) {
                                    // Garante que o CNPJ lido do arquivo termina com '\0', evitando lixo
                                    p.cnpj[sizeof(p.cnpj)-1] = '\0';

                                    // Comparo o CNPJ lido com o CNPJ que queremos atualizar
                                    // Uso strncmp com o tamanho total do campo (12) para incluir o '\0'
                                    if (strncmp(p.cnpj, cnpj_busca, sizeof(p.cnpj)) == 0) {

                                        // Volto o ponteiro do arquivo para o início deste registro
                                        // para sobrescrever com os novos dados
                                        if (fseek(fp, -(long)sizeof(Produtora), SEEK_CUR) != 0) {
                                            perror("Erro no fseek durante atualização");
                                            fclose(fp);
                                            return NULL;
                                        }

                                        // Mantém o CNPJ original da produtora, pois não pode ser alterado
                                        strncpy(novos_dados->cnpj, p.cnpj, sizeof(novos_dados->cnpj));
                                        novos_dados->cnpj[sizeof(novos_dados->cnpj)-1] = '\0';

                                        // Gravo a struct atualizada no arquivo no lugar do antigo registro
                                        if (fwrite(novos_dados, sizeof(Produtora), 1, fp) != 1) {
                                            perror("Erro ao sobrescrever produtora em produtoras.bin");
                                            fclose(fp);
                                            return NULL;
                                        }

                                        // Fecho o arquivo e informo que a atualização deu certo
                                        fclose(fp);
                                        printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c atualizado em BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                        cnpj_busca[9], cnpj_busca[10],cnpj_busca[11], cnpj_busca[12],
                                        cnpj_busca[13], cnpj_busca[14]);

                                        // Retorno o ponteiro para os novos dados
                                        return novos_dados;
                                    }
                                }

                                // Se chegar aqui, significa que não encontrei o CNPJ no arquivo
                                fclose(fp);
                                printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                                    cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                                    cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                                    cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                                    cnpj_busca[13]);
                                return NULL;
                            }

        }
        default: {
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
        }
    }
}
//funçao para buscar e exibir diretamente uma produtora pelo CNPJ e tipo de armazenamento
void buscar_e_exibir_produtora(const char* cnpj_busca, TipoArmazenamento tipo) {
    // Verifico se o CNPJ passado é nulo (inválido)
    if (!cnpj_busca) {
        exibir_mensagem("+--------------------------+\n");
        exibir_mensagem("| CNPJ inválido!       |\n");
        exibir_mensagem("+--------------------------+\n");
        return; // se for inválido, saio da função
    }

    Produtora* produtora = NULL; // Ponteiro que vai armazenar a produtora encontrada, se existir

    // Escolho o tipo de armazenamento usado
    switch (tipo) {

        // =========================
        // Caso 1: busca em memória
        // =========================
        case MEMORIA: {
            // Percorro o array de produtoras em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ de cada produtora com o CNPJ digitado
                if (strncmp(produtoras_memoria[i].cnpj, cnpj_busca, sizeof(produtoras_memoria[i].cnpj)) == 0) {
                    // Se encontrar, guardo o endereço da produtora encontrada
                    produtora = &produtoras_memoria[i];
                    // Exibo as informações dessa produtora
                    ver_produtora(produtora);
                    break; // paro o loop, pois já encontrei a produtora
                }
            }
            break;
        }

        // ==============================
        // Caso 2: busca em arquivo texto
        // ==============================
        case TEXTO: {
            // Abro o arquivo texto para leitura
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) {
                // Se não conseguir abrir, mostro o erro e saio
                perror("Erro ao abrir produtoras.txt");
                return;
            }

            // Aloco memória para armazenar temporariamente uma produtora
            produtora = malloc(sizeof(Produtora));
            char linha[512]; // buffer para armazenar cada linha do arquivo

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Extraio os campos separados por ponto e vírgula
                sscanf(linha, "%49[^;];%99[^;];%13[^;];%14[^;];%99[^;];%11[^;];%49[^;];%49[^;];%11[^;];%f",
                    produtora->nome_fantasia,
                    produtora->razao_social,
                    produtora->inscricao_estadual,
                    produtora->cnpj,
                    produtora->endereco_completo,
                    produtora->tel,
                    produtora->email,
                    produtora->nome_resp,
                    produtora->tel_resp,
                    &produtora->lucro
                );

                // Comparo o CNPJ lido com o CNPJ buscado
                if (strncmp(produtora->cnpj, cnpj_busca, sizeof(produtora->cnpj)) == 0) {
                    // Se encontrar, fecho o arquivo e exibo a produtora
                    fclose(fp);
                    ver_produtora(produtora);
                    free(produtora); // libero a memória alocada
                    return; // retorno, pois já encontrei a produtora
                }
            }

            // Se chegar aqui, a produtora não foi encontrada
            fclose(fp);   // fecho o arquivo
            free(produtora); // libero a memória usada
            produtora = NULL; // deixo o ponteiro nulo
            break;
        }

        // =================================
        // Caso 3: busca em arquivo binário
        // =================================
        case BINARIO: {
            // Abro o arquivo binário para leitura
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) {
                // Se não conseguir abrir, mostro o erro e saio
                perror("Erro ao abrir produtoras.bin");
                return;
            }

            // Aloco memória para armazenar temporariamente uma produtora
            produtora = malloc(sizeof(Produtora));

            // Leio o arquivo binário produtora por produtora
            while (fread(produtora, sizeof(Produtora), 1, fp) == 1) {
                // Garante que a string do CNPJ termina em '\0'
                produtora->cnpj[sizeof(produtora->cnpj)-1] = '\0';

                // Comparo o CNPJ lido com o CNPJ buscado
                if (strncmp(produtora->cnpj, cnpj_busca, sizeof(produtora->cnpj)) == 0) {
                    // Se encontrar, fecho o arquivo e exibo as informações
                    fclose(fp);
                    ver_produtora(produtora);
                    free(produtora); // libero a memória alocada
                    return; // retorno, pois já encontrei a produtora
                }
            }

            // Se terminar o arquivo e não encontrar, libero tudo
            fclose(fp);
            free(produtora);
            produtora = NULL;
            break;
        }

        // Caso o tipo de armazenamento não seja reconhecido
        default:
            exibir_mensagem("Tipo de armazenamento inválido!\n");
            return;
    }

    // Se nenhum produtora foi encontrado nos casos acima, mostro mensagem padrão
    if (!produtora) {
        printf("+--------------------------+\n");
        printf("| Produtora não encontrada!  |\n");
        printf("+--------------------------+\n");
    }
}

//função que retorna a quantidade de produtoras salvas na memória
int get_qtd_produtoras() {
    return qtd; // retorno o valor do contador, que tá sendo incrementado toda vez que salvo uma produtora
}

// Função para deletar uma produtora a partir do CNPJ e do tipo de armazenamento
void deletar_produtora(const char* cnpj_busca, TipoArmazenamento tipo) {
    // Verifico se o parâmetro cnpj_busca é nulo antes de continuar
    if (!cnpj_busca) {
        printf("CNPJ inválido!\n");
        return;
    }

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto e binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar produtora armazenada em MEMÓRIA
        // =====================================================
        case MEMORIA: {
            int encontrado = 0; // variável para controlar se o produtora foi achado

            // Percorro o vetor global de produtoras armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ do produtora atual com o CNPJ digitado
                if (strncmp(produtoras_memoria[i].cnpj, cnpj_busca, sizeof(produtoras_memoria[i].cnpj)) == 0) {

                    // Quando encontro o produtora, preciso removê-la deslocando os próximos para trás
                    for (int j = i; j < qtd - 1; j++) {
                        produtoras_memoria[j] = produtoras_memoria[j + 1];
                    }

                    // Reduzo a quantidade total de produtoras armazenados
                    qtd--;

                    // Marco que encontrei o produtora
                    encontrado = 1;

                    printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletada em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);
                    break; // saio do laço, já deletei
                }
            }

            // Caso o produtora não tenha sido encontrado, aviso o usuário
            if (!encontrado)
                 printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);

            break;
        }

        // =====================================================
        // Caso 2 — Deletar produtora de ARQUIVO TEXTO
        // =====================================================
        case TEXTO: {
            // Abro o arquivo de produtoras em modo leitura
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) {
                perror("Erro ao abrir produtoras.txt");
                return;
            }

            // Crio um arquivo temporário para regravar os dados válidos
            FILE* temp = fopen("temp.txt", "w");
            if (!temp) {
                perror("Erro ao criar temp.txt");
                fclose(fp);
                return;
            }

            char linha[512];   // buffer para ler linha por linha do arquivo
            int deletado = 0;  // controle se o produtora foi removido
            Produtora produtora;   // struct temporária para armazenar os dados lidos

            // Leio o arquivo original linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Uso sscanf para quebrar os dados lidos separados por ';'
                sscanf(linha, "%49[^;];%99[^;];%13[^;];%14[^;];%99[^;];%11[^;];%49[^;];%49[^;];%11[^;];%f",
                    produtora.nome_fantasia,
                    produtora.razao_social,
                    produtora.inscricao_estadual,
                    produtora.cnpj,
                    produtora.endereco_completo,
                    produtora.tel,
                    produtora.email,
                    produtora.nome_resp,
                    produtora.tel_resp,
                    &produtora.lucro
                );

                // Se o CNPJ não for o que quero excluir, regravo essa linha no arquivo temporário
                if (strncmp(produtora.cnpj, cnpj_busca, sizeof(produtora.cnpj)) != 0) {
                    fprintf(temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%f\n",
                        produtora.nome_fantasia,
                        produtora.razao_social,
                        produtora.inscricao_estadual,
                        produtora.cnpj,
                        produtora.endereco_completo,
                        produtora.tel,
                        produtora.email,
                        produtora.nome_resp,
                        produtora.tel_resp,
                        &produtora.lucro
                    );
                } else {
                    // Caso o CNPJ seja o que quero deletar, apenas marco a exclusão
                    deletado = 1;
                }
            }


            // Fecho os arquivos abertos
            fclose(fp);
            fclose(temp);

            // Excluo o arquivo antigo e renomeio o temporário como o novo "produtoras.txt"
            remove("produtoras.txt");
            rename("temp.txt", "produtoras.txt");

            // Exibo mensagem informando o resultado da operação
            if (deletado)
                printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletada em ARQUIVO TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);
            else
                printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada no ARQUIVO TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);

            break;
        }

        // =====================================================
        // Caso 3 — Deletar cliente de ARQUIVO BINÁRIO
        // =====================================================
        case BINARIO: {
            // Abro o arquivo binário original em modo leitura binária
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return;
            }

            // Crio um arquivo temporário para regravar os produtoras que continuarão existindo
            FILE* temp = fopen("temp.bin", "wb");
            if (!temp) {
                perror("Erro ao criar temp.bin");
                fclose(fp);
                return;
            }

            Produtora produtora;   // struct temporária para armazenar a produtora lida
            int deletado = 0;  // controle se alguma produtora foi realmente removida

            // Leio cada produtora do arquivo binário original
            while (fread(&produtora, sizeof(Produtora), 1, fp) == 1) {
                // Garanto que o CNPJ termina com '\0' para evitar erro de comparação
                produtora.cnpj[sizeof(produtora.cnpj) - 1] = '\0';

                // Se o CNPJ não for o que deve ser deletado, gravo essa produtora no arquivo temporário
                if (strncmp(produtora.cnpj, cnpj_busca, sizeof(produtora.cnpj)) != 0) {
                    fwrite(&produtora, sizeof(Produtora), 1, temp);
                } else {
                    // Caso o CNPJ seja o da produtora a excluir, apenas marco que deletei
                    deletado = 1;
                }
            }

            // Fecho os arquivos abertos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo temporário atualizado
            remove("produtoras.bin");
            rename("temp.bin", "produtoras.bin");

            // Exibo mensagens de status ao usuário
            if (deletado)
                 printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletada em ARQUIVO BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);
            else
                printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada no ARQUIVO BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);

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