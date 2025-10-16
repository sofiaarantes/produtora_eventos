#include <stdio.h>
#include "operadores_sistema/operadores_sistema_view.h"
#include "main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
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

    int opcao = mostrar_menu_configuracao();
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

    exibir_mensagem("Armazenamento configurado!");
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
    printf("2 - Editar método de armazenamento\n");
    printf("3 - Deletar conta\n");
    printf("0 - Sair\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

void ler_dados_atualizados_operador(char* nome, char* usuario) {
    printf("\n--- Editar Dados ---\n");
    ler_string("Novo nome: ", nome, sizeof(nome));
    ler_string("Novo usuário: ", usuario,sizeof(usuario));
}
