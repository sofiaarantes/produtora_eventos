#ifndef FORNECEDOR_PARCEIRO_VIEW_H
#define FORNECEDOR_PARCEIRO_VIEW_H

#include "../../model/fornecedor/fornecedor_parceiro.h"

//menu do crud de fornecedor/parceiro
int menu_fornecedor();

// Mostra dados de um fornecedor/parceiro 
void ver_fornecedor_parceiro(const Fornecedor_parceiro* fornecedor_parceiro);

// Lê os dados de um fornecedor/parceiro e RETORNA um Fornecedor_parceiro preenchido
Fornecedor_parceiro ler_dados_fornecedor_parceiro();

// Pega dados atualizados de fornecedor/parceiro 
void ler_dados_atualizados_fornecedor_parceiro(
    char* nome_fantasia,
    char* razao_social,
    char* endereco_completo,
    char* tel,
    char* tipo_servico,
    float* valor
    
);

#endif // FORNECEDOR_PARCEIRO_VIEW_H