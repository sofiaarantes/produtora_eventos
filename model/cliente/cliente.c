#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

// Cria e inicializa um cliente
   
Cliente* criar_cliente(int id, 
                        const char* nome, 
                        int idade,
                        const char* endereco_completo,
                        const char* cpf_cnpj,
                        const char* tel,
                        const char* email,
                        const char*nome_contato) {
                            
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
//strncpy não garante que a string copiada será terminada com \0 se o tamanho da string de origem for igual ou maior que o tamanho do destino.
// Sem o \0, funções que manipulam strings (como printf) podem ler além do fim do array e causar comportamento inesperado ou crash.

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
