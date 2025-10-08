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
    char* nome_fantasia, size_t tam_nome_fantasia,
    char* razao_social, size_t tam_razao_social,
    char* inscricao_estadual, size_t tam_inscricao_estadual,
    char* endereco_completo, size_t tam_endereco,
    char* tel, size_t tam_tel,
    char* email, size_t tam_email,
    char* nome_resp, size_t tam_nome_resp,
    char* tel_resp, size_t tam_tel_resp,
    float* lucro
    //passar tambem o tamanho dos arrays para a funçao ler_stringsaber ate onde pode ler
);
#endif