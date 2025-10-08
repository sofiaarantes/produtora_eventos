#include <stdio.h>
#include "../view/main/main_view.h"

int menu_main() {
    int escolher = 0;
    printf("+--------------------------+\n");
    printf("|      Seja Bem Vindo!     |\n");
    printf("+--------------------------+\n");
    printf("\n=========== MENU ===========\n");
    printf("1 - Acessar Menu Cliente\n");
    printf("2 - Acessar Menu Produtora\n");
    printf("3 - Acessar Equipe Interna\n");
    printf("4 - Acessar Menu Recursos e Equipamentos\n");
    printf("5 - Acessar Menu Fornecedores e Parceiros\n");
    printf("6 - Acessar Menu Operadores do Sistema\n");
    printf("7 - Alterar método de armazenamento\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &escolher);
    return escolher;
}

void exibir_mensagem(const char* msg) {
    printf("%s\n", msg);
}

// Exibe o menu de configuração do tipo de armazenamento
int mostrar_menu_configuracao() {
    int opcao = 0;
    printf("\n=== Configuração de Armazenamento ===\n");
    printf("Em qual formato gostaria de salvar as informações inseridas no sistema?\n");
    printf("1 - Memória (dados temporários)\n");
    printf("2 - Arquivo Texto\n");
    printf("3 - Arquivo Binário\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}