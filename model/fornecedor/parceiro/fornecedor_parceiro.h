#ifndef FORNECEDOR_CLIENTE_H
#define FORNECEDOR_CLIENTE_H

//Estrtura do fornecedor/parceiro
typedef struct{
    int id;
    char nome_fantasia[50];
    char razao_social[50];
    char cpf_cnpj[20];
    char endereco_completo[100];
    char tel[15];
    char tipo_servico[50];
}Fornecedor_parceiro;

#endif
