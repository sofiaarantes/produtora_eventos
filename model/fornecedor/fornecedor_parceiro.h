#ifndef FORNECEDOR_PARCEIRO_H
#define FORNECEDOR_PARCEIRO_H
#include "../model/config_armazenamento/config_armazenamento.h"

//Estrtura do fornecedor/parceiro
typedef struct{
    int id;
    char nome_fantasia[50];
    char razao_social[50];
    char cnpj[15];  // 14 dígitos + '\0'
    char endereco_completo[100];
    char tel[12];
    char tipo_servico[50];
    int id_logado;
    float valor;
}Fornecedor_parceiro;

// Função que cria e salva um fornecedor/parceiro de acordo com o tipo escolhido
Fornecedor_parceiro* criar_fornecedor_parceiro(Fornecedor_parceiro* fornecedor_parceiro, TipoArmazenamento tipo);

// Função que atualiza um fornecedor/parceiro de acordo com o tipo escolhido e o CNPJ passado pelo parametro
Fornecedor_parceiro* atualizar_fornecedor_parceiro(const char* cnpj_busca, Fornecedor_parceiro* novos_dados, TipoArmazenamento tipo);

// Função para buscar e exibir diretamente um fornecedor/parceiro pelo CNPJ e tipo de armazenamento
void buscar_e_exibir_fornecedor_parceiro(const char* cnpj_busca, TipoArmazenamento tipo);

//função que retorna a quantidade de fornecedores/parceiros cadastrados na memória
int get_qtd_fornecedores();

// Função para deletar um fornecedor/parceiro a partir do CNPJ e do tipo de armazenamento
void deletar_fornecedor_parceiro(const char* cnpj_busca, TipoArmazenamento tipo);

//funçao que lista os fornecedores/parceiros de acordo com o tipo de armazenamento 
void listar_fornecedores(TipoArmazenamento tipo);

//função que busca um fornecedor/parceiro pelo ID
Fornecedor_parceiro* buscar_fornecedor_por_id(TipoArmazenamento tipo, int id);
#endif
