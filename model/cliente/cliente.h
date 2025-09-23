#ifndef CLIENTE_H
#define CLIENTE_H

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
    char tel[15];
    char email[50];
    char nome_contato[50];
   
} Cliente;

// Funções do modelo (CRUD)
Cliente* criar_cliente(int id, const char* nome, int idade,const char* endereco_completo,const char* cpf_cnpj,const char* tel,const char* email,const char*nome_contato);
void atualizar_cliente(Cliente* cliente, const char* nome, int idade,const char* endereco_completo,const char* cpf_cnpj,const char* tel,const char* email,const char*nome_contato);
void deletar_cliente(Cliente* cliente);
void consultar_cliente(Cliente* cliente);

#endif
