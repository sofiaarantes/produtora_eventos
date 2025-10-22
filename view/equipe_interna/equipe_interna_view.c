#include <stdio.h>
#include "equipe_interna/equipe_interna_view.h"
#include "../../util/util.h"

void exibir_funcionario(const EquipeInterna* equipeInterna) {
    if (!equipeInterna) {
        printf("+ --------------------------------------------------------------- +\n");
        printf("|                     Funcionario inexistente!                    |\n");
        printf("+ --------------------------------------------------------------- +\n");
        return;
    }
    printf("\n+ --------------------------------------------------------------- +\n");
    printf("| Nome  : %s\n", equipeInterna->nome);
    printf("| CPF   : %s\n", equipeInterna->cpf);
    printf("| Funcao: %s\n", equipeInterna->funcao);
    printf("| Valor da Diária/Hora: %f\n", equipeInterna->valor_diaria);
    printf("+ --------------------------------------------------------------- +\n");
}

int exibir_menu_equipe_interna() {
    int opcao;
    printf("\n===================================================================\n");
    printf("|                       Menu Equipe Interna                       |\n");
    printf("===================================================================\n");
    printf("|  1 - Adicionar funcionario                                      |\n");
    printf("|  2 - Atualizar funcionario                                      |\n");
    printf("|  3 - Exibir funcionario                                         |\n");
    printf("|  4 - Deletar funcionario                                        |\n");
    printf("|  0 - Voltar ao menu anterior                                    |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
EquipeInterna ler_dados_funcionario() {
    EquipeInterna f;
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                         Novo Funcionário                        |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Nome: ", f.nome, sizeof(f.nome));
    ler_string("|  CPF: ", f.cpf, sizeof(f.cpf));
    ler_string("|  Funcao: ", f.funcao, sizeof(f.funcao));
    ler_float("|  Valor da Diaria/hora: ", &f.valor_diaria);
    printf("+ --------------------------------------------------------------- +\n");
    return f;
}

void ler_dados_atualizados_funcionario(char* nome, char* funcao, float* valor_diaria) {
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                      Atualizar Funcionário                      |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Novo nome: ", nome, sizeof(nome));
    ler_string("|  Nova funcao: ", funcao, sizeof(funcao));
    ler_float("|  Novo valor da diaria: ", valor_diaria);
    printf("+ --------------------------------------------------------------- +\n");
}
