#include <stdio.h>
#include "recursos_equipamentos/recursos_equipamentos_view.h"
#include "../../util/util.h"

void exibir_equipamento(const RecursosEquipamentos* equipamentos) {
    if (!equipamentos) {
        printf("+ --------------------------------------------------------------- +\n");
        printf("|                    Equipamento inexistente!                     |\n");
        printf("+ --------------------------------------------------------------- +\n");
        return;
    }
    printf("+ --------------------------------------------------------------- +\n");
    printf("| ID: %d\n", equipamentos->id);
    printf("| Descrição: %s\n", equipamentos->descricao);
    printf("| Categoria: %s\n", equipamentos->categoria);
    printf("| Quantidade no Estoque: %d\n", equipamentos->qtd_estoque);
    printf("| Preço de Custo: R$ %.2f\n", equipamentos->preco_custo);
    printf("| Valor da Diária/Hora: R$ %.2f\n", equipamentos->valor_diaria);
    printf("+ --------------------------------------------------------------- +\n");
}

int exibir_menu_equipamentos() {
    int opcao;
    printf("\n===================================================================\n");
    printf("|                   Menu Recursos e Equipamentos                  |\n");
    printf("===================================================================\n");
    printf("|  1 - Adicionar novo recurso/equipamento                         |\n");
    printf("|  2 - Atualizar recurso/equipamento                              |\n");
    printf("|  3 - Exibir recurso/equipamento                                 |\n");
    printf("|  4 - Deletar recurso/equipamento                                |\n");
    printf("|  0 - Voltar ao menu anterior                                    |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

//O espaço antes do % → ignora quebras de linha e espaços anteriores (evita problemas com entradas seguidas).
RecursosEquipamentos ler_dados_equipamento() {
    RecursosEquipamentos e;
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                     Novo Recurso/Equipamento                    |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Descrição: ", e.descricao, sizeof(e.descricao));
    ler_string("|  Categoria: ", e.categoria, sizeof(e.categoria)); 
    ler_int("|  Quantidade no Estoque: ", &e.qtd_estoque);
    ler_float("|  Preço de Custo: ", &e.preco_custo);
    ler_float("|  Valor da Diária/hora: ", &e.valor_diaria);
    printf("+ --------------------------------------------------------------- +\n");
    return e;  // Retorna a struct preenchida
}

void ler_dados_atualizados_equipamento(char* descricao, char* categoria, int* qtd_estoque, float* preco_custo, float* valor_diaria) {
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                  Atualizar Recurso/Equipamento                  |\n");
    printf("+ --------------------------------------------------------------- +\n");
    ler_string("|  Nova descrição: ", descricao, sizeof(descricao));
    ler_string("|  Nova categoria: ", categoria, sizeof(categoria));
    ler_int("|  Nova quantidade no estoque: ", qtd_estoque);
    ler_float("|  Novo preço de custo: ", preco_custo);
    ler_float("|  Novo valor da diária: ", valor_diaria);
    printf("+ --------------------------------------------------------------- +\n");
}
