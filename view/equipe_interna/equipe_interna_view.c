#include <stdio.h>
#include "equipe_interna/equipe_interna_view.h"

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
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
EquipeInterna ler_dados_funcionario() {
    EquipeInterna f;
    printf("ID: "); scanf("%d", &f.id);
    printf("Nome: "); scanf(" %[^\n]", f.nome);
    printf("CPF: (Cuidado ao inserir esse dado, ele nao podera ser alterado depois)\n"); 
    scanf(" %[^\n]", f.cpf); 
    printf("Função: "); scanf(" %[^\n]", f.funcao);
    printf("Valor da Diária/hora: "); scanf("%f", &f.valor_diaria);
    return f;  // Retorna a struct preenchida
}

void ler_dados_atualizados_funcionario(char* nome, char* funcao, float* valor_diaria) {
    printf("Novo nome: "); scanf(" %[^\n]", nome);
    printf("Nova função: "); scanf(" %[^\n]", funcao);
    printf("Novo valor da diária: "); scanf("%f", valor_diaria);
}
