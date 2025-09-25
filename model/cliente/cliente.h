
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
void atualizar_cliente(Cliente* cliente, const char* nome, int idade,const char* endereco_completo,const char* cpf_cnpj,const char* tel,const char* email,const char*nome_contato);
void deletar_cliente(Cliente* cliente);
void consultar_cliente(Cliente* cliente);

#endif
