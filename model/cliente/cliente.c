#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

// Cria e inicializa um cliente
   
Cliente* criar_cliente(int id, const char* nome, int idade,const char* endereco_completo,const char* cpf_cnpj,const char* tel,const char* email,const char*nome_contato) {
    Cliente* novo = (Cliente*) malloc(sizeof(Cliente));
    if (!novo) {
        return NULL; // erro de memória
    }
    // Inicializa os campos do cliente
    novo->id = id;  // ID do cliente
    strncpy(novo->nome, nome, sizeof(novo->nome)); // Copia o nome, garantindo que não ultrapasse o tamanho
    novo->nome[sizeof(novo->nome) - 1] = '\0'; // Garante terminação nula
    novo->idade = idade; // Idade do cliente
    strncpy(novo->endereco_completo, endereco_completo, sizeof(novo->endereco_completo));
    novo->endereco_completo[sizeof(novo->endereco_completo) - 1] = '\0';
    strncpy(novo->cpf_cnpj, cpf_cnpj, sizeof(novo->cpf_cnpj));
    novo->cpf_cnpj[sizeof(novo->cpf_cnpj) - 1] = '\0';
    strncpy(novo->tel, tel, sizeof(novo->tel));
    novo->tel[sizeof(novo->tel) - 1] = '\0';
    strncpy(novo->email, email, sizeof(novo->email));
    novo->email[sizeof(novo->email) - 1] = '\0';
    strncpy(novo->nome_contato, nome_contato, sizeof(novo->nome_contato));
    novo->nome_contato[sizeof(novo->nome_contato) - 1] = '\0';

    return novo;
}

// Atualiza os dados do cliente
void atualizar_cliente(Cliente* cliente, const char* nome, int idade,const char* endereco_completo,const char* cpf_cnpj,const char* tel,const char* email,const char*nome_contato) {
    if (!cliente) return;
    strncpy(cliente->nome, nome, sizeof(cliente->nome));
    cliente->idade = idade;
}

// Remove o cliente (libera memória)
void deletar_cliente(Cliente* cliente) {
    if (cliente) free(cliente);
}
