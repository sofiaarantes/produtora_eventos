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

        // Caso 2: salvar em arquivo texto
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

