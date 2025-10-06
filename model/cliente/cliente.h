
#ifndef CLIENTE_H
#define CLIENTE_H
#include "../model/config_armazenamento/config_armazenamento.h"
// =============================
// MODEL: representa os dados
// =============================

// Estrutura de Cliente
typedef struct {
    int id;
    char nome[50];
    int idade;
    char endereco_completo[100];
    char cpf_cnpj[12];
    char tel[12];
    char email[50];
    char nome_contato[50];
   
} Cliente;

// Funções do modelo (CRUD)
// Cria um novo cliente e retorna um ponteiro para ele,recebe o tipo armazenamento e a struct cliente a ser adicionada
Cliente* criar_cliente(Cliente* cliente,TipoArmazenamento tipo);

// Função para exibir os dados de um cliente buscados pelo cpf/cnpj
void buscar_e_exibir_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo);

// Retorna o número de clientes na memória
int get_qtd_clientes();



// Função que atualiza um cliente de acordo com o tipo escolhido
// recebe o CPF/CNPJ do cliente que quero atualizar e o tipo de armazenamento
Cliente* atualizar_cliente(const char* cpf_cnpj_busca, Cliente* novos_dados, TipoArmazenamento tipo);
#endif
