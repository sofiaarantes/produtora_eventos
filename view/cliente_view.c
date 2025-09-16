#include <stdio.h>
#include "cliente_view.h"

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
    return c;  // retorna a struct preenchida
}

void ler_dados_atualizacao(char* nome, int* idade) {
    printf("Novo nome: "); scanf(" %[^\n]", nome);
    printf("Nova idade: "); scanf("%d", idade);
}

void exibir_mensagem(const char* msg) {
    printf("%s\n", msg);
}
