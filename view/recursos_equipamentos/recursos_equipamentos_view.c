#include <stdio.h>
#include "recursos_equipamentos/recursos_equipamentos_view.h"

void exibir_equipamento(const RecursosEquipamentos* equipamentos) {
    if (!equipamentos) {
        printf("+--------------------------+\n");
        printf("| Equipamento inexistente! |\n");
        printf("+--------------------------+\n");
        return;
    }
    printf("+-------------------------------+\n");
    printf("| ID: %d\n", equipamentos->id);
    printf("| Descrição: %s\n", equipamentos->descricao);
    printf("| Categoria: %s\n", equipamentos->categoria);
    printf("| Quantidade no Estoque: %d\n", equipamentos->qtd_estoque);
    printf("| Preço de Custo: R$ %.2f\n", equipamentos->preco_custo);
    printf("| Valor da Diária/Hora: R$ %.2f\n", equipamentos->valor_diaria);
    printf("+-------------------------------+\n");
}

int exibir_menu_equipamentos() {
    int opcao;
    printf("\n==== MENU RECURSOS E EQUIPAMENTOS ====\n");
    printf("1 - Adicionar novo recurso/equipamento\n");
    printf("2 - Atualizar recurso/equipamento\n");
    printf("3 - Exibir recurso/equipamento\n");
    printf("4 - Deletar recurso/equipamento\n");
    printf("0 - Voltar ao menu anterior\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
RecursosEquipamentos ler_dados_equipamento() {
    RecursosEquipamentos e;
    printf("Descrição: "); scanf(" %[^\n]", e.descricao);
    printf("Categoria: \n"); scanf(" %[^\n]", e.categoria); 
    printf("Quantidade no Estoque: "); scanf(" %d", &e.qtd_estoque);
    printf("Preço de Custo: "); scanf("%f", &e.preco_custo);
    printf("Valor da Diária/hora: "); scanf("%f", &e.valor_diaria);
    return e;  // Retorna a struct preenchida
}

void ler_dados_atualizados_equipamento(char* descricao, char* categoria, int* qtd_estoque, float* preco_custo, float* valor_diaria) {
    printf("Nova descrição: "); scanf(" %[^\n]", descricao);
    printf("Nova categoria: "); scanf(" %[^\n]", categoria);
    printf("Nova quantidade no estoque: "); scanf("%d", qtd_estoque);
    printf("Novo preço de custo: "); scanf("%f", preco_custo);
    printf("Novo valor da diária: "); scanf("%f", valor_diaria);
}
