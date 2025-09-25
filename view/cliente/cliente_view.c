#include <stdio.h>
#include "cliente/cliente_view.h"
#include "../../util/util.h"

// void exibir_cliente(const Cliente* cliente) {
//     if (!cliente) {
//         printf("Cliente inexistente!\n");
//         return;
//     }
//     printf("ID: %d | Nome: %s | Idade: %d\n",
//            cliente->id, cliente->nome, cliente->idade);
// }


void exibir_cliente(const Cliente* cliente) {
    if (!cliente) {
        printf("+--------------------------+\n");
        printf("| Cliente inexistente!     |\n");
        printf("+--------------------------+\n");
        return;
    }
    printf("+--------------------------+\n");
    printf("| ID   : %d\n", cliente->id);
    printf("| Nome : %s\n", cliente->nome);
    printf("| Idade: %d\n", cliente->idade);
    //printf("Telefone: (%c%c) %c%c%c%c%c-%c%c%c%c\n",
//        cliente.tel[0], cliente.tel[1],    // DDD
//        cliente.tel[2], cliente.tel[3], cliente.tel[4], cliente.tel[5], cliente.tel[6], // primeiros 5
//        cliente.tel[7], cliente.tel[8], cliente.tel[9], cliente.tel[10]); // últimos 4
// );
    printf("+--------------------------+\n");
}

int exibir_menu() {
    int opcao;
    printf("\n==== MENU CLIENTE ====\n");
    printf("1 - Criar cliente\n");
    printf("2 - Atualizar cliente\n");
    printf("3 - Exibir cliente\n");
    printf("4 - Deletar cliente\n");
    printf("5 - Voltar ao menu anterior\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

Cliente ler_dados_cliente() {
    Cliente c;

    printf("ID: ");
    scanf("%d", &c.id);
    printf("Nome: ");
    scanf(" %[^\n]", c.nome);
    printf("Idade: ");
    scanf("%d", &c.idade);
    printf("Endereço completo: ");
    scanf(" %[^\n]", c.endereco_completo);
    printf("CPF/CNPJ: ");
    scanf(" %[^\n]", c.cpf_cnpj);
    limpar_digitos(c.cpf_cnpj);  // mantém só números no CPF/CNPJ
    printf("Telefone: ");
    scanf(" %[^\n]", c.tel);
    limpar_digitos(c.tel);  // mantém só números no telefone
    printf("E-mail: ");
    scanf(" %[^\n]", c.email);
    printf("Nome do contato: ");
    scanf(" %[^\n]", c.nome_contato);
    return c;  // retorna a struct preenchida
}

void ler_dados_atualizados_cliente(
    char* nome,              
    int* idade,              
    char* endereco_completo, 
    char* cpf_cnpj,          
    char* tel,               
    char* email,             
    char* nome_contato      
) {

    printf("Novo nome: ");
    scanf(" %[^\n]", nome);
    printf("Nova idade: ");
    scanf("%d", idade);
    printf("Novo endereco completo: ");
    scanf(" %[^\n]", endereco_completo);
    printf("Novo CPF/CNPJ: ");
    scanf(" %[^\n]", cpf_cnpj);
    printf("Novo telefone: ");
    scanf(" %[^\n]", tel);
    printf("Novo email: ");
    scanf(" %[^\n]", email);
    printf("Novo nome do contato: ");
    scanf(" %[^\n]", nome_contato);
}
//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
