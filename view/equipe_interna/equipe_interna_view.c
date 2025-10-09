#include <stdio.h>
#include "equipe_interna/equipe_interna_view.h"
#include "../../util/util.h"

void exibir_funcionario(const EquipeInterna* equipeInterna) {
    if (!equipeInterna) {
        printf("+--------------------------+\n");
        printf("| Funcionário inexistente! |\n");
        printf("+--------------------------+\n");
        return;
    }
    printf("+-------------------------------+\n");
    printf("| ID    : %d\n", equipeInterna->id);
    printf("| Nome  : %s\n", equipeInterna->nome);
    printf("| CPF   : %s\n", equipeInterna->cpf);
    printf("| Funcao: %s\n", equipeInterna->funcao);
    printf("| Valor da Diária/Hora: %f\n", equipeInterna->valor_diaria);
    printf("+-------------------------------+\n");
}

int exibir_menu_equipe_interna() {
    int opcao;
    printf("\n==== MENU EQUIPE INTERNA ====\n");
    printf("1 - Adicionar funcionario\n");
    printf("2 - Atualizar funcionário\n");
    printf("3 - Exibir funcionário\n");
    printf("4 - Deletar funcionário\n");
    printf("0 - Voltar ao menu anterior\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
EquipeInterna ler_dados_funcionario() {
    EquipeInterna f;
    printf("\n==== DADOS DO FUNCIONÁRIO ====\n");
    ler_string("Nome: ", f.nome, sizeof(f.nome));
    ler_string("CPF: (Cuidado ao inserir esse dado, ele nao podera ser alterado depois)\n", f.cpf, sizeof(f.cpf));
    ler_string("Função: ", f.funcao, sizeof(f.funcao));
    ler_float("Valor da Diária/hora: ", &f.valor_diaria);
    return f;  // Retorna a struct preenchida
}

void ler_dados_atualizados_funcionario(char* nome, char* funcao, float* valor_diaria) {
    ler_string("Novo nome: ", nome, sizeof(nome));
    ler_string("Nova função: ", funcao, sizeof(funcao));
    ler_float("Novo valor da diária: ", valor_diaria);
}
