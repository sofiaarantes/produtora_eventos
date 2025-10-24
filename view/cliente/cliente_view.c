#include <stdio.h>
#include "cliente/cliente_view.h"
#include "../../util/util.h"
#include "../../view/main/main_view.h"
#include "../../model/cliente/cliente.h"



void ver_cliente(const Cliente* cliente) {
    
    if (!cliente) {
        printf("+--------------------------+\n");
        printf("| Cliente inexistente!     |\n");
        printf("+--------------------------+\n");
        return;
    }
    printf("+---------------------------------------------------------------------+\n");
    printf("| ID   : %d\n", cliente->id);
    printf("| Nome : %s\n", cliente->nome);
    printf("| Idade: %d\n", cliente->idade);
    printf("| Telefone: (%c%c) %c%c%c%c%c-%c%c%c%c\n",
       cliente->tel[0], cliente->tel[1],    // DDD
       cliente->tel[2], cliente->tel[3], cliente->tel[4], cliente->tel[5], cliente->tel[6], // primeiros 5
       cliente->tel[7], cliente->tel[8], cliente->tel[9], cliente->tel[10]); // últimos 4   
    printf("| Endereco : %s\n", cliente->endereco_completo);
    if (cliente->tipo_doc == TIPO_CPF) {
         printf("| CPF: %c%c%c.%c%c%c.%c%c%c-%c%c\n",
       cliente->cpf_cnpj[0], cliente->cpf_cnpj[1], cliente->cpf_cnpj[2],
       cliente->cpf_cnpj[3], cliente->cpf_cnpj[4], cliente->cpf_cnpj[5],
       cliente->cpf_cnpj[6], cliente->cpf_cnpj[7], cliente->cpf_cnpj[8],
       cliente->cpf_cnpj[9], cliente->cpf_cnpj[10]);
    } else if (cliente->tipo_doc == TIPO_CNPJ) {
        printf("| CNPJ: %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c\n",
               cliente->cpf_cnpj[0], cliente->cpf_cnpj[1], cliente->cpf_cnpj[2],
               cliente->cpf_cnpj[3], cliente->cpf_cnpj[4], cliente->cpf_cnpj[5],
               cliente->cpf_cnpj[6], cliente->cpf_cnpj[7], cliente->cpf_cnpj[8],
               cliente->cpf_cnpj[9], cliente->cpf_cnpj[10], cliente->cpf_cnpj[11],
               cliente->cpf_cnpj[12], cliente->cpf_cnpj[13]);
    } else {
        printf("| Tipo de documento: DESCONHECIDO\n");
    }
   
    
    printf("| Email : %s\n", cliente->email);
    printf("| Nome do contato: %s\n", cliente->nome_contato);
    const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                                        (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";
    printf("| Tipo de documento: %s\n", tipoTexto);
    printf("+---------------------------------------------------------------------+\n");
}

int menu_cliente() {
    int opcao;
    printf("\n==================================================================\n");
    printf("|                   Menu Cliente                                  |\n");
    printf("===================================================================\n");
    printf("|  1 - Adicionar novo cliente                                     |\n");
    printf("|  2 - Atualizar cliente                                          |\n");
    printf("|  3 - Exibir cliente                                             |\n");
    printf("|  4 - Deletar cliente                                            |\n");
    printf("|  0 - Voltar ao menu anterior                                    |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

Cliente ler_dados_cliente() {
    Cliente c;
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                     Novo Cliente                                |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Nome: ", c.nome, sizeof(c.nome));
    ler_int("|  Idade: ", &c.idade);
    ler_string("|  Endereco completo: ", c.endereco_completo, sizeof(c.endereco_completo));
    ler_string("|  CPF/CNPJ (atencao ao inserir esse dado,ele nao podera ser alterado depois): ", c.cpf_cnpj, sizeof(c.cpf_cnpj));
    ler_string("|  Telefone: ", c.tel, sizeof(c.tel));
    ler_string("|  Email: ", c.email, sizeof(c.email));
    ler_string("|  Nome do contato: ", c.nome_contato, sizeof(c.nome_contato));
    printf("+ --------------------------------------------------------------- +\n");
    
    return c;  // retorna a struct preenchida
}

void ler_dados_atualizados_cliente(
    char* nome,
    int* idade,
    char* endereco_completo,
    char* tel,
    char* email,
    char* nome_contato
    //passar tambem o tamanho dos arrays para a funçao ler_string saber ate onde pode ler
) {
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                  Atualizar Cliente                               |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Novo nome: ", nome, 50);
    ler_int("|  Nova idade: ", idade);
    ler_string("|  Novo endereco completo: ", endereco_completo, 100);
    ler_string("|  Novo telefone: ", tel, 12);
    ler_string("|  Novo email: ", email, 50);
    ler_string("|  Novo nome do contato: ", nome_contato, 50);
    printf("+ --------------------------------------------------------------- +\n");
}
