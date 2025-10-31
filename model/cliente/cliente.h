
#ifndef CLIENTE_H
#define CLIENTE_H
#include "../model/config_armazenamento/config_armazenamento.h"
// =============================
// MODEL: representa os dados
// =============================
// Enum para definir o tipo de documento
typedef enum {
    TIPO_CPF, //vale 0
    TIPO_CNPJ, //vale 1
    TIPO_DESCONHECIDO //vale 2
} TipoDocumento;

// Estrutura de Cliente
typedef struct {
    int id;
    char nome[50];
    int idade;
    char endereco_completo[100];
    char cpf_cnpj[20]; //14 digitos para cnpj ou 11 digitso para cpf + '\0'
    char tel[12];
    char email[50];
    char nome_contato[50];
    TipoDocumento tipo_doc;
    int id_logado; // ID do operador a que o cliente pertence
} Cliente;

// Funções do modelo (CRUD)
// Cria um novo cliente e retorna um ponteiro para ele,recebe o tipo armazenamento e a struct cliente a ser adicionada
Cliente* criar_cliente(Cliente* cliente,TipoArmazenamento tipo);

// Função para exibir os dados de um cliente buscados pelo cpf/cnpj
void buscar_e_exibir_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo);


// Função que atualiza um cliente de acordo com o tipo escolhido
// recebe o CPF/CNPJ do cliente que quero atualizar e o tipo de armazenamento
Cliente* atualizar_cliente(const char* cpf_cnpj_busca, Cliente* novos_dados, TipoArmazenamento tipo);

// Remove o cliente (libera memória) de acordo com o cpf/cnpj e tipo de armazenamento
void deletar_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo);

//preciso dessa funçao para identificar se o documento do cliente é cpf ou cnpj
TipoDocumento identificar_documento(const char *doc);

//função que lista os clientes de acordo com o tipo de armazenamento
void listar_clientes(TipoArmazenamento tipo);
#endif
