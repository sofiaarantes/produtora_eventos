#include <stdio.h>
#include "main/main_view.h"
int primeiro_menu() {
    int escolher;
     printf("+--------------------------+\n");
        printf("| Seja Bem Vindo!     |\n");
        printf("+--------------------------+\n");
    printf("\n==== MENU ==== agora\n");
    printf("1 - Acessar Menu Clientes\n");
    printf("2 - Acessar Menu Produtora\n");
    printf("3 - Acessar Equipe Interna\n");
    printf("4 - Acessar Menu Recursos e Equipamentos\n");
    printf("5 - Acessar Menu Fornecedores e Parceiros\n");
    printf("4 - Acessar Menu Operadores do Sistema\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &escolher);
    return escolher;
}