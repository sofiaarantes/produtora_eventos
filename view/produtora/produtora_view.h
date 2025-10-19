#ifndef PRODUTORAVIEW_H
#define PRODUTORAVIEW_H

#include "../../model/produtora/produtora.h"

//menu do crud de produtora
int menu_produtora();

// Mostra dados de uma produtora 
void ver_produtora(const Produtora* produtora);

// Lê os dados de uma produtora e RETORNA uma Produtora preenchida
Produtora ler_dados_produtora();

// Pega dados atualizados de produtora
void ler_dados_atualizados_produtora(
    char* nome_fantasia,    
    char* razao_social,
    char* inscricao_estadual,
    char* endereco_completo,
    char* tel,
    char* email,
    char* nome_resp,
    char* tel_resp,
    float* lucro
        
);
#endif