#ifndef FORNECEDOR_PARCEIRO_VIEW_H
#define FORNECEDOR_PARCEIRO_VIEW_H

#include "../../model/fornecedor/fornecedor_parceiro.h"

//menu do crud de fornecedor/parceiro
int menu_fornecedor();

// Mostra dados de um fornecedor/parceiro de acordo com o cpnj passado e o tipo de armazenamento
void ver_fornecedor_parceiro(const Fornecedor_parceiro* fornecedor_parceiro);

// Lê os dados de um fornecedor/parceiro e RETORNA um Fornecedor_parceiro preenchido
Fornecedor_parceiro ler_dados_fornecedor_parceiro();

// Pega dados atualizados de fornecedor/parceiro 
void ler_dados_atualizados_fornecedor_parceiro(
    char* nome_fantasia, size_t tam_nome_fantasia,
    char* razao_social, size_t tam_razao_social,
    char* endereco_completo, size_t tam_endereco,
    char* tel, size_t tam_tel,
    char* tipo_servico, size_t tam_tipo_servico
    //passar tambem o tamanho dos arrays para a funçao ler_stringsaber ate onde pode ler
);

#endif // FORNECEDOR_PARCEIRO_VIEW_H