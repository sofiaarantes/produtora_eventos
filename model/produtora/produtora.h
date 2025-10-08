#ifndef PRODUTORA_H
#define PRODUTORA_H
#include "../model/config_armazenamento/config_armazenamento.h"

//Estrtura do produtora
typedef struct{
    char nome_fantasia[50];
    char razao_social[50];
    char inscricao_estadual[20]; //13 dígitos + '\0'
    char cnpj[15];  // 14 dígitos + '\0'
    char endereco_completo[100];
    char tel[12];
    char email[50];
    char nome_resp[50];
    char tel_resp[12];
    float lucro;
}Produtora;

// Função que cria e salva uma produtora de acordo com o tipo escolhido
Produtora* criar_produtora(Produtora* produtora, TipoArmazenamento tipo);
#endif