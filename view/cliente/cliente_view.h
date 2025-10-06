#ifndef CLIENTE_VIEW_H
#define CLIENTE_VIEW_H

#include "../model/cliente/cliente.h"

// Mostra dados de um cliente
void ver_cliente(const Cliente* cliente);

// Exibe o menu principal e retorna a opção escolhida
int exibir_menu();

// Lê os dados de um cliente e RETORNA um Cliente preenchido
Cliente ler_dados_cliente();

// Pega dados atualizados de cliente (mantemos por referência para comparar)
void ler_dados_atualizados_cliente(
    char* nome, size_t tam_nome,
    int* idade,
    char* endereco_completo, size_t tam_endereco,
    char* tel, size_t tam_tel,
    char* email, size_t tam_email,
    char* nome_contato, size_t tam_nome_contato
    //passar tambem o tamanho dos arrays para a funçao ler_string para saber ate onde pode ler
);


#endif
