#ifndef CLIENTE_VIEW_H
#define CLIENTE_VIEW_H

#include "../model/cliente/cliente.h"

// Mostra dados de um cliente
void exibir_cliente(const Cliente* cliente);

// Exibe o menu principal e retorna a opção escolhida
int exibir_menu();

// Lê os dados de um cliente e RETORNA um Cliente preenchido
Cliente ler_dados_cliente();

// Pega dados atualizados de cliente (mantemos por referência para comparar)
void ler_dados_atualizados_cliente(
    char* nome,              
    int* idade,              
    char* endereco_completo, 
    char* cpf_cnpj,          
    char* tel,               
    char* email,             
    char* nome_contato      
);


#endif
