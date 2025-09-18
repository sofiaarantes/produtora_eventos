#include <stdio.h>
#include "equipe_interna/equipe_interna_view.h"

void exibir_funcionario(const EquipeInterna* equipeInterna) {
    if (!equipeInterna) {
        printf("+--------------------------+\n");
        printf("| Funcionário inexistente!     |\n");
        printf("+--------------------------+\n");
        return;
    }
    printf("+--------------------------+\n");
    printf("| ID    : %d\n", equipeInterna->id);
    printf("| Nome  : %s\n", equipeInterna->nome);
    printf("| CPF   : %d\n", equipeInterna->cpf);
    printf("| Funcao: %s\n", equipeInterna->funcao);
    printf("| Valor da Diária/Hora: %.2f\n", equipeInterna->valor_diaria);
    printf("+--------------------------+\n");
}



int exibir_menu_equipe_interna() {
    int opcao;
    printf("\n==== MENU EQUIPE INTERNA ====\n");
    printf("1 - Adicionar funcionário\n");
    printf("2 - Atualizar funcionário\n");
    printf("3 - Exibir funcionário\n");
    printf("4 - Deletar funcionário\n");
    printf("5 - Voltar ao menu anterior\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

EquipeInterna ler_dados_funcionario() {
    EquipeInterna c;
    printf("ID: "); scanf("%d", &c.id);
    printf("Nome: "); scanf(" %[^\n]", c.nome);
    printf("CPF: "); scanf("%d", &c.cpf);
    printf("Função: "); scanf(" %[^\n]", c.funcao);
    printf("Valor da Diária/hora: "); scanf("%.2f", &c.valor_diaria);
    return c;  // retorna a struct preenchida
}

void ler_dados_atualizacao(char* nome, int* cpf, char* funcao, float* valor_diaria) {
    printf("Novo nome: "); scanf(" %[^\n]", nome);
    printf("Novo CPF: "); scanf("%d", cpf);
    printf("Nova Função: "); scanf(" %[^\n]", funcao);
}

void exibir_mensagem(const char* msg) {
    printf("%s\n", msg);
}
