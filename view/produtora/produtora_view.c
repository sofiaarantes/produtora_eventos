#include <stdio.h>
#include "produtora/produtora_view.h"
#include "../../util/util.h"
#include "../../view/main/main_view.h"

int menu_produtora() {
    int opcao;
    printf("\n==== MENU PRODUTORA ====\n");
    printf("1 - Criar produtora\n");
    printf("2 - Atualizar produtora\n");
    printf("3 - Exibir produtora\n");
    printf("4 - Deletar produtora\n");
    printf("0 - Sair\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

void ver_produtora(const Produtora* produtora) {
    if (!produtora) {
        printf("+--------------------------+\n");
        printf("| Produtora inexistente!   |\n");
        printf("+--------------------------+\n");
        return;
    }
    printf("+---------------------------------------------------------------------+\n");
    printf("| Nome Fantasia : %s\n", produtora->nome_fantasia);
    printf("| Razao Social : %s\n", produtora->razao_social);
    printf("| Inscricao Estadual : %c%c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c\n", produtora->inscricao_estadual[0], produtora->inscricao_estadual[1], produtora->inscricao_estadual[2],
       produtora->inscricao_estadual[3], produtora->inscricao_estadual[4], produtora->inscricao_estadual[5],
       produtora->inscricao_estadual[6], produtora->inscricao_estadual[7], produtora->inscricao_estadual[8],
       produtora->inscricao_estadual[9], produtora->inscricao_estadual[10], produtora->inscricao_estadual[11], produtora->inscricao_estadual[12],
       produtora->inscricao_estadual[13]);
    printf("| Telefone: (%c%c) %c%c%c%c%c-%c%c%c%c\n",
       produtora->tel[0], produtora->tel[1],    // DDD
       produtora->tel[2], produtora->tel[3], produtora->tel[4], produtora->tel[5], produtora->tel[6], // primeiros 5
       produtora->tel[7], produtora->tel[8], produtora->tel[9], produtora->tel[10]); // últimos 4
    printf("| Endereco : %s\n", produtora->endereco_completo);
    printf("| CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c\n", produtora->cnpj[0], produtora->cnpj[1], produtora->cnpj[2],
                                        produtora->cnpj[3], produtora->cnpj[4], produtora->cnpj[5],
                                        produtora->cnpj[6], produtora->cnpj[7], produtora->cnpj[8],
                                        produtora->cnpj[9], produtora->cnpj[10], produtora->cnpj[11], produtora->cnpj[12],
                                        produtora->cnpj[13], produtora->cnpj[14]);
    //usando %c (em vez de %s) porque estou imprimindo cada caractere individualmente do CNPJ, e não a string completa.

    printf("| Email : %s\n", produtora->email);
    printf("| Nome do Responsavel : %s\n", produtora->nome_resp);
    printf("| Telefone do Responsavel: (%c%c) %c%c%c%c%c-%c%c%c%c\n",
       produtora->tel_resp[0], produtora->tel_resp[1],    // DDD
       produtora->tel_resp[2], produtora->tel_resp[3], produtora->tel_resp[4], produtora->tel_resp[5], produtora->tel_resp[6], // primeiros 5
       produtora->tel_resp[7], produtora->tel_resp[8], produtora->tel_resp[9], produtora->tel_resp[10]); 
    printf("| Margem de lucro padrao : %.2f\n", produtora->lucro);
   printf("+---------------------------------------------------------------------+\n");
}

Produtora ler_dados_produtora() {
    Produtora p;
    ler_string("Nome Fantasia: ", p.nome_fantasia, sizeof(p.nome_fantasia));
    ler_string("Razao Social: ", p.razao_social, sizeof(p.razao_social));
    ler_string("Inscricao Estadual: ", p.inscricao_estadual, sizeof(p.inscricao_estadual));
    ler_string("Endereco completo: ", p.endereco_completo, sizeof(p.endereco_completo));
    ler_string("CNPJ (atencao ao inserir esse dado,ele nao podera ser alterado depois): ", p.cnpj, sizeof(p.cnpj));
    ler_string("Telefone: ", p.tel, sizeof(p.tel));
    ler_string("Email: ", p.email, sizeof(p.email));
    ler_string("Nome do responsavel: ", p.nome_resp, sizeof(p.nome_resp));
    ler_string("Telefone do responsavel: ", p.tel_resp, sizeof(p.tel_resp));
    ler_float("Margem de lucro padrao (apenas número, ex: 15.5): ", &p.lucro);
    return p;  // retorna a struct preenchida
}
void ler_dados_atualizados_produtora(
    char* nome_fantasia, size_t tam_nome_fantasia,
    char* razao_social, size_t tam_razao_social,
    char* inscricao_estadual, size_t tam_inscricao_estadual,
    char* endereco_completo, size_t tam_endereco,
    char* tel, size_t tam_tel,
    char* email, size_t tam_email,
    char* nome_resp, size_t tam_nome_resp,
    char* tel_resp, size_t tam_tel_resp,
    float* lucro)
    //passar tambem o tamanho dos arrays para a funçao ler_stringsaber ate onde pode ler

{
    ler_string("Novo nome fantasia: ", nome_fantasia, tam_nome_fantasia);
    ler_string("Nova razao social: ", razao_social, tam_razao_social);
    ler_string("Nova inscricao estadual: ", inscricao_estadual, tam_inscricao_estadual);
    ler_string("Novo endereco completo: ", endereco_completo, tam_endereco);
    ler_string("Novo telefone: ", tel, tam_tel);
    ler_string("Novo email: ", email, tam_email);
    ler_string("Novo nome do responsavel: ", nome_resp, tam_nome_resp);
    ler_string("Novo telefone do responsavel: ", tel_resp, tam_tel_resp);
    ler_float("Nova margem de lucro padrao (apenas numero, ex: 15.5): ", lucro);
}
    //passar tambem o tamanho dos arrays para a funçao ler_stringsaber ate onde pode ler

