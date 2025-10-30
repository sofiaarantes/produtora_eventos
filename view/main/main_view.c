#include <stdio.h>
#include "../view/main/main_view.h"
#include "../../util/util.h"

int menu_main() {
    int escolher = 0;
    printf("\n===================================================================\n");
    printf("|                          Menu Principal                         |\n");
    printf("===================================================================\n");
    printf("|  1 - Acessar Menu Cliente                                       |\n");
    printf("|  2 - Acessar Menu Produtora                                     |\n");
    printf("|  3 - Acessar Menu Equipe Interna                                |\n");
    printf("|  4 - Acessar Menu Recursos e Equipamentos                       |\n");
    printf("|  5 - Acessar Menu Fornecedores e Parceiros                      |\n");
    printf("|  6 - Minha Conta                                                |\n");
    printf("|  0 - Sair                                                       |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &escolher);
    return escolher;
}

void exibir_mensagem(const char* msg) {
    printf("%s\n", msg);
}

// Exibe o menu de configuração do tipo de armazenamento
int mostrar_menu_configuracao() {
    int opcao;
    printf("\n========================== Armazenamento ==========================\n");
    printf("| Em qual formato gostaria de salvar suas informacoes?            |\n");
    printf("|  1 - Memoria (dados temporarios)                                |\n");
    printf("|  2 - Arquivo Texto                                              |\n");
    printf("|  3 - Arquivo Binario                                            |\n");
    printf("|  0 - Sair                                                       |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}