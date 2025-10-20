#include <stdio.h>
#include "operadores_sistema/operadores_sistema_view.h"
#include "main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "../../controller/main/main_controller.h"
#include "../../util/util.h"

int exibir_menu_operadores() {
    int opcao;
    printf("===================================================================\n");
    printf("------------------- BEM VINDO AO NOSSO SISTEMA --------------------\n");
    printf("===================================================================\n");
    printf("|  1 - Ja tenho uma conta, quero fazer login!                     |\n");
    printf("|  2 - Ainda nao tenho uma conta, quero me registrar!             |\n");
    printf("|  0 - Sair                                                       |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

Operadores ler_dados_operador_login() {
    Operadores o;
    printf("============================== Login ==============================\n");
    ler_string("|  Usuario: ", o.usuario, sizeof(o.usuario));
    ler_string("|  Senha: ", o.senha, sizeof(o.senha));
    printf("===================================================================\n");
    return o;  // Retorna a struct preenchida
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
Operadores ler_dados_operador_cadastro() {
    Operadores o;
    printf("\n============================ Cadastro =============================\n");
    ler_string("|  Nome: ", o.nome, sizeof(o.nome));
    ler_string("|  Usuario: ", o.usuario, sizeof(o.usuario));
    ler_string("|  Senha: ", o.senha, sizeof(o.senha));
    printf("+ --------------------------------------------------------------- +\n");
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
        case 0:
            exibir_mensagem("Saindo do cadastro...");
            // Sinaliza que o cadastro foi cancelado
            o.tipo = 0;
            return o;
        default:
            exibir_mensagem("Opção invalida. Cadastro cancelado.");
            o.tipo = 0;
            return o;
    }
    printf("===================================================================\n");
    return o;
}

void exibir_operador(const Operadores* operador) {
    if (!operador) {
        printf("+ --------------------------------------------------------------- +\n");
        printf("|                      Operador inexistente!                      |\n");
        printf("+ --------------------------------------------------------------- +\n");
        return;
    }
    const char* tipo_str;
    switch (operador->tipo) {
        case MEMORIA:
            tipo_str = "Memoria";
            break;
        case TEXTO:
            tipo_str = "Texto";
            break;
        case BINARIO:
            tipo_str = "Binario";
            break;
        default:
            tipo_str = "Desconhecido";
            break;
    }
    printf("+ --------------------------------------------------------------- +\n");
    printf("| Nome: %s\n", operador->nome);
    printf("| Usuario: %s\n", operador->usuario);
    printf("| Armazenamento: %s\n", tipo_str);
    printf("+ --------------------------------------------------------------- +\n");
}

int exibir_menu_operadores_editar() {
    int opcao;
    printf("===================================================================\n");
    printf("|                           Minha Conta                           |\n");
    printf("===================================================================\n");
    printf("|  1 - Editar dados                                               |\n");
    printf("|  2 - Editar metodo de armazenamento                             |\n");
    printf("|  3 - Deletar conta                                              |\n");
    printf("|  0 - Sair                                                       |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

void ler_dados_atualizados_operador(char* nome, char* usuario) {
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                           Editar Dados                          |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Novo nome: ", nome, sizeof(nome));
    ler_string("|  Novo usuario: ", usuario,sizeof(usuario));
    printf("+ --------------------------------------------------------------- +\n");
}
