#include <stdio.h>
#include "operadores_sistema/operadores_sistema_view.h"
#include "main/main_view.h"

int exibir_menu_operadores() {
    int opcao;
    printf("==================================\n");
    printf("--- BEM VINDO AO NOSSO SISTEMA ---\n");
    printf("==================================\n");
    printf("1 - Já tenho uma conta, quero fazer login\n");
    printf("2 - Ainda não tenho uma conta, quero me registrar\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

Operadores ler_dados_operador_login() {
    Operadores o;
    printf("Usuário: "); scanf(" %[^\n]", o.usuario); 
    printf("Senha: "); scanf(" %[^\n]", o.senha); 
    return o;  // Retorna a struct preenchida
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
Operadores ler_dados_operador_cadastro() {
    Operadores o;
    printf("Para continuar, nos informe seus dados de login:\n");
    printf("Nome: "); scanf(" %[^\n]", o.nome); 
    printf("Usuário: "); scanf(" %[^\n]", o.usuario); 
    printf("Senha: "); scanf(" %[^\n]", o.senha); 
    int opcao = mostrar_menu_configuracao(); // mostra menu e retorna a opção
    switch (opcao) {
        case 1:
            o.tipo = MEMORIA;
            break;
        case 2:
            o.tipo = TEXTO;
            break;
        case 3:
            o.tipo = BINARIO;
            break;
        default:
            o.tipo = MEMORIA; 
            break;
    }
    return o;  // Retorna a struct preenchida
}

void exibir_operador(const Operadores* operador) {
    if (!operador) {
        printf("+--------------------------+\n");
        printf("|   Operador inexistente!  |\n");
        printf("+--------------------------+\n");
        return;
    }
    const char* tipo_str;
    switch (operador->tipo) {
        case MEMORIA:
            tipo_str = "Memória";
            break;
        case TEXTO:
            tipo_str = "Texto";
            break;
        case BINARIO:
            tipo_str = "Binário";
            break;
        default:
            tipo_str = "Desconhecido";
            break;
    }
    printf("+-----------------------+\n");
    printf("| Nome: %s\n", operador->nome);
    printf("| Usuário: %s\n", operador->usuario);
    printf("| Armazenamento: %s\n", tipo_str);
    printf("+-----------------------+\n");
}

int exibir_menu_operadores_editar() {
    int opcao;
    printf("=================\n");
    printf("----- CONTA -----\n");
    printf("=================\n");
    printf("1 - Editar dados\n");
    printf("1 - Editar método de armazenamento\n");
    printf("2 - Deletar conta\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

void ler_dados_atualizados_operador(char* nome, char* usuario) {
    printf("\n--- Editar Dados ---\n");
    printf("Novo nome: "); scanf(" %[^\n]", nome);
    printf("Novo usuário: "); scanf(" %[^\n]", usuario);
}
