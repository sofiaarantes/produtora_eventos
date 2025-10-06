
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

void deletar_cliente(Cliente* cliente);
void consultar_cliente(Cliente* cliente);

// Retorna o número de clientes na memória
int get_qtd_clientes();

// Busca cliente por CPF/CNPJ na memória (retorna ponteiro para o cliente ou NULL se não achar)
Cliente* buscar_cliente_por_cpf_memoria(const char* cpf_cnpj_busca);

// Busca cliente por CPF/CNPJ em arquivo texto (retorna ponteiro para o cliente ou NULL se não achar)
Cliente* buscar_cliente_por_cpf_texto(const char* cpf_cnpj_busca);

// Busca cliente por CPF/CNPJ em arquivo binário (retorna ponteiro para o cliente ou NULL se não achar)
Cliente* buscar_cliente_por_cpf_binario(const char* cpf_cnpj_busca);

// Função que atualiza um cliente de acordo com o tipo escolhido
// recebe o CPF/CNPJ do cliente que quero atualizar e o tipo de armazenamento
Cliente* atualizar_cliente(const char* cpf_cnpj_busca, Cliente* novos_dados, TipoArmazenamento tipo);
#endif
