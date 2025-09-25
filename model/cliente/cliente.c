#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

#define MAX_CLIENTES 100
static Cliente clientes_memoria[MAX_CLIENTES]; // cada posição guarda um cliente
static int qtd = 0; // contador de quantos clientes já estão salvos na memória

// Função que cria e salva um cliente de acordo com o tipo escolhido
Cliente* criar_cliente(Cliente* cliente, TipoArmazenamento tipo) {
    if (!cliente) return NULL; // se o ponteiro for nulo, não posso salvar

    switch (tipo) {

        // Caso 1: salvar na memória
        case MEMORIA: {
            if (qtd < MAX_CLIENTES) {
                // copio todos os dados do cliente passado para o array de memória
                clientes_memoria[qtd] = *cliente;

                // crio um ponteiro que aponta para o cliente que acabei de salvar
                Cliente* salvo = &clientes_memoria[qtd];

                qtd++; // aumento o contador de clientes na memória

                printf("Cliente %s salvo em MEMÓRIA!\n", salvo->nome);
                return salvo; // retorno o endereço do cliente salvo
            } else {
                printf("Erro: limite de clientes na memória atingido!\n");
                return NULL;
            }
        }

        // Caso 2: salvar em arquivo texto
        case TEXTO: {
            // abro o arquivo em modo append para não sobrescrever os clientes anteriores
            FILE* fp = fopen("clientes.txt", "a");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return NULL;
            }

            // escrevo os dados do cliente no arquivo separados por ponto e vírgula
            fprintf(fp, "%d;%s;%d;%s;%s;%s;%s;%s\n",
                cliente->id,
                cliente->nome,
                cliente->idade,
                cliente->endereco_completo,
                cliente->cpf_cnpj,
                cliente->tel,
                cliente->email,
                cliente->nome_contato
            );

            fclose(fp); // fecho o arquivo
            printf("Cliente %s salvo em TEXTO!\n", cliente->nome);
            return cliente; // retorno o cliente que foi salvo
        }

        // Caso 3: salvar em arquivo binário
        case BINARIO: {
            // abro o arquivo em modo append binário
            FILE* fp = fopen("clientes.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return NULL;
            }

            // gravo a struct inteira diretamente no arquivo
            fwrite(cliente, sizeof(Cliente), 1, fp);

            fclose(fp); // fecho o arquivo
            printf("Cliente %s salvo em BINÁRIO!\n", cliente->nome);
            return cliente; // retorno o cliente que foi salvo
        }

        // Caso inválido
        default:
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
    }
}
// Atualiza os dados do cliente
void atualizar_cliente(Cliente* cliente, 
                       const char* nome, 
                       int idade, 
                       const char* endereco_completo, 
                       const char* cpf_cnpj, 
                       const char* tel, 
                       const char* email, 
                       const char* nome_contato) {
    // Verifica se o ponteiro cliente é válido
    if (!cliente) return;

    // Atualiza o nome do cliente com strncpy para evitar overflow
    strncpy(cliente->nome, nome, sizeof(cliente->nome) - 1);
    cliente->nome[sizeof(cliente->nome) - 1] = '\0'; // garante terminação nula

    // Atualiza a idade
    cliente->idade = idade;

    // Atualiza o endereço completo com segurança
    strncpy(cliente->endereco_completo, endereco_completo, sizeof(cliente->endereco_completo) - 1);
    cliente->endereco_completo[sizeof(cliente->endereco_completo) - 1] = '\0';

    // Atualiza o CPF/CNPJ
    strncpy(cliente->cpf_cnpj, cpf_cnpj, sizeof(cliente->cpf_cnpj) - 1);
    cliente->cpf_cnpj[sizeof(cliente->cpf_cnpj) - 1] = '\0';

    // Atualiza o telefone
    strncpy(cliente->tel, tel, sizeof(cliente->tel) - 1);
    cliente->tel[sizeof(cliente->tel) - 1] = '\0';

    // Atualiza o e-mail
    strncpy(cliente->email, email, sizeof(cliente->email) - 1);
    cliente->email[sizeof(cliente->email) - 1] = '\0';

    // Atualiza o nome do contato
    strncpy(cliente->nome_contato, nome_contato, sizeof(cliente->nome_contato) - 1);
    cliente->nome_contato[sizeof(cliente->nome_contato) - 1] = '\0';
}

// Remove o cliente (libera memória)
void deletar_cliente(Cliente* cliente) {
    if (cliente) free(cliente);
}
