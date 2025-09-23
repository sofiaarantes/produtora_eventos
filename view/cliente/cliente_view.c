#include <stdio.h>
#include "cliente/cliente_view.h"

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
    printf("ID: "); scanf("%d", &c.id);
    printf("Nome: "); scanf(" %[^\n]", c.nome);
    printf("Idade: "); scanf("%d", &c.idade);
    printf("Endereço completo: "); scanf("%s",c.endereco_completo);
    printf("CPF/CNPJ: "); scanf("%s",c.cpf_cnpj);
    printf("Telefone: "); scanf("%s",c.tel);
    printf("E-mail: "); scanf("%s",c.email);
    printf("Nome do contato: "); scanf("%s",c.nome_contato);
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
