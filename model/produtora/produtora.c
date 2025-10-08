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


        
    }
}    
