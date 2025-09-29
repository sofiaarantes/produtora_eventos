
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
    char cpf_cnpj[20];
    char tel[11];
    char email[50];
    char nome_contato[50];
   
} Cliente;

// Funções do modelo (CRUD)
// Cria um novo cliente e retorna um ponteiro para ele,recebe o tipo armazenamento e a struct cliente a ser adicionada
Cliente* criar_cliente(Cliente* cliente,TipoArmazenamento tipo);
void atualizar_cliente(Cliente* cliente, const char* nome, int idade,const char* endereco_completo,const char*
     cpf_cnpj,const char* tel,const char* email,const char*nome_contato,TipoArmazenamento);
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

//Função para atualizar cliente em binario
void atualizar_cliente_binario(Cliente* cliente_bin, 
                               const char* nome, int idade,
                               const char* endereco_completo, 
                               const char* tel,
                               const char* email, 
                               const char* nome_contato);

//Função para atualizar cliente em texto
void atualizar_cliente_texto(Cliente* cliente_txt, 
                             const char* nome, int idade,
                             const char* endereco_completo, 
                             const char* tel,
                             const char* email, 
                             const char* nome_contato);

//Função para atualizar cliente em memória
void atualizar_cliente_memoria(Cliente* cliente_mem, 
                               const char* nome, int idade,
                               const char* endereco_completo, 
                               const char* tel,
                               const char* email, 
                               const char* nome_contato);
#endif
