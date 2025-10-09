#include <stdio.h>
#include "operadores_sistema/operadores_sistema_view.h"
#include "main/main_view.h"
#include "../../util/util.h"

int exibir_menu_operadores() {
    int opcao;
    printf("==================================\n");
    printf("--- BEM VINDO AO NOSSO SISTEMA ---\n");
    printf("==================================\n");
    printf("1 - Ja tenho uma conta, quero fazer login\n");
    printf("2 - Ainda não tenho uma conta, quero me registrar\n");
    printf("3 - Sair\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

Operadores ler_dados_operador_login() {
    Operadores o;
    ler_string("Usuário: ", o.usuario, sizeof(o.usuario));
    ler_string("Senha: ", o.senha, sizeof(o.senha));
    return o;  // Retorna a struct preenchida
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
Operadores ler_dados_operador_cadastro() {
    Operadores o;
    printf("Para continuar, nos informe seus dados de login:\n");
    ler_string("Nome: ", o.nome, sizeof(o.nome));
    ler_string("Usuário: ", o.usuario, sizeof(o.usuario));
    ler_string("Senha: ", o.senha, sizeof(o.senha));
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
