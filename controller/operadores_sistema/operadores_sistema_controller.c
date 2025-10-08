#include <stdio.h>
#include <string.h>
#include "../../model/operadores_sistema/operadores_sistema.h"
#include "../../view/operadores_sistema/operadores_sistema_view.h"
#include "../../controller/config_armazenamento/config_armazenamento_controller.h"
#include "../../view/main/main_view.h"
#include "../../controller/main/main_controller.h"

void criptografar_senha(char* senha) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] = senha[i] + 3; // desloca cada caractere 3 posições (Cesar cipher)
    }
}

void gerenciar_login() {
    int opcao;
    do {
        opcao = exibir_menu_operadores();

        if (opcao == 1) { // Login
            Operadores o = ler_dados_operador_login();
            criptografar_senha(o.senha);

            FILE* fp = fopen("operadores.txt", "r");
            if (!fp) {
                printf("\nNenhum operador cadastrado ainda. Tente novamente com outras credenciais.\n");
                continue; // volta ao menu inicial
            }

            Operadores tmp;
            int encontrado = 0;
            while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];\n",
                          &tmp.id, tmp.nome, tmp.usuario, tmp.senha) != EOF) {
                if (strcmp(o.usuario, tmp.usuario) == 0 &&
                    strcmp(o.senha, tmp.senha) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            fclose(fp);

            if (encontrado) {
                printf("\nLogin realizado com sucesso! Bem-vindo, %s.\n", tmp.nome);
                menu_main();
            } else {
                printf("\nCredenciais inválidas! Tente novamente.\n");
            }

        } else if (opcao == 2) { // Cadastro
            Operadores novo = ler_dados_operador_cadastro();
            criptografar_senha(novo.senha);
            adicionar_operador(&novo);
            printf("\nCadastro realizado com sucesso! Bem-vindo(a), %s.\n", novo.usuario);
            menu_main();

        } else if (opcao == 3) {
            printf("\nSaindo do sistema... Até logo!\n");
        } else {
            printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 3);
}
