#ifndef PRODUTORA_H
#define PRODUTORA_H
#include "../model/config_armazenamento/config_armazenamento.h"

// Estrutura do produtora
typedef struct{
    char nome_fantasia[50];
    char razao_social[50];
    char inscricao_estadual[13]; //13 dígitos + '\0'
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

// Função que atualiza um produtora de acordo com o tipo escolhido
// recebe o CPF/CNPJ do produtora que quero atualizar e o tipo de armazenamento
Produtora* atualizar_produtora(const char* cnpj_busca, Produtora* novos_dados, TipoArmazenamento tipo);

// Função para buscar e exibir diretamente uma produtora pelo CNPJ e tipo de armazenamento
void buscar_e_exibir_produtora(const char* cnpj_busca, TipoArmazenamento tipo);

//função que retorna a quantidade de produtoras salvas na memória
int get_qtd_produtoras();

// Função para deletar uma produtora a partir do CNPJ e do tipo de armazenamento
void deletar_produtora(const char* cnpj_busca, TipoArmazenamento tipo);

// Migrar dados

// Lista todos os registros do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
Produtora* listar_todos_produtoras(TipoArmazenamento tipo, int* out_count);

// Remove todos os registros do armazenamento especificado. Retorna 1 em sucesso.
int limpar_produtoras(TipoArmazenamento tipo);

#endif