#ifndef CLIENTE_H
#define CLIENTE_H

// =============================
// MODEL: representa os dados
// =============================

// Estrutura básica de Cliente
typedef struct {
    int id;
    char nome[50];
    int idade;
    // Aqui você pode expandir: adicionar CPF, email, endereço, telefone, etc.
} Cliente;

// Funções do modelo (CRUD)
Cliente* criar_cliente(int id, const char* nome, int idade);
void atualizar_cliente(Cliente* cliente, const char* nome, int idade);
void deletar_cliente(Cliente* cliente);

#endif
