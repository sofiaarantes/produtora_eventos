#include <stdio.h>
#include "fornecedor/fornecedor_parceiro_view.h"
#include "../../util/util.h"
#include "../../view/main/main_view.h"

int menu_fornecedor() {
    
     int opcao;
    printf("\n===================================================================\n");
    printf("|                   Menu Fornecedor/Parceiro                     |\n");
    printf("===================================================================\n");
    printf("|  1 - Adicionar novo fornecedor/parceiro                        |\n");
    printf("|  2 - Atualizar fornecedor/parceiro                               |\n");
    printf("|  3 - Exibir fornecedor/parceiro                                  |\n");
    printf("|  4 - Deletar fornecedor/parceiro                                 |\n");
    printf("|  0 - Voltar ao menu anterior                                    |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

void ver_fornecedor_parceiro(const Fornecedor_parceiro* fornecedor_parceiro) {
    if (!fornecedor_parceiro) {
        printf("+------------------------------------+\n");
        printf("| Fornecedor/Parceiro inexistente!     |\n");
        printf("+------------------------------------+\n");
        return;
    }
    printf("+---------------------------------------------------------------------+\n");
    printf("| ID   : %d\n", fornecedor_parceiro->id);
    printf("| Nome Fantasia : %s\n", fornecedor_parceiro->nome_fantasia);
    printf("| Razao Social : %s\n", fornecedor_parceiro->razao_social);
    printf("| Telefone: (%c%c) %c%c%c%c%c-%c%c%c%c\n",
       fornecedor_parceiro->tel[0], fornecedor_parceiro->tel[1],    // DDD
       fornecedor_parceiro->tel[2], fornecedor_parceiro->tel[3], fornecedor_parceiro->tel[4], fornecedor_parceiro->tel[5], fornecedor_parceiro->tel[6], // primeiros 5
       fornecedor_parceiro->tel[7], fornecedor_parceiro->tel[8], fornecedor_parceiro->tel[9], fornecedor_parceiro->tel[10]); // últimos 4
    printf("| Endereco : %s\n", fornecedor_parceiro->endereco_completo);
    printf("| CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c\n", fornecedor_parceiro->cnpj[0], fornecedor_parceiro->cnpj[1], fornecedor_parceiro->cnpj[2],
                                        fornecedor_parceiro->cnpj[3], fornecedor_parceiro->cnpj[4], fornecedor_parceiro->cnpj[5],
                                        fornecedor_parceiro->cnpj[6], fornecedor_parceiro->cnpj[7], fornecedor_parceiro->cnpj[8],
                                        fornecedor_parceiro->cnpj[9], fornecedor_parceiro->cnpj[10], fornecedor_parceiro->cnpj[11], fornecedor_parceiro->cnpj[12],
                                        fornecedor_parceiro->cnpj[13]);
        //usando %c (em vez de %s) porque estou imprimindo cada caractere individualmente do CNPJ, e não a string completa.
    printf("| Tipo de servico : %s\n", fornecedor_parceiro->tipo_servico);
    printf("+---------------------------------------------------------------------+\n");
}

Fornecedor_parceiro ler_dados_fornecedor_parceiro() {
    Fornecedor_parceiro fp;

   printf("+ --------------------------------------------------------------- +\n");
    printf("|                     Novo Fornecedor/Parceiro                   |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Nome Fantasia: ", fp.nome_fantasia, sizeof(fp.nome_fantasia));
    ler_string("|  Razao Social: ", fp.razao_social, sizeof(fp.razao_social));
    ler_string("|  Endereco completo: ", fp.endereco_completo, sizeof(fp.endereco_completo));
    ler_string("|  CNPJ (atencao ao inserir esse dado,ele nao podera ser alterado depois): ", fp.cnpj, sizeof(fp.cnpj));
    ler_string("|  Telefone: ", fp.tel, sizeof(fp.tel));
    ler_string("|  Tipo de servico: ", fp.tipo_servico, sizeof(fp.tipo_servico));
    printf("+ --------------------------------------------------------------- +\n");
    return fp;  // retorna a struct preenchida
}
void ler_dados_atualizados_fornecedor_parceiro(
    char* nome_fantasia,
    char* razao_social,
    char* endereco_completo,
    char* tel,
    char* tipo_servico
    //passar tambem o tamanho dos arrays para a funçao ler_stringsaber ate onde pode ler
) {
   printf("+ --------------------------------------------------------------- +\n");
    printf("|                  Atualizar Fornecedor/Parceiro                  |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Novo nome fantasia: ", nome_fantasia, sizeof(nome_fantasia));
    ler_string("|  Nova razao social: ", razao_social, sizeof(razao_social));
    ler_string("|  Novo endereco completo: ", endereco_completo, sizeof(endereco_completo));
    ler_string("|  Novo telefone: ", tel, sizeof(tel));
    ler_string("|  Novo tipo de servico: ", tipo_servico, sizeof(tipo_servico));
    printf("+ --------------------------------------------------------------- +\n");
}
    //passar tambem o tamanho dos arrays para a funçao ler_stringsaber ate onde pode ler

